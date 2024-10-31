/*
 * @Author: jyx
 * @Date: 2024-10-25 00:43:07
 * @LastEditors: jyx
 * @Description:
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    pid_t pid;
    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    {
        // 子进程
        printf("this is exit process.\n");
        printf("exit test with buffered output");
        exit(0);
    }
    else
    {
        // 父进程
        printf("this is _exit process.\n");
        printf("_exit test with buffered output");
        _exit(0);
    }
}
