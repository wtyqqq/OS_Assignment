#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include "utils.h"
#include "process_data.h"
int main()
{
	pid_t pid;
	int pipe_fd[2];
	int pipe2_fd[2];
	if (pipe(pipe_fd) == -1)
	{
		printf("pipe created error!\n");
	}
	if (pipe(pipe2_fd) == -1)
	{
		printf("pipe2 created error!\n");
	}
	pid = fork();
	if (pid != -1)
	{
		if (pid == 0) // 进程1：读取文件，写入管道
		{
			// 读取testdata文件
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
				read_num = readline(fd, &buff); // 针对大文件，使用自己定义的readline函数一行一行的读。
				long long int num = 0;
				while (read_num > 0)
				{
					num = atoi(buff);
					write(pipe_fd[1], &num, sizeof(long long int));
					read_num = readline(fd, &buff); // 针对大文件，使用自己定义的
				}
				num = -1;
				write(pipe_fd[1], &num, sizeof(long long int));
			}
			if (close(fd) == -1)
			{
				printf("close file error!\n");
			}
			printf("process 1 finished!\n");
			exit(0);
		}
		else
		{
			pid_t pid2;
			if ((pid2 = fork()) == -1)
			{
				printf("fork error!\n");
			}
			else
			{
				if (pid2 == 0)
				{
					long long int read_num; // 进程2：读取管道，处理数据，写入管道
					long long int processed_num;
					while (read(pipe_fd[0], &read_num, sizeof(long long int)))
					{
						if (read_num == -1)
						{
							break;
						}
						printf("receive_num(p2):%lld ", read_num);
						processed_num = process(read_num);
						printf("process_num:%lld\n", processed_num);
						write(pipe2_fd[1], &processed_num, sizeof(long long int));
					}
					processed_num = -1;
					write(pipe2_fd[1], &processed_num, sizeof(long long int));
					printf("process 2 finished!\n");
					exit(0);
				}
				else
				{ // 进程3：读取管道，写入文件
					long long int read_num2;
					while (read(pipe2_fd[0], &read_num2, sizeof(long long int)))
					{
						if (read_num2 == -1)
						{
							break;
						}
						printf("receive_num(p3):%lld\n", read_num2);
					}
				}
			}
		}
	}
	else
	{
		printf("fork error!\n");
	}
}
