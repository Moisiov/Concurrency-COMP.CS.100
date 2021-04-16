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

template<class T>
auto ThreadPool::enqueue(ThreadPool::Task task)->std::future<decltype (task())>
{
    auto wrapper = std::make_shared<std::packaged_task
            <decltype(task())()>>(std::move(task));

    {
        std::unique_lock<std::mutex> lock{m_mutex_};
        m_tasks_.emplace([=]{(*wrapper)();});
    }

    m_cond_var_.notify_one();
    return wrapper->get_future();
}


void ThreadPool::start(std::size_t thread_count)
{
    for (unsigned i = 0; i < thread_count; ++i)
    {
        m_threads_.emplace_back([=]{
            while (1)
            {
                Task task;
                {
                    std::unique_lock<std::mutex> lock{m_mutex_};
                    m_cond_var_.wait(lock, [=]{return m_stopping_
                                || !m_tasks_.empty();});

                    if (m_stopping_ && m_tasks_.empty()) break;

                    task = std::move(m_tasks_.front());
                    m_tasks_.pop();
                }

                task();
            }
        });
    }
}

void ThreadPool::stop() noexcept
{
    {
        std::unique_lock<std::mutex> lock{m_mutex_};
        m_stopping_ = true;
    }

    m_cond_var_.notify_all();

    for (auto& th : m_threads_) {
        th.join();
    }
}
