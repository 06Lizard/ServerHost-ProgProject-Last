#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads) : stop(false) 
{
    // create number of threads
    for (size_t i = 0; i < numThreads; ++i) 
    {
        threads.emplace_back([this]() 
        {
            while (true)
            {
                std::function<void()> task;
                {
                    // looks the accses to the queue so it's protected
                    std::unique_lock<std::mutex> lock(queueMutex);
                    // waits for new task or threadpool is stoped
                    condition.wait(lock, [this]() { return stop || !tasks.empty(); });
                    // exits if it was taht the thread was stoped
                    if (stop && tasks.empty()) return;
                    // otherwhise recive the task from queue
                    task = std::move(tasks.front());
                    // removes the task from queue so multiple threads don't work on the same task
                    tasks.pop();
                }
                // executes that task
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool()
{
    std::unique_lock<std::mutex> lock(queueMutex);
    stop = true;
    condition.notify_all();
    for (std::thread& thread : threads) 
    {
        thread.join();
    }
}