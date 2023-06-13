#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "process_data.h"
#include "time.h"
#include "sys/time.h"

int main()
{
	struct timeval start,end;  
	pid_t pid, pid1, pid2, pid3;
	int pipe_fd[2];
	int pipe2_fd[2];
	gettimeofday(&start, NULL); 
	if (pipe(pipe_fd) == -1)
	{
		printf("pipe created error!\n");
	}
	if (pipe(pipe2_fd) == -1)
	{
		printf("pipe2 created error!\n");
	}
	if ((pid1 = fork()) != -1)
	{
		if (pid1 == 0) // 进程1：读取文件，写入管道
		{
			// 读取testdata文件
			FILE* file = fopen("../testdata.txt", "r");
			int read_num;
			char *buff = NULL;
			long long int num;
			read_num = fscanf(file, "%lld", &num); 
			while (read_num > 0)
			{
				write(pipe_fd[1], &num, sizeof(long long int));
				read_num = fscanf(file, "%lld", &num); 
			}
			num = -1;
			write(pipe_fd[1], &num, sizeof(long long int));
			if (fclose(file) == -1)
			{
				printf("close file error!\n");
			}
			printf("process 1 finished!\n");
			exit(0);
		}
	}
	if ((pid2 = fork()) != -1)
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
				// printf("receive_num(p2):%lld ", read_num);
				processed_num = process(read_num);
				// printf("process_num:%lld\n", processed_num);
				write(pipe2_fd[1], &processed_num, sizeof(long long int));
			}
			processed_num = -1;
			write(pipe2_fd[1], &processed_num, sizeof(long long int));
			printf("process 2 finished!\n");
			exit(0);
		}
	}
	else
	{
		printf("fork error!\n");
	}

	if ((pid3 = fork()) != -1)
	{
		if (pid3 == 0)
		{
			// 进程3：读取管道，写入文件
			long long int read_num2;
			FILE* fd2;
			fd2 = fopen("../2.txt", "w"); // 以只写的方式打开文件，如果文件不存在则创建，如果文件存在则清空
			while (read(pipe2_fd[0], &read_num2, sizeof(long long int)))
			{
				if (read_num2 == -1)
				{
					break;
				}
				// printf("receive_num(p3):%lld\n", read_num2);
				// write_line(fd2, read_num2);
				fprintf(fd2, "%lld\n", read_num2);
			}
			if (fclose(fd2) == -1)
			{
				printf("close file error!\n");
			}
			printf("process 3 finished!\n");
			exit(0);
		}
	}
	else
	{
		printf("fork error!\n");
	}

    wait(NULL);
    wait(NULL);
    wait(NULL);

	gettimeofday(&end, NULL); 
	long timeuse =1000000*( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec; 
	printf("time cost:%fs\n", (double)timeuse/1000000);
	return 0;
	}
