/*
 * @Author: jyx
 * @Date: 2024-10-23 14:07:18
 * @LastEditors: jyx
 * @Description:从键盘读取数据，写入管道，读取管道，写到屏幕
 */
/*#include <unistd.h>
功能 : 创建一无名管道
           原型 int pipe(int fd[2]);
参数
fd：文件描述符数组, 其中fd[0] 表示读端,fd[1] 表示写端
 返回值 : 成功返回0，失败返回错误代码*/

/*ssize_t write(int fd, const void *buf, size_t count);
用于将数据写入文件描述符所代表的文件或设备中
fd：代表要写入的目标；
buf：写入的内容：
count：写入的字节数。*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    int fds[2];
    char buf[100];
    int len;

    if (pipe(fds) == -1)
    {
        perror("make pipe");
        exit(1);
    }
    while (fgets(buf, 100, stdin))
    {
        len = strlen(buf);
        if (write(fds[1], buf, len) != len)
        {
            perror("write to pipe");
            exit(1);
            break;
        }
        memset(buf, 0x00, sizeof(buf));
        if ((len = read(fds[0], buf, 100)) == -1)
        {
            perror("read from pipe");
            break;
        }
        // write to stdout
        if (write(1, buf, len) != len)
        {
            perror("write to stdout");
            break;
        }
        return 0;
    }
}