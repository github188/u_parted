#include "myfunc.h"

int main(void)
{
	/*定义打开各个文件的描述符*/
	int mntcnt_fd = -1;
	int mytest_log_fd = -1;

	/*记录分区数量*/
	char mntcnt = 0;

	/*定义接收system函数返回值变量*/
	int status = 0;	

	/*存储临时信息变量*/
	char buf[BUFSIZE] = {0};
	char ret = 0;

	/*挂载点根目录/mnt/upoint 挂载点log子目录，log下文件mytest.log*/
	const char *mntpoint = "/mnt/upoint";
	const char *subdir = "/mnt/upoint/log"; 
	const char *file = "/mnt/upoint/log/mytest.log";
	const char *tmptxt = "mntcnt.txt";

	/*存储构建的命令字符串的设备名*/
	char dev[BUFSIZE] = {0};		

	/*定义各种命令变量*/
	char mntcnt_cmd[CMDSIZE] = {0};
	char delpart_cmd[CMDSIZE] = {0};
	char parted_cmd[CMDSIZE] = {0};
    char fmt_cmd[CMDSIZE] = {0};
	char umnt_cmd[CMDSIZE] = {0};
	char mnt_cmd[CMDSIZE] = {0};

	mntcnt_fd = open_txt(tmptxt);

	/*构建分区/格式化/删除分区命令字符串*/
	/*VFS中的/proc文件不可以定位到结尾，故定向到另一个文件中,便于后续使用*/
	system("cat /proc/partitions > mntcnt.txt");
	ret = lseek(mntcnt_fd, -5, SEEK_END);
	if (ret == -1)
	{
		perror("lseek");
		close(mntcnt_fd);
		exit(-1);
	}
	ret = read(mntcnt_fd, dev, 3); /*读取磁盘设备名*/
	if (ret == -1)
	{
		close(mntcnt_fd);
		perror("read usb device error");
		close(mntcnt_fd);
		exit(-1);
	}

	sprintf(mntcnt_cmd, "grep -c /dev/%s1 /etc/mtab > mntcnt.txt", dev);
	sprintf(delpart_cmd, "(echo d && echo w) | fdisk /dev/%s", dev);
	sprintf(parted_cmd, "echo Ignore | parted /dev/%s mkpart primary 0 31GB", dev);
	sprintf(umnt_cmd, "umount /dev/%s1", dev);
	sprintf(fmt_cmd, "mkfs.vfat /dev/%s1", dev);	
	sprintf(mnt_cmd, "/dev/%s1", dev);

	ret = lseek(mntcnt_fd, 0, SEEK_SET);
	if (ret == -1)
	{
		perror("lseek");
		close(mntcnt_fd);
		exit(-1);
	}
	status = system(mntcnt_cmd); /*获取/dev/sdb1挂载信息，有些系统识别磁盘后会自动挂载分区*/
	stat_check(status, mntcnt_cmd);
	ret = read(mntcnt_fd, buf, 1); /*获得/dev/sdb1分区已挂载次数*/
	if (ret == -1)
	{
		Log("unlink mntcnt.txt fail");
		exit(-1);
	}
	if (ret == -1)
	{
		Log("read mntcnt");
		close(mntcnt_fd);
		exit(-1);		
	}
	unlink(tmptxt);

	mntcnt = atoi(buf);
	if (mntcnt > 0)
	{
		printf("/dev/%s1 device mounted num:%d，start umount...\n", dev, mntcnt);
		while(mntcnt--) /*实际应用中一般不会将文件系统(分区)挂载在多个目录下*/
		{
			/*格式化前先卸载原来的同名分区，由于不知道自动挂载的目录名，所以此处不能用umount函数*/
			status = system(umnt_cmd); 
			stat_check(status, umnt_cmd);
		}
		printf("first umount /dev/%s1 success.\n", dev);
		close(mntcnt_fd);
	}

	/*分区前先删除分区，否则出现建立分区重叠，无法创建新的分区*/
	status = system(delpart_cmd);
	stat_check(status, delpart_cmd);
	
	/*为/dev/sdb设备建立分区*/	
	printf("make a partition for /dev/%s, capacity is the whole.\n", dev);
	status = system(parted_cmd); 
	stat_check(status, parted_cmd);

	/*格式化sdb1分区文件系统为FAT32*/
	status = system(fmt_cmd); 
	stat_check(status, fmt_cmd);

	ret = mk_dir(mntpoint); /*创建挂载点目录*/
	if (ret == -1)
	{
		Log("create mntdir fail");
		exit(-1);
	}

	/*挂载磁盘设备*/
	ret = mount(mnt_cmd, "/mnt/upoint", "vfat", 0, "iocharset=utf8");
	if (ret == -1)
	{
		Log("mount /dev/%s1 fail", dev);
		exit(-1);
	}

	ret = mk_dir(subdir); /*在挂载点下创建Log目录*/
	if (ret == -1)
	{
		Log("create subdir fail");
		exit(-1);
	}
	else
	{
		/*在挂载点子目录Log下创建mytest.log文件*/
		char wrbuf[20] = "hello world";
		mytest_log_fd = open(file, O_RDWR|O_CREAT|O_TRUNC, 0666); 
		if (mytest_log_fd == -1)
		{
			Log("create mytest.log fail");
			exit(-1);
		}
		ret = write(mytest_log_fd, wrbuf, strlen(wrbuf));
		if (ret == -1)
		{
			Log("write mytest.log fail");
			exit(-1);
		}
		close(mytest_log_fd); /*卸载/dev/sdb1前，关闭挂载点中打开的文件*/

		printf("write mytest.log complete, start umount /dev/%s1...\n", dev);
		ret = umount(mntpoint);/*卸载磁盘设备*/
		if (ret == -1)
		{
			Log("final umount /dev/%s1 fail", dev);
		}
		printf("final umount /dev/%s1 complete.\n", dev);
	}
	
	return 0;
}

