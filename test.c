#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

void LOG(const char* format, ... )  
{  
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
    printf("%04d-%02d-%02d %02d:%02d:%02d %s\n", local->tm_year+1900, local->tm_mon,  
            local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec,  
            wzLog);  
    sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d %s\n", local->tm_year+1900, local->tm_mon,  
                local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec,  
                wzLog);  
    FILE* file = fopen("./Log/mytest.log","a+");  
    fwrite(buffer,1,strlen(buffer),file);  
    fclose(file);  
 
    return ;  
}  

int main(void)
{
	LOG("hello===========%d\n", 888);
}

