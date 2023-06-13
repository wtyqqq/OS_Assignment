#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
int height;
int display_page(char *ptr, int offset)
// 打印一整页
{
    int index, count = 0;
    char *buf = malloc(1024); 

    ptr += offset; // 定位至指定偏移量处
    for (index = 0; index < 1024; index++)
    {
        if (ptr[index] == '\n')
            count++; // 计算行数
        if (count == height)
            break; // 显示第 7 行时停止
    }
    memcpy(buf, ptr, index); // 将内容复制到缓冲区
    buf[index] = '\0';     // 添加字符串结束符
    system("clear");
    printf("%s\n", buf);
    printf("(K/k 1 line up, j/J 1 line down, Enter one screen):");
    return index;
}


int next_row(char *ptr, int offset)
// 下翻一页，返回页尾偏移量
{
    int index, count = 0, j;
    char *buffer = malloc(1024); // 使用动态内存分配空间
    ptr += offset;                      // 定位至指定偏移量处
    for (j = 0; offset - j > 0; j--)
    {
        if (ptr[j] == '\n')
            count++; // 计算行数
        if (count == height)
            break; // 找到上一页的行末位置
    }
    for (index = 0; index < 100; index++)
        if (ptr[index] == '\n')
            break;                    // 找到下一行的行末位置
    memcpy(buffer, ptr + j + 1, index - j); // 将内容复制到缓冲区
    buffer[index - j - 1] = '\0';         // 添加字符串结束符
    system("clear");
    printf("%s\n", buffer);
    printf("(K/k 1 line up, j/J 1 line down, Enter one screen):");
    return index;
}

// 上翻一页，返回页首偏移量
int last_row(char *s, int d)
{
    int count = 0;
    char *buffer = malloc(1024);
    int i = d, j = d;

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

        if (count == height + 2)
            break; // 找到上一页的行末位置
    }
    if (count < height + 1)
    {
        memcpy(buffer, s, d); // 将内容复制到缓冲区
        buffer[d - 1] = '\0'; // 添加字符串结束符
        system("clear");
        printf("%s\n", buffer);
        printf("(K/k 1 line up, j/J 1 line down, Enter one screen):");
        return d - 1;
    }
    else
    {
        memcpy(buffer, s + j + 1, i - j); // 将内容复制到缓冲区
        buffer[i - j - 1] = '\0';         // 添加字符串结束符
        system("clear");
        printf("%s\n", buffer);
        printf("(K/k 1 line up, j/J 1 line down, Enter one screen):");
        return i;
    }
}

int main(int argc, char *argv[])
{
    int fd, now = 0;
    char choice;
    char *start;
    struct stat file_stat;
    // 获取终端高度
    struct winsize size;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
    height = size.ws_row - 1;
    // 干掉烦人的缓冲区
    struct termios original, raw;
    tcgetattr(STDIN_FILENO, &original); // 获取当前终端属性
    raw = original;
    raw.c_lflag &= ~(ICANON | ECHO);          // 禁用标准模式和回显
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw); // 设置原始模式
    if (argc != 2)
    {
        printf("usage：%s <filename>\n", argv[0]);
        exit(1);
    }
    char *filename = argv[1];

    // 打开文件
    fd = open(filename, O_RDONLY);
    // 获取文件状态
    fstat(fd, &file_stat);
    // 映射
    start = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (start == MAP_FAILED)
    {
        perror("mmap failed!!!!!!!!!");
        exit(1);
    }
    // 输出第一页内容
    now = display_page(start, now) + 1;
    choice = getchar();
    while (choice != 'q' && choice != 'Q')
    {
        if (now > file_stat.st_size) // 如果超出文件大小则退出
        {
            choice = getchar();
            break;
        }
        else if (choice == ' ' || choice == '\n') 
            now = now + 1 + display_page(start, now);
        else if (choice == 'j' || choice == 'J')
            now = now + 1 + next_row(start, now);
        else if (choice == 'k' || choice == 'K')
            now = 1 + last_row(start, now);
        choice = getchar();
    }

    // 解除内存映射并关闭文件
    munmap(start, file_stat.st_size);
    close(fd);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original); // 恢复终端属性
    return 0;
}