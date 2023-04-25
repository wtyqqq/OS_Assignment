#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "utils.h"
int main()
{
	pid_t pid;
	pid = fork();
	if (pid != -1)
	{
		if (pid == 0)
		{
			printf("this is child!\n");
			//读取testdata文件
			int fd;
			fd = open("testdata.txt", O_RDONLY);
			if (fd == -1)
			{
				printf("open file error!\n");
			}
			else
			{	
				char *buff = NULL;
				readline(fd,&buff); // 针对大文件，使用自己定义的readline函数一行一行的读。
				printf("line:%s", buff);
			}
		}
		else
		{
			printf("this is parent!\n");
		}
	}
	else
	{
		printf("fork error!\n");
	}
}
