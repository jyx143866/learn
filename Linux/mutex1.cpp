/***
 * @Author: jyx
 * @Date: 2024-10-25 07:59:10
 * @LastEditors: jyx
 * @Description:操作共享变量会有问题的售票系统代码
 */
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

int ticket = 100;

void *sale(void *arg)
{
    char *id = (char *)arg;

    while (1)
    {
        if (ticket > 0)
        {
            usleep(1000);
            cout << id << "sells tickets : " << ticket << endl;
            ticket--;
        }
        else
        {
            break;
        }
    }
}

int main()
{
    pthread_t tid1, tid2, tid3, tid4;

    pthread_create(&tid1, NULL, sale, (void *)"thread 1");
    pthread_create(&tid2, NULL, sale, (void *)"thread 2");
    pthread_create(&tid3, NULL, sale, (void *)"thread 3");
    pthread_create(&tid4, NULL, sale, (void *)"thread 4");

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);

    return 0;
}