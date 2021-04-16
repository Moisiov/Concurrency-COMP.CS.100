#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <thread>
#include <vector>
#include <condition_variable>
#include <mutex>

class ThreadPool
{
public:
    using Task = std::function<void()>;

    explicit ThreadPool(std::size_t threadCount);

    ~ThreadPool();

private:
    std::vector<std::thread> m_threads_;
    std::condition_variable m_cond_var_;
    std::mutex m_mutex_;
};

#endif // THREADPOOL_H
