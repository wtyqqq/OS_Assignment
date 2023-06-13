#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h> 
#include <termios.h>

// 输出一页内容，返回页尾偏移量
int one_page(char *s, int d)
{
    int i, count = 0;
    char *buffer = malloc(1024); // 使用动态内存分配空间

    s += d; // 定位至指定偏移量处
    for (i = 0; i < 1024; i++)
    {
        if (s[i] == '\n')
            count++; // 计算行数
        if (count == 10)
            break; // 显示第 7 行时停止
    }
    memcpy(buffer, s, i); // 将内容复制到缓冲区
    buffer[i] = '\0'; // 添加字符串结束符
    system("clear");
    printf("%s\n", buffer);
    return i;
}

// 下翻一页，返回页尾偏移量
int next_row(char *s, int d)
{
    int i,count = 0,j;
    char *buffer = malloc(1024); // 使用动态内存分配空间
    s += d; // 定位至指定偏移量处
    for (j = 0; d - j > 0; j--)
    {
        if (s[j] == '\n')
            count++; // 计算行数
        if (count == 10)
            break; // 找到上一页的行末位置
    }
    for (i = 0; i < 100; i++)
        if (s[i] == '\n')
            break; // 找到下一行的行末位置
    memcpy(buffer, s + j + 1, i - j); // 将内容复制到缓冲区
    buffer[i - j - 1] = '\0'; // 添加字符串结束符
    system("clear");
    printf("%s\n", buffer);
    return i;
}

// 上翻一页，返回页首偏移量
int last_row(char *s, int d)
{
    int count = 0;
    char *buffer = malloc(1024);
    int i = d,j = d;

    for (; i > 0; i--)
    {
        if (s[i] == '\n')
            count++; // 计算行数
        if (count == 2)
            break; // 找到上一页的行末位置
    }
    
    count = 0;    
    for (; j > -1; j--)
    {
        if (s[j] == '\n')
            count++; // 计算行数
        
        if (count == 12)
            break; // 找到上一页的行末位置
    }
    if (count < 11)
    {
        memcpy(buffer, s , d); // 将内容复制到缓冲区
    	buffer[d - 1] = '\0'; // 添加字符串结束符    
    	system("clear");
    	printf("%s\n", buffer);	
    	return d - 1;
    }
    else
    {
        memcpy(buffer, s + j + 1, i - j); // 将内容复制到缓冲区
    	buffer[i - j - 1] = '\0'; // 添加字符串结束符
    	system("clear");
        printf("%s\n", buffer);
        return i;
    }


}

int main()
{
    int fd, play = 0;
    char lab;
    char *start;
    struct stat sb;
    struct termios original, raw;
    tcgetattr(STDIN_FILENO, &original);  // 获取当前终端属性
    raw = original;
    raw.c_lflag &= ~(ICANON | ECHO);     // 禁用标准模式和回显
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);  // 设置原始模式
    // 打开文件并将其映射至内存
    fd = open("../2.txt", O_RDONLY);
    fstat(fd, &sb);
    start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (start == MAP_FAILED)
        return (1);

    // 输出第一页内容
    play = one_page(start, play) + 1;
    lab = getchar();
    while (lab != 'q' && lab != 'Q')
    {
        if (play > sb.st_size) // 如果超出文件大小则退出
        {
            lab = getchar();
            break;
        }
        else if (lab == ' ' ) // 空格键下翻一页
            play = play + 1 + one_page(start, play);
        else if (lab == 'D' || lab == 'd') // D 或 d 键下翻一行
            play = play + 1 + next_row(start, play);
        else if (lab == 'U' || lab == 'u') // U 或 u 键上翻一页
            play = 1 + last_row(start, play);
        lab = getchar();
    }

    // 解除内存映射并关闭文件
    munmap(start, sb.st_size);
    close(fd);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original);  // 恢复终端属性
    return 0;
}