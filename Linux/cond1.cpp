/***
 * @Author: jyx
 * @Date: 2024-10-26 01:41:12
 * @LastEditors: jyx
 * @Description:条件变量
 */
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

pthread_mutex_t lock;
pthread_cond_t cond;

void *route1(void *arg)
{
    while (1)
    {
        pthread_cond_wait(&cond, &lock);
        std::cout << "running ..." << std::endl;
    }
}

void *route2(void *arg)
{
    while (1)
    {
        std::cout << "thread2 start " << std::endl;
        pthread_cond_signal(&cond);
        sleep(3);
        std::cout << "thread2 wake up thread1 " << std::endl;
        }
}

int main()
{
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_t t1, t2;

    pthread_create(&t1, NULL, route1, NULL);
    pthread_create(&t2, NULL, route2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
