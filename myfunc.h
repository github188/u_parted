#ifndef _MYFUNC_H
#define _MYFUNC_H 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include<sys/wait.h>
#include <fcntl.h>

#define BUFSIZE 2

/*定义system将要使用的指令字串*/
const char *diskcmd = "fdisk -l > diskmsg.txt"; 
const char *sdbcnt_cmd = "grep -c  /dev/sdb diskmsg.txt > sdbcnt.txt"; 
const char *dfcmd = "df > dfmsg.txt";	
const char *mntcnt_cmd = "grep -c  /dev/sdb1 dfmsg.txt > mntcnt.txt";	
const char *fdiskcmd = "./fdisk.sh | sudo fdisk /dev/sdb"; 
const char *umntcmd = "umount /dev/sdb1";	
const char *fmtcmd = "mkfs.vfat /dev/sdb1"; 
const char *mntcmd = "mount -t vfat /dev/sdb1 /mnt/upoint"; 

/*声明一个system执行返回检测函数*/
void stat_check(int status, const char *cmdstring);

/*声明一个日志函数*/
void Log(const char* format, ... ); 

/*声明一个打开文本文件函数*/
int open_txt(const char* txtstring);

#endif	/* _MYFUNC_H */


