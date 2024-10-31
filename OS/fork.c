/*
 * @Author: jyx
 * @Date: 2024-10-23 14:06:18
 * @LastEditors: jyx
 * @Description:fork函数的认识
 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int ret = fork();
    if (ret < 0)
    {
        perror("fork");
    }
    else if (ret == 0)
    {
        printf("我是子进程：%d！ret = %d\n", getpid(), ret);
    }
    else
    {
        printf("我是父进程：%d! ret = %d\n", getpid(), ret);
    }
    sleep(2);
    return 0;
}