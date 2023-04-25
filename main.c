#include<stdio.h>
#include <unistd.h>
#include<sys/types.h>

int main(){
	pid_t  pid;
	pid = fork();
	if(pid!=-1)
	{
		if(pid == 0){
			printf("this is child!\n");
		}
		else{
			printf("this is parent!\n");
		}
		
	}
	else{
		printf("fork error!\n");
		
	}
	
	
}
