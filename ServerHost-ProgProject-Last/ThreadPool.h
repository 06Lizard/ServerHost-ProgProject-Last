// ThreadPool.h
#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="numThreads"></param>
    ThreadPool(size_t numThreads);

    /// <summary>
    /// 
    /// </summary>
    ~ThreadPool();

    /// <summary>
    /// 
    /// </summary>
    /// <typeparam name="F"></typeparam>
    /// <typeparam name="...Args"></typeparam>
    /// <param name="f"></param>
    /// <param name="...args"></param>
    template<class F, class... Args>
    void enqueue(F&& f, Args&&... args);

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};

template<class F, class... Args>
void ThreadPool::enqueue(F&& f, Args&&... args) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.emplace(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    }
    condition.notify_one();
}
