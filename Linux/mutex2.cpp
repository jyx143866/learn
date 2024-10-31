/***
 * @Author: jyx
 * @Date: 2024-10-26 01:23:23
 * @LastEditors: jyx
 * @Description:
 */
/***
 * @Author: jyx
 * @Date: 2024-10-26 01:23:23
 * @LastEditors: jyx
 * @Description: 改进后加锁的售票系统
 */
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

using namespace std;

int ticket = 100;
pthread_mutex_t lock;

void *sale(void *arg)
{

    while (1)
    {
        pthread_mutex_lock(&lock);

        if (ticket > 0)
        {

            cout << (char *)arg << "sell" << ticket-- << endl;
            pthread_mutex_unlock(&lock);
        }
        else
        {
            pthread_mutex_unlock(&lock);
            break;
        }
    }
}

int main()
{
    pthread_mutex_init(&lock, NULL);

    pthread_t t1, t2, t3, t4;

    pthread_create(&t1, NULL, sale, (void *)"thread1");
    pthread_create(&t2, NULL, sale, (void *)"thread2");
    pthread_create(&t3, NULL, sale, (void *)"thread3");
    pthread_create(&t4, NULL, sale, (void *)"thread4");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}