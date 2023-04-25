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
			fd = open("1.txt", O_RDONLY);
			if (fd == -1)
			{
				printf("open file error!\n");
			}
			else
			{	
				int read_num;
				char *buff = NULL;
				read_num = readline(fd,&buff); // 针对大文件，使用自己定义的readline函数一行一行的读。
				while (read_num>0)
				{
					printf("line:%s\n", buff);
					printf("read_num:%d\n", read_num);
					read_num = readline(fd,&buff); // 针对大文件，使用自己定义的
				}
				
				
				// lseek(fd,read_num+1,SEEK_CUR);
				
				
				
				
				// read_num = readline(fd,&buff); // 针对大文件，使用自己定义的readline函数一行一行的读。
				// printf("line:%s\n", buff);
				// printf("read_num:%d\n", read_num);
				
				// read_num = readline(fd,&buff); // 针对大文件，使用自己定义的readline函数一行一行的读。
				// if(*buff=='\0')
				// {
				// 	printf("buff is null\n");
				// }
			}
			if (close(fd) == -1)
			{
				printf("close file error!\n");
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
