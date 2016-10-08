#include "myfunc.h"

int main(void)
{
	/*定义打开各个文件的描述符*/
	int diskmsg_fd = -2;
	int sdbcnt_fd = -2;
	int dfmsg_fd = -2;
	int mntcnt_fd = -2;
	int mytest_log_fd = -2;

	/*记录U盘和分区数量*/
	char sdbcnt = 0;
	char mntcnt = 0;

	/*定义接收system函数返回值变量*/
	int status = 0;	

	/*存储临时信息变量*/
	char buf[BUFSIZE] = {0};
	char ret1 = 0;
	char ret2 = 0;

	/*挂载点根目录下创建log目录，log下创建文件mytest.log*/
	const char *directory = "/mnt/upoint/log"; 
	const char *file = "/mnt/upoint/log/mytest.log";

	diskmsg_fd = open_txt("./diskmsg.txt");
	sdbcnt_fd = open_txt("./sdbcnt.txt");
	dfmsg_fd = open_txt("./dfmsg.txt");
	mntcnt_fd = open_txt("./mntcnt.txt");

	status = system(diskcmd); /*获取磁盘信息*/
	stat_check(status, diskcmd);

	status = system(sdbcnt_cmd); /*检测是否有/dev/sdb设备*/
	stat_check(status, sdbcnt_cmd);

	status = system(dfcmd); /*获取已挂载的分区信息*/
	stat_check(status, dfcmd);

	status = system(mntcnt_cmd); /*获取/dev/sdb1挂载信息*/
	stat_check(status, mntcnt_cmd);
	
	ret1 = read(mntcnt_fd, buf, 1); /*获得/dev/sdb1分区已挂载次数*/
	if (ret1 == -1)
	{
		perror("read mntcnt");
		exit(-1);		
	}
	mntcnt = atoi(buf);
	if (mntcnt > 0)
	{
		printf("/dev/sdb1 device mounted num:%d，start umount...\n", mntcnt);
		while(mntcnt--)
		{
			status = system(umntcmd); /*格式化前先卸载原来的同名分区*/
			stat_check(status, umntcmd);
		}
		printf("umount success!\n");
	}

	memset(buf, 0, strlen(buf)+1);
	ret2 = read(sdbcnt_fd, buf, 1);
	if (ret2 == -1)
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
	{
		printf("make a partition for /dev/sdb, capacity is the whole \n");
		status = system(fdiskcmd); /*建立分区*/
		stat_check(status, fdiskcmd);
	}

	status = system(fmtcmd); /*格式化sdb1分区文件系统为FAT32*/
	stat_check(status, fmtcmd);

	status = system(mntcmd); /*挂载/dev/sdb1分区*/
	stat_check(status, mntcmd);

	ret1 = mkdir(directory, 0777); /*在挂载点下创建Log目录*/
	if (ret1 == -1)
	{
		perror("create directory fail");
		exit(-1);
	}
	else
	{
		/*在挂载点子目录Log下创建mytest.log文件*/
		char wrbuf[20] = "hello world";
		mytest_log_fd = open(file, O_RDWR|O_CREAT|O_TRUNC, 0666); 
		if (mytest_log_fd == -1)
		{
			perror("create mytest.log fail");
			exit(-1);
		}
		ret2 = write(mytest_log_fd, wrbuf, strlen(wrbuf));
		if (ret2 == -1)
		{
			perror("write mytest.log fail");
			exit(-1);
		}
		close(mytest_log_fd); /*卸载/dev/sdb1前，关闭挂载点中打开的文件*/

		printf("write mytest.log complete, start umount /dev/sdb1\n");
		status = system(umntcmd);
		stat_check(status, umntcmd);
	}
	
	close(diskmsg_fd);
	close(sdbcnt_fd);
	close(dfmsg_fd);
	close(mntcnt_fd);
	
	return 0;
}




