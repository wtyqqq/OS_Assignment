#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
int readline(int fd,char** buff)
{
        int rl=-1;
        char c;
        long maxlength=128;
        long count=0;
        if(fd<0)
        {
                printf("open file error\n");
                exit(0);
        }
        *buff=(char*)malloc(sizeof(char)*maxlength);
        while((rl=read(fd,&c,1))>0)
        {
                if(count==maxlength)
                {
                        maxlength+=128;
                        *buff=(char*)realloc(*buff,maxlength);
                        if(*buff==NULL)
                        {
                                printf("allocate memory error\n");
                                close(fd);
                                exit(0);
                        }
                }
                (*buff)[count++]=c;
                 if(c=='\n' || c==EOF)
                {
                        break;
                }
 
        }
        (*buff)[count]='\0';
        return count;
}