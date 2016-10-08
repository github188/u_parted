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

/*定义一个日志函数*/
void Log(const char* format, ... )  
{  
	FILE* file = NULL;
    char wzLog[1024] = {0};  
    char buffer[1024] = {0};  
    va_list args;  
    va_start(args, format);  
    vsprintf(wzLog, format, args);  
    va_end(args);  
  
    time_t now;  
    time(&now);  
    struct tm *local;  
    local = localtime(&now);  

    /*printf("%04d-%02d-%02d %02d:%02d:%02d %s\n", local->tm_year+1900, local->tm_mon, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec,  wzLog);*/

    sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d %s\n", local->tm_year+1900, local->tm_mon, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec, wzLog);  

    file = fopen("./Log/mytest.log","a+");  
	if (file == NULL)
	{
		perror("open mytest.log fail!");
		exit(-1);
	}
    fwrite(buffer, 1, strlen(buffer), file);  
    fclose(file);  
 
    return ;  
}  

/*定义system执行返回检测函数*/
void stat_check(int status, const char *cmdstring)
{
	if (cmdstring == NULL)
	{
		Log("cmd is NULL", cmdstring);
		exit(-1);		
	}

	if (status < 0)
	{
		Log("cmd: %s\t error: %s", cmdstring, strerror(errno));
		exit(-1);
	}

	if (WIFEXITED(status))
	{
		/*取得cmdstring正常执行结果*/
		Log("cmd: %s\t Normal termination, exit status = %d", 
                                               cmdstring,  WEXITSTATUS(status)); 
	}
	else if (WIFSIGNALED(status))
	{
		/*如果cdmstring被信号中止，取得信号的值*/
		Log("cmd: %s\t Abnormal termination, signal number = %d", 
                                               cmdstring, WTERMSIG(status)); 
		exit(-1);
	}
	else if (WIFSTOPPED(status))
	{
		/*如果cdmstring被信号暂停，取得信号的值*/
		Log("cmd: %s\t Process stopped, signal number = %d",
                                               cmdstring, WSTOPSIG(status)); 
		exit(-1);
	}

	return ;
}

int open_txt(const char* txtstring)
{
	int tmp = -2;

	tmp = open(txtstring, O_RDWR|O_CREAT|O_TRUNC, 0666);
	if (tmp == -1)
	{
		perror(txtstring);
		exit(-1);
	}

	return tmp;
}



