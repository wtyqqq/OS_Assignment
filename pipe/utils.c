#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
int read_line(int fd,char** buff)
{
        // 针对大文件，使用自己定义的read_line函数一行一行的读。
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
                
                if(c=='\n' || c==EOF)
                {
                        break;
                }else{
                    (*buff)[count++]=c;
                }
 
        }
        (*buff)[count]='\0';
        return count;
}