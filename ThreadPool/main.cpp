/***
 * @Author: jyx
 * @Date: 2024-10-28 08:11:08
 * @LastEditors: jyx
 * @Description:
 */
/***
 * @Author: jyx
 * @Date: 2024-10-28 08:11:08
 * @LastEditors: jyx
 * @Description:
 */

#include "ThreadPool.hpp"
#include <iostream>
#include <chrono>
#include <thread>

/*
有些场景，是希望能够获取线程执行任务的返回值的
举例：
1 + ...... + 30000 的和
thread1 1 + ...... + 10000
thread2 10001 + ...... + 20000
......
main thread：给每一个线程分配计算的区间，并等待它们算完返回结果，合并最终的结果
*/

using uLong = unsigned long long;

class MyTask : public Task
{
public:
    MyTask(int begin, int end)
        : begin_(begin), end_(end)
    {
    }

    // 问题：怎么设置run函数的返回值，可以表示任意类型
    // Java Python Object类是所有其他类类型的基类
    // C++17中的Any类
    Any run()
    {

        std::cout << "Thread ID: " << std::this_thread::get_id() << " is starting work." << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(4)); // 模拟长时间工作
        uLong sum = 0;
        for (uLong i = begin_; i <= end_; i++)
        {
            sum += i;
        }

        std::cout << "Thread ID: " << std::this_thread::get_id() << " has completed work." << std::endl;
        return sum;
    }

private:
    int begin_;
    int end_;
};

int main()
{
    {
    
        ThreadPool pool;

        // 用户自己设置线程池的工作模式
        pool.setMode(PoolMode::MODE_CACHED);

        // 开始启动线程池
        pool.start(4);

        // 提交任务
        Result res1 = pool.submitTask(std::make_shared<MyTask>(1, 100000000));
        Result res2 = pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
        Result res3 = pool.submitTask(std::make_shared<MyTask>(200000001, 300000000));
        pool.submitTask(std::make_shared<MyTask>(1, 100000000));
        pool.submitTask(std::make_shared<MyTask>(1, 100000000));
        pool.submitTask(std::make_shared<MyTask>(1, 100000000));

        uLong sum1 = res1.get().cast_<uLong>();
        uLong sum2 = res2.get().cast_<uLong>();
        uLong sum3 = res3.get().cast_<uLong>();

        std::cout << "结果: " << sum1 + sum2 + sum3 << std::endl;
    }                                                                                                                              
    return 0;
}
