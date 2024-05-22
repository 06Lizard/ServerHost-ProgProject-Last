#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
public:
    /// <summary>
    /// Takes in the amount of threads we want to use in the pool
    /// </summary>
    /// <param name="numThreads"></param>
    ThreadPool(size_t numThreads);

    /// <summary>
    /// 
    /// </summary>
    ~ThreadPool();

    /// <summary>
    /// this is what takes in tasks and quues them up in the threadpool
    /// </summary>
    /// <typeparam name="F"></typeparam>
    /// <typeparam name="...Args"></typeparam>
    /// <param name="f"></param>
    /// <param name="...args"></param>
    template<class F, class... Args>
    void enqueue(F&& f, Args&&... args);

private:
    std::vector<std::thread> threads; // vector for all threads 
    std::queue<std::function<void()>> tasks; // the task queue
    std::mutex queueMutex;  // mutual exclude to make sure the tasts synchronice with eatchother
    std::condition_variable condition; // condition to notify the states of diferent tasks on the threads 
    bool stop; // just a bool to be able to stop the threadpool
};

template<class F, class... Args>
void ThreadPool::enqueue(F&& f, Args&&... args) 
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.emplace(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    }
    condition.notify_one();
}