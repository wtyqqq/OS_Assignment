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

				char buf[1024];
				readline(fd,buf);
				printf("line:%s\n", buf);
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
