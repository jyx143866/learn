/***
 * @Author: jyx
 * @Date: 2024-10-26 07:02:15
 * @LastEditors: jyx
 * @Description:线程池
 */
#include "ThreadPool.hpp"
#include <functional>
#include <thread>
#include <iostream>

const int TASK_MAX_THREASHHOLD = INT32_MAX;
const int THREAD_MAX_THRESHHOLD = 100;
const int THREAD_MAX_IDLE_TIME = 60; // 单位：秒

// 线程池构造
ThreadPool::ThreadPool()
    : initThreadsSize_(0), taskSize_(0), idleThreadSize_(0), curThreadSize_(0), taskQueMaxThreshHold_(TASK_MAX_THREASHHOLD), poolMode_(PoolMode::MODE_FIXED), isPoolRunning_(false), threadSizeThreshHold_(THREAD_MAX_THRESHHOLD)

{
}

// 线程池析构
ThreadPool::~ThreadPool()
{
    // 资源回收
    isPoolRunning_ = false;
    notEmpty_.notify_all();
    // 等待线程池里面的线程结束任务 有两种状态：1阻塞   2运行
    std::unique_lock<std::mutex> lock(taskQueMtx_);
    exitcond_.wait(lock, [&]() -> bool
                   { return threads_.size() == 0; });
}

// 检查pool的运行状态
bool ThreadPool::checkRunningState() const
{
    return isPoolRunning_;
}

// 设置线程池工作模式
void ThreadPool::setMode(PoolMode mode)
{
    if (checkRunningState())
    {
        return;
    }
    poolMode_ = mode;
}

// 设置线程池catched模式下的线程的数量上限
void ThreadPool::setThreadSizeThreshHold(int threshhold)
{
    if (checkRunningState())
    {
        return;
    }
    if (poolMode_ == PoolMode::MODE_CACHED)
    {
        threadSizeThreshHold_ = threshhold;
    }
}

// 设置task任务队列上限的阈值
void ThreadPool::setTaskQueMaxThreshHold(int threshhold)
{
    if (checkRunningState())
    {
        return;
    }
    taskQueMaxThreshHold_ = threshhold;
}

// 给线程池提交任务 用户调用该接口，传入任务对象，生产任务
Result ThreadPool::submitTask(std::shared_ptr<Task> sp)
{
    // 获取锁
    std::unique_lock<std::mutex> lock(taskQueMtx_);
    // 线程的通信  等待任务队列有空余  wait一直等    wait_for等待多久    wait_until等到某个时刻
    // 用户提交任务，最长不能阻塞超过1s，否则判断提交任务失败，返回
    if (!notFull_.wait_for(lock, std::chrono::seconds(1), [&]() -> bool
                           { return taskQue_.size() < (size_t)taskQueMaxThreshHold_; }))
    {
        // 表示notFull_等待1s钟，条件依然没有满足
        std::cerr << "task queue is full, submit task fail." << std::endl;

        return Result(sp, false);
    }

    // 如果有空余，把任务放入任务队列中
    taskQue_.emplace(sp);
    taskSize_++;

    // 因为新放了任务，任务队列不空了，在notEmpty_上通知
    notEmpty_.notify_all();
    // catched模式需要根据任务数量和空闲线程的数量，判断是否需要创建新的线程出来？
    if (poolMode_ == PoolMode::MODE_CACHED && taskSize_ > idleThreadSize_ && curThreadSize_ < threadSizeThreshHold_)
    {
        // 创建新线程
        std::cout << "Creating new thread due to task load." << std::endl;
        auto ptr = std::make_unique<Thread>(std::bind(&ThreadPool::threadFunc, this, std::placeholders::_1));
        int threadId = ptr->getId();
        threads_.emplace(threadId, std::move(ptr));
        threads_[threadId]->start();
        idleThreadSize_++;
        curThreadSize_++;
    }
    // 返回任务的Result对象
    return Result(sp);
}

// 开启线程池
void ThreadPool::start(int initThreadSize)
{
    // 设置线程池的运行状态
    isPoolRunning_ = true;
    // 记录初始线程个数
    initThreadsSize_ = initThreadSize;
    curThreadSize_ = initThreadSize;

    // 创建线程对象
    for (int i = 0; i < initThreadsSize_; i++)
    {
        auto ptr = std::make_unique<Thread>(std::bind(&ThreadPool::threadFunc, this, std::placeholders::_1));
        // threads_.emplace_back(std::move(ptr));
        int threadId = ptr->getId();
        threads_.emplace(threadId, std::move(ptr));
    }
    // 启动所有线程     std::vector<Thread *> Threads_
    for (int i = 0; i < initThreadsSize_; i++)
    {
        // 创建thread线程对象的时候， 把线程函数给到thread线程对象
        threads_[i]->start(); // 需要执行一个线程函数
        idleThreadSize_++;    // 每启动一个线程空闲线程数量加1
    }
}

// 定义线程要执行的函数 线程池的所有线程从任务队列里面消费任务
void ThreadPool::threadFunc(int threadid) // 线程函数返回，线程也就结束了
{
    auto lastTime = std::chrono::high_resolution_clock().now();
    while (isPoolRunning_)
    {
        std::shared_ptr<Task> task;
        {
            // 先获取锁
            std::unique_lock<std::mutex> lock(taskQueMtx_);
            // catched模式下，有可能已经创建了很多的线程，但是空闲时间超过60s，应该把多余的线程回收掉
            // 超过initThreadSize数量的线程要进行回收
            // 当前时间 - 上一次线程执行的时间 > 60s
            // 每1s返回一次     怎么区分超时返回，还是有任务待执行返回
            while (taskQue_.size() == 0)
            {
                if (poolMode_ == PoolMode::MODE_CACHED)
                {
                    // 条件变量超时返回
                    if (std::cv_status::timeout == notEmpty_.wait_for(lock, std::chrono::seconds(1)))
                    {
                        auto now = std::chrono::high_resolution_clock().now();
                        auto dur = std::chrono::duration_cast<std::chrono::seconds>(now - lastTime);
                        if (dur.count() >= 60 && curThreadSize_ > initThreadsSize_)
                        {
                            // 回收当前线程
                            // 记录线程相关数量的变量的值修改
                            // 把线程对象从线程列表容器中删除   没有办法匹配 threadfunc()对应的哪个thread对象
                            // threadid 找到 thread对象 再删除
                            threads_.erase(threadid); // std::this_thread::get_id
                            curThreadSize_--;
                            idleThreadSize_--;
                            std::cout << "threadId : " << std::this_thread::get_id() << "exit!" << std::endl;
                            return;
                        }
                    }
                }
                else
                {
                    // 等待notEmpty条件
                    notEmpty_.wait(lock);
                }
                // 线程池要结束，回收线程资源
                if (!isPoolRunning_)
                {
                    threads_.erase(threadid);
                    std::cout << "threads exit !" << std::endl;
                    exitcond_.notify_all();
                    return;
                }
            }

            idleThreadSize_--;
            // 从任务队列取一个任务出来
            task = taskQue_.front();
            taskQue_.pop();
            taskSize_--;

            // 如果依旧有剩余任务，继续通知其他线程执行任务
            if (taskQue_.size() > 0)
            {
                notEmpty_.notify_all();
            }

            // 取出一个任务，通知可以继续生产任务了
            notFull_.notify_all();
        }
        // 出作用域后，这把智能锁就被释放掉了

        // 当前线程负责执行这个任务
        if (task != NULL)
        {
            // task->run();    // 执行任务，把任务的返回值通过setVal方法给到Result
            task->exec();
        }
        idleThreadSize_++;
        lastTime = std::chrono::high_resolution_clock().now(); // 更新线程执行完任务的时间
    }
    threads_.erase(threadid);
    std::cout << "threads exit !" << std::endl;
    exitcond_.notify_all();
}

///////////////////////////////// 线程方法实现

int Thread::generateId_ = 0;

// 线程构造函数
Thread::Thread(ThreadFunc func)
    : func_(func), threadId_(generateId_++)
{
}

// 线程析构函数
Thread::~Thread()
{
}

// 线程启动
void Thread::start()
{
    // 创建一个线程来执行一个线程函数
    std::thread t(func_, threadId_); // c++11来说 线程对象t 和线程函数func_
    t.detach();                      // 设置分离线程
}

// 获取线程ID
int Thread::getId() const
{
    return threadId_;
}

////////////////////// // Task方法实现

Task::Task()
    : result_(nullptr)
{
}

void Task::setResult(Result *res)
{
    result_ = res;
}

void Task::exec()
{
    if (result_ != nullptr)
    {
        result_->setVal(run()); // 这里发生多态调用
    }
}

////////////////////// Result方法的实现

// 构造函数
Result::Result() {}
// 拷贝构造函数
Result::Result(const Result &other)
    : task_(other.task_), isValid_(other.isValid_.load())
{
    // 其他成员的拷贝逻辑
}
Result::Result(std::shared_ptr<Task> task, bool isValid)
    : task_(task), isValid_(isValid)
{
    task->setResult(this);
}

void Result::setVal(Any any)
{
    // 存储task返回值
    any_ = std::move(any);
    sem_.post(); // 已经获取了任务的返回值，增加信号量资源
}

Any Result::get()
{
    if (!isValid_)
    {
        return "";
    }
    sem_.wait(); // task如果没有执行完，这里会阻塞用户的线程
    return std::move(any_);
}
