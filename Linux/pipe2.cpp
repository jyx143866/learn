/***
 * @Author: jyx
 * @Date: 2024-10-23 14:47:21
 * @LastEditors: jyx
 * @Description:管道和进程配合的使用
 */
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
using namespace std;
int main()
{
    int fd[2];
    int ret = pipe(fd);
    if (ret == -1)
    {
        std::cout << "管道创建失败！" << endl;
    }
    cout << "fd[0]:" << fd[0] << endl
         << "fd[1]:" << fd[1] << endl;
    pid_t pid = fork();
    if (pid < 0)
    {
        cout << "子进程创建失败！" << endl;
        exit(1);
    }
    // 子进程来读
    else if (pid == 0)
    {
        close(fd[1]);
        char buf[1024];
        int len = strlen(buf);
        read(fd[0], buf, sizeof(buf));
        cout << "子进程读：" << buf << endl;
    }
    // 父进程来写
    else if (pid > 0)
    {
        close(fd[0]);
        char str[1024];
        fgets(str, 1024, stdin);
        write(fd[1], str, strlen(str));
    }

    return 0;
}