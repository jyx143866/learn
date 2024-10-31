/***
 * @Author: jyx
 * @Date: 2024-10-26 01:57:48
 * @LastEditors: jyx
 * @Description:生产者消费者模型
 */
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <queue>
#include <pthread.h>

#define NUM 8

class BlockQueue
{
public:
    BlockQueue(int _cap = NUM)
        : cap(_cap)
    {
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&full, NULL);
        pthread_cond_init(&empty, NULL);
    }

    ~BlockQueue()
    {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&full);
        pthread_cond_destroy(&empty);
    }

    void PushData(const int &data)
    {
        pthread_mutex_lock(&mutex);
        while (IsFull())
        {
            pthread_cond_signal(&empty);
            std::cout << "queue is full, product stop !" << std::endl;
            pthread_cond_wait(&full, &mutex);
        }
        q.push(data);
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }

    void PopData(int &data)
    {
        pthread_mutex_lock(&mutex);
        while (IsEmpty())
        {
            pthread_cond_signal(&full);
            std::cout << "queue is empty, consume stop !" << std::endl;
            pthread_cond_wait(&empty, &mutex);
        }
        data = q.front();
        q.pop();
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);
    }

    bool IsFull()
    {
        return q.size() == cap ? true : false;
    }

    bool IsEmpty()
    {
        return q.size() == 0 ? true : false;
    }

private:
    std::queue<int> q;
    int cap;
    pthread_mutex_t mutex;
    pthread_cond_t full;
    pthread_cond_t empty;
};

void *Product(void *arg)
{
    BlockQueue *q = (BlockQueue *)arg;
    srand((unsigned long)time(NULL));
    while (1)
    {
        int data = rand() % 1024;
        q->PushData(data);
        std::cout << "product data done : " << data << std::endl;
        sleep(1);
    }
}

void *Consumer(void *arg)
{
    BlockQueue *q = (BlockQueue *)arg;
    int data;
    while (1)
    {
        q->PopData(data);
        std::cout << "Consume data done : " << data << std::endl;
    }
}

int main()
{
    BlockQueue q;
    pthread_t c, p;

    pthread_create(&c, NULL, Consumer, (void *)&q);
    pthread_create(&p, NULL, Product, (void *)&q);

    pthread_join(c, NULL);
    pthread_join(p, NULL);

    return 0;
}