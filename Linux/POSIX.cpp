/***
 * @Author: jyx
 * @Date: 2024-10-26 02:47:34
 * @LastEditors: jyx
 * @Description:基于环形队列的生产者消费者模型
 */
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <vector>
#include <pthread.h>

using namespace std;

#define NUM 16

class RingQueue
{
public:
    RingQueue(int _cap = NUM)
        : q(_cap), cap(_cap)
    {
        sem_init(&data_sem, 0, 0);
        sem_init(&space_sem, 0, cap);
        consume_step = 0;
        product_step = 0;
    }

    ~RingQueue()
    {
        sem_destroy(&data_sem);
        sem_destroy(&space_sem);
    }

    void PutData(const int &data)
    {
        sem_wait(&space_sem);
        q[consume_step++] = data;
        consume_step %= cap;
        sem_post(&data_sem);
    }

    void GetData(int &data)
    {
        sem_wait(&data_sem);
        data = q[product_step++];
        product_step %= cap;
        sem_post(&space_sem);
    }

private:
    vector<int> q;
    int cap;
    sem_t data_sem;
    sem_t space_sem;
    int consume_step;
    int product_step;
};

void *Consume(void *arg)
{
    RingQueue *q = (RingQueue *)arg;
    int data;
    while (1)
    {
        q->GetData(data);
        cout << "Consume data done : " << data << endl;
        sleep(1);
    }
}

void *Product(void *arg)
{
    RingQueue *q = (RingQueue *)arg;
    srand((unsigned long)time(NULL));
    while (1)
    {
        int data = rand() % 1024;
        q->PutData(data);
        cout << "Product data done : " << data << endl;
        sleep(1);
    }
}

int main()
{
    RingQueue q;
    pthread_t c, p;

    pthread_create(&c, NULL, Consume, (void *)&q);
    pthread_create(&p, NULL, Product, (void *)&q);

    pthread_join(c, NULL);
    pthread_join(p, NULL);

    return 0;
}