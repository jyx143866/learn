/***
 * @Author: jyx
 * @Date: 2024-10-24 13:47:07
 * @LastEditors: jyx
 * @Description:线程退出函数和线程等待
 */
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

using namespace std;

void *thread1(void *arg)
{
    cout << "thread 1 returning ..." << endl;
    int *p = new int;
    *p = 1;
    return (void *)p;
}

void *thread2(void *arg)
{
    cout << "thread 2 exiting ..." << endl;
    int *p = new int;
    *p = 2;
    pthread_exit((void *)p);
}

void *thread3(void *arg)
{
    while (1)
    {
        cout << "thread 3 running ..." << endl;
        sleep(2);
    }
}

int main()
{
    pthread_t tid;
    void *ret;

    pthread_create(&tid, NULL, thread1, NULL);
    pthread_join(tid, &ret);

    cout << "thread return, thread id is " << tid << "return code : " << *(int *)ret << endl;
    free(ret);

    pthread_create(&tid, NULL, thread2, NULL);
    pthread_join(tid, &ret);

    cout << "thread return, thread id is " << tid << "return code : " << *(int *)ret << endl;
    free(ret);

    pthread_create(&tid, NULL, thread3, NULL);
    sleep(3);
    pthread_cancel(tid);
    pthread_join(tid, &ret);
    if (ret == PTHREAD_CANCELED)
    {
        cout << "thread return, thread id is " << tid << "return code : PTHREAD_CANCELED" << endl;
    }
    else
    {
        cout << "thread return, thread id is " << tid << "return code : NULL" << endl;
    }
    return 0;
}