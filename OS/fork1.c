/*
 * @Author: jyx
 * @Date: 2024-10-25 00:11:45
 * @LastEditors: jyx
 * @Description:
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        // fork失败
        perror("fork");
        return 1;
    }
    else if (pid == 0)
    {
        for (int i = 1; i <= 5; i++)
        {
            // usleep(1000000);
            printf("子进程: PID = %d, 循环次数 = %d \n", getpid(), i);
            usleep(1000000);
        }
        _exit(0);
    }
    else
    {
        for (int i = 1; i <= 5; i++)
        {
            printf("父进程: PID = %d, 循环次数 = %d \n", getpid(), i);
            usleep(1000000);
        }
        wait(NULL);
    }

    return 0;
}
