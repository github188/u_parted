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
#include <sys/wait.h>
#include <fcntl.h>

#define BUFSIZE 8
#define CMDSIZE 128

/*定义system将要使用的指令字串*/
//const char *mntcnt_cmd = "grep -c  /dev/sdb1 /proc/mounts > mntcnt.txt";	

/*用parted中的rm命令删除分区，当磁盘卸载了，但有时出现删除不成功:提示设备挂载中*/
//const char *delpart_cmd = "parted /dev/sdc rm 1"; 
/*最终采用fdisk来删除分区*/
//const char *delpart_cmd = "(echo d && echo w) | fdisk /dev/sdb";

//const char *fdiskcmd = "echo Ignore | parted /dev/sdb mkpart primary 0 31GB";
//const char *umntcmd = "umount /dev/sdb1";	

/*可在fdiskcmd中一起加入格式化命令，但挂载出现无法加载超级块，此处单独格式化*/
//const char *fmtcmd = "mkfs.vfat /dev/sdb1"; 

/*system执行返回检测函数*/
void stat_check(int status, const char *cmdstring);

/*日志函数*/
void Log(const char* format, ... ); 

/*打开文本文件函数*/
int open_txt(const char* txtstring);

/*创建目录函数*/
int mk_dir(const char *dir);

#endif	/* _MYFUNC_H */


