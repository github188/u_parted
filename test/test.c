#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void mytime(void)  
{  
    time_t now;  
    time(&now);  
    struct tm *local;  
    local = localtime(&now);  

    printf("%04d-%02d-%02d %02d:%02d:%02d\n", local->tm_year+1900, local->tm_mon, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);

    return ;  
} 

int main(void)
{
	int i = 0;

	mytime();
	for (i=0; i<20000; i++)
	{
		system("ls -l > /dev/null 2>&1");
		//popen("ls -l > /dev/null 2>&1", "r");
	}
	mytime();
}


