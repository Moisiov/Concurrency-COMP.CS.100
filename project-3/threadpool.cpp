#include "threadpool.h"

ThreadPool::ThreadPool()
{
    unsigned supported_threads = std::thread::hardware_concurrency();
    start(supported_threads ? supported_threads : 8);
}

ThreadPool::ThreadPool(std::size_t thread_count)
{
    start(thread_count);
}

ThreadPool::~ThreadPool()
{
    stop();
}

void ThreadPool::start(std::size_t thread_count)
{
    for (unsigned i = 0; i < thread_count; ++i)
    {
        m_threads_.emplace_back([=]{
            while (1)
            {
                std::unique_lock<std::mutex> lock{m_mutex_};
                m_cond_var_.wait(lock, [=]{return m_stopping_;});

                if (m_stopping_) break;
            }
        });
    }
}
