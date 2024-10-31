/*
 * @Author: jyx
 * @Date: 2024-10-25 00:57:34
 * @LastEditors: jyx
 * @Description:
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void count(const char *filename)
{
    int fd = open(filename, O_RDONLY);

    if (fd < 0)
    {
        perror("打开文件失败......");
        exit(1);
    }

    int count = lseek(fd, 0, SEEK_END);
    close(fd);

    printf("文件名：%s, %d\n", filename, count);
}

int main(int argc, char *argv[])
{

    for (int i = 1; i < argc; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("Fork failed");
            exit(1);
        }
        else if (pid == 0)
        {
            count(argv[i]);
            exit(0);
        }
    }

    for (int i = 1; i < argc; i++)
    {
        wait(NULL);
    }

    return 0;
}
