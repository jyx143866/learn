/***
 * @Author: jyx
 * @Date: 2024-10-24 14:08:05
 * @LastEditors: jyx
 * @Description:线程分离pthread_detach()
 */
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

using namespace std;

void *thread_run(void *arg)
{
    pthread_detach(pthread_self());
    cout << (char *)arg << endl;
    return NULL;
}

int main()
{
    pthread_t tid;
    if (pthread_create(&tid, NULL, thread_run, (void *)"thread1 run ...") != 0)
    {
        cout << "create error!" << endl;
        return 1;
    }
    int ret = 0;
    sleep(1); // 很重要，要让线程先分离，再等待

    if (pthread_join(tid, NULL) == 0)
    {
        printf("pthread wait success\n");
        ret = 0;
    }
    else
    {
        printf("pthread wait failed\n");
        ret = 1;
    }
    return ret;
}