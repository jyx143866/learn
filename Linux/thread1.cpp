/***
 * @Author: jyx
 * @Date: 2024-10-24 07:22:00
 * @LastEditors: jyx
 * @Description:线程
 */
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

using namespace std;

void *printf(void *arg)
{
    std::cout << "child thread : " << getpid() << endl;
}

int main()
{
    pthread_t pid;
    int ret = pthread_create(&pid, NULL, printf, NULL);
    if (ret != 0)
    {
        std::cout << "create failed!" << endl;
    }
    while (1)
    {
        std::cout << "I am main thread!" << endl;
        sleep(10);
        }
    return 0;
}
