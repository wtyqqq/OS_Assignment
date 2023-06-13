#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include "process_data.h"
#include "time.h"
#include "sys/time.h"
#include <semaphore.h>
#define MAX_NUM 2000000

typedef struct{
    sem_t sem1;
    int count;
    long long int numbers[MAX_NUM];
}data_shared;

int main()
{   
    void *shm = NULL;
    struct timeval start, end;  
	pid_t pid, pid1, pid2, pid3;
    int shmid; 
    gettimeofday(&start, NULL); 
    shmid = shmget((key_t)1235, sizeof(data_shared), 0666|IPC_CREAT);
    if (shmid == -1)
    {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }
    shm = shmat(shmid, 0, 0);
    data_shared* shared_data_ptr = (data_shared*)shm;
    sem_init(&shared_data_ptr->sem1, 1, 0);
    pid1 = fork();
    
    if (pid1 == 0) {
        if (shm == (void *)-1)
        {
            fprintf(stderr, "shmat failed\n");
            exit(EXIT_FAILURE);
        }
        FILE* file = fopen("../testdata.txt", "r");
        int read_num;
        char *buff = NULL;
        long long int num;
        read_num = fscanf(file, "%lld", &num); 
        shared_data_ptr->count = 0;
        // printf("process 1 start!\n");
        // printf("%lld\n", num);
        while (read_num == 1 && shared_data_ptr->count < MAX_NUM)
        {
            shared_data_ptr->numbers[shared_data_ptr->count++] = num;
            read_num = fscanf(file, "%lld", &num); 
            // printf("%lld\n", num);
        }
        sem_post(&shared_data_ptr->sem1);
        exit(0);
    }

    pid2 = fork();
    if (pid2 == 0) {
        sem_wait(&shared_data_ptr->sem1);
        for (int i = 0; i < shared_data_ptr->count; i++) {
            shared_data_ptr->numbers[i] = process(shared_data_ptr->numbers[i]);
        }
        sem_post(&shared_data_ptr->sem1);
        exit(0);
    }

    pid3 = fork();
    if (pid3 == 0) {
        sem_wait(&shared_data_ptr->sem1);
        FILE* file = fopen("../shared_mem.txt", "w");
        for (int i = 0; i < shared_data_ptr->count; i++) {
            fprintf(file, "%lld\n", shared_data_ptr->numbers[i]);
        }
        fclose(file);
        exit(0);
    }

    wait(NULL);
    wait(NULL);
    wait(NULL);
    
    gettimeofday(&end, NULL); 
    long timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("time cost:%fs\n", (double)timeuse/1000000);
    return 0;
}