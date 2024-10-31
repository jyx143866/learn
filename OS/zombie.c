/*
 * @Author: jyx
 * @Date: 2024-10-23 14:04:03
 * @LastEditors: jyx
 * @Description:僵尸进程的生成和查看
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
int main()
{
    pid_t id = fork();
    if (id < 0)
    {
        perror("fork");
    }
    else if (id > 0)
    { // parent
        printf("parent[%d] is sleeping...\n", getpid());
        sleep(30);
    }
    else
    {
        printf("child[%d] is begin Z...\n", getpid());
        sleep(5);
        exit(EXIT_SUCCESS);
    }
    return 0;
}