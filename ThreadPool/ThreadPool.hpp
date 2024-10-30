/***
 * @Author: jyx
 * @Date: 2024-10-26 07:03:34
 * @LastEditors: jyx
 * @Description:线程池
 */
#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <vector>
#include <queue>
#include <memory>             //智能指针
#include <atomic>             // 原子操作
#include <mutex>              //互斥锁
#include <condition_variable> //条件变量
#include <functional>
#include <unordered_map>

using uLong = unsigned long long;

// Any类型，可以接收任意数据的类型
class Any
{
public:
    Any() = default;
    ~Any() = default;
    Any(const Any &) = delete;
    Any &operator=(const Any &) = delete;
    Any(Any &&) = default;
    Any &operator=(Any &&) = default;

    // 这个构造函数可以让Any类型接收任意其他类型的数据
    template <typename T>
    Any(T data) : base_(std::make_unique<Derive<T>>(data))
    {
    }

    // 这个方法能把Any对象里面存储的数据提取出来
    template <typename T>
    T cast_()
    {
        // 我们怎么从base里面找到他所指向的派生Derive对象，从他里面取出data成员变量
        // 基类指针转换成派生类指针
        // C++四种类型强转 RTTI
        Derive<T> *pd = dynamic_cast<Derive<T> *>(base_.get());
        if (pd == nullptr)
        {
            throw "type is unmatch";
        }
        return pd->data_;
    }

private:
    // 基类类型
    class Base
    {
    public:
        virtual ~Base() = default;
    };

    // 派生类类型
    template <typename T>
    class Derive : public Base
    {
    public:
        Derive(T data) : data_(data)
        {
        }

        T data_;
    };

private:
    // 定义一个基类指针
    std::unique_ptr<Base> base_;
};

// 实现一个信号量类
class Semaphore
{
public:
    Semaphore()
        : resLimit_(0)
    {
    }
    Semaphore(int limit)
        : resLimit_(0)
    {
    }
    ~Semaphore() {}

    // 获取一个信号量资源
    void wait()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        // 等待信号量有资源，没有资源的话阻塞
        cond_.wait(lock, [&]() -> bool
                   { return resLimit_ > 0; });
        resLimit_--;
    }

    // 增加一个信号量资源
    void post()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        resLimit_++;
        cond_.notify_one();
    }

private:
    int resLimit_;
    std::mutex mtx_;
    std::condition_variable cond_;
};

// Task类型的前置声明
class Task;
// 实现接收提交到线程池的task任务执行完成后的返回值类型Result
class Result
{
public:
    // 构造函数
    Result();
    Result(std::shared_ptr<Task> task, bool isValid = true);
    // 拷贝构造函数
    Result(const Result &other);
    ~Result() = default;

    // 问题：setVal方法，获取任务指向完的返回值
    void setVal(Any any);

    // 问题：get方法，用户调用这个方法获取task的返回值
    Any get();

private:
    Any any_;                    // 存储一个任务的返回值
    Semaphore sem_;              // 线程通信信号量
    std::shared_ptr<Task> task_; // 指向对应获取返回值的对象
    std::atomic_bool isValid_;   // 返回值是否有效
};

// 任务抽象类，用户可以自定义任意任务类型，从Task继承，重写run方法，实现自定义任务处理
class Task
{
public:
    Task();
    ~Task() = default;
    void exec();
    virtual Any run() = 0;
    void setResult(Result *res);

private:
    Result *result_;
};

// 线程支持的两种模式
enum class PoolMode
{
    MODE_FIXED,  // 固定数量的线程
    MODE_CACHED, // 线程数量可动态增长
};

// 线程类型
class Thread
{
public:
    // 线程函数对象类型
    using ThreadFunc = std::function<void(int)>;
    // 线程构造函数
    Thread(ThreadFunc func);

    // 线程析构函数
    ~Thread();

    // 启动线程
    void start();

    // 获取线程ID
    int getId() const;

private:
    ThreadFunc func_;
    static int generateId_;
    int threadId_; // 线程ID
};

/*exampel
ThreadPool pool;
pool.start(4);

class MyTask : public Task
{
    public:
        void run(){ // 线程代码......}
};

pool.submitTask(std::make_shared<MyTask>());

*/

// 线程池类型
class ThreadPool
{
public:
    // 线程池构造
    ThreadPool();

    // 线程池析构
    ~ThreadPool();

    // 设置线程池模式
    void setMode(PoolMode mode);

    // 设置线程池catched模式下的线程的数量上限
    void setThreadSizeThreshHold(int threshhold);

    // 设置task任务队列上限的阈值
    void setTaskQueMaxThreshHold(int threshhold);

    // 给线程池提交任务
    Result submitTask(std::shared_ptr<Task> sp);

    // 开启线程池
    void start(int initThreadSize = 4);

    // 禁止对线程池对象进行拷贝构造
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool operator=(const ThreadPool &) = delete;

    // 检查pool的运行状态
    bool checkRunningState() const;

private:
    // 定义线程要执行的函数
    void threadFunc(int threadid);

private:
    // std::vector<std::unique_ptr<Thread>> threads_; // 线程列表
    std::unordered_map<int, std::unique_ptr<Thread>> threads_;
    size_t initThreadsSize_;         // 初始的线程数量
    std::atomic_int curThreadSize_;  // 记录当前线程池里面的线程的总数量
    std::atomic_int idleThreadSize_; // 记录空闲线程的数量
    int threadSizeThreshHold_;       // 线程数量上限的数量

    std::queue<std::shared_ptr<Task>> taskQue_; // 使用智能指针，任务队列
    std::atomic_uint taskSize_;                 // 任务的数量
    uint taskQueMaxThreshHold_;                 // 任务队列数量的上限

    std::mutex taskQueMtx_;            // 保证任务队列的线程安全
    std::condition_variable notFull_;  // 表示任务队列不满
    std::condition_variable notEmpty_; // 表示任务队列不空
    std::condition_variable exitcond_;  //等待线程资源全部回收

    PoolMode poolMode_;              // 当前线程池的工作模式
    std::atomic_bool isPoolRunning_; // 表示当前线程池的启动状态
};

#endif