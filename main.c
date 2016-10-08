#include "myfunc.h"

int main(void)
{
	/*定义打开各个文件的描述符*/
	//int diskmsg_fd = -2;
	//int sdbcnt_fd = -2;
	//int dfmsg_fd = -2;
	int mntcnt_fd = -2;
	int mytest_log_fd = -2;

	/*记录U盘和分区数量*/
	//char sdbcnt = 0;
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
	const char *tmptxt = "./mntcnt.txt";

	//diskmsg_fd = open_txt("./diskmsg.txt");
	//sdbcnt_fd = open_txt("./sdbcnt.txt");
	//dfmsg_fd = open_txt("./dfmsg.txt");
	mntcnt_fd = open_txt(tmptxt);

	//status = system(diskcmd); /*获取磁盘信息*/
	//stat_check(status, diskcmd);

	//status = system(sdbcnt_cmd); /*检测是否有/dev/sdb设备*/
	//stat_check(status, sdbcnt_cmd);

	//status = system(dfcmd); /*获取已挂载的分区信息*/
	//stat_check(status, dfcmd);

	status = system(mntcnt_cmd); /*获取/dev/sdb1挂载信息，因为有些系统识别磁盘后会自动挂载分区*/
	stat_check(status, mntcnt_cmd);
	
	ret = read(mntcnt_fd, buf, 1); /*获得/dev/sdb1分区已挂载次数*/
	if (ret == -1)
	{
		Log("read mntcnt");
		close(mntcnt_fd);
		exit(-1);		
	}
	mntcnt = atoi(buf);
	if (mntcnt > 0)
	{
		printf("/dev/sdb1 device mounted num:%d，start umount...\n", mntcnt);
		while(mntcnt--) /*实际应用中一般不会将文件系统(分区)挂载在多个目录下*/
		{
			status = system(umntcmd); /*格式化前先卸载原来的同名分区，由于不知道自动挂载的目录名，所以此处不能用umount函数*/
			stat_check(status, umntcmd);
		}
		printf("first umount /dev/sdb1 success.\n");
		close(mntcnt_fd);
	}

	/*memset(buf, 0, strlen(buf)+1);
	ret = read(sdbcnt_fd, buf, 1);
	if (ret == -1)
	{
		perror("read sdbcnt");
		exit(-1);
	}
	sdbcnt = atoi(buf);
	if (sdbcnt <= 0)
	{
		printf("no USB disk!\n");
		return 0;
	}
	else 
	{*/
		printf("make a partition for /dev/sdb, capacity is the whole.\n");
		status = system(fdiskcmd); /*为/dev/sdb设备建立分区*/
		stat_check(status, fdiskcmd);
	//}

	status = system(fmtcmd); /*格式化sdb1分区文件系统为FAT32*/
	stat_check(status, fmtcmd);

	ret = mk_dir(mntpoint); /*创建挂载点目录*/
	if (ret == -1)
	{
		Log("create mntdir fail");
		exit(-1);
	}

	//status = system(mntcmd); /*挂载/dev/sdb1分区*/
	//stat_check(status, mntcmd);
	ret = mount("/dev/sdb1", "/mnt/upoint", "vfat", 0, "iocharset=utf8");
	if (ret == -1)
	{
		Log("mount /dev/sdb1 fail");
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

		printf("write mytest.log complete, start umount /dev/sdb1...\n");
		//status = system(umntcmd);
		//stat_check(status, umntcmd);
		ret = umount(mntpoint);
		if (ret == -1)
		{
			Log("final umount /dev/sdb1 fail");
		}
		printf("final umount /dev/sdb1 complete.\n");
	}
	
	ret = unlink(tmptxt);
	if (ret == -1)
	{
		Log("unlink mntcnt.txt fail");
		exit(-1);
	}
	//close(diskmsg_fd);
	//close(sdbcnt_fd);
	//close(dfmsg_fd);
	//close(mntcnt_fd);
	
	return 0;
}

