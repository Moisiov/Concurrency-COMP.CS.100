#include "threadpool.h"

ThreadPool::ThreadPool()
{
    // check hardware supported threads and start that many threads
    // if no supported threads detected, set thread count to 8
    unsigned supported_threads = std::thread::hardware_concurrency();
    start(supported_threads ? supported_threads : 8);
}

ThreadPool::ThreadPool(std::size_t thread_count)
{
    // start given amount of threads
    start(thread_count);
}

ThreadPool::~ThreadPool()
{
    stop();
}

void ThreadPool::enqueue(Task task)
{
    // use mutex lock in critical scope
    {
        std::unique_lock<std::mutex> lock(m_mutex_);

        // move task to queue instead of copying
        m_tasks_.emplace(std::move(task));
    }

    // wake up one thread
    m_cond_var_.notify_one();
}

void ThreadPool::start(std::size_t thread_count)
{
    // add threads to m_threads_ and run infinite loop that checks and runs tasks from task queue m_tasks_
    for (unsigned i = 0; i < thread_count; ++i)
    {
        m_threads_.emplace_back([=]{
            while (1)
            {
                Task task;

                // use mutex lock in critical scope
                {
                    std::unique_lock<std::mutex> lock{m_mutex_};

                    // wait if ThreadPool not stopping and task queue is empty
                    m_cond_var_.wait(lock, [=]{return m_stopping_
                                || !m_tasks_.empty();});

                    // break the infinite loop if ThreadPool stopping and task queue is empty
                    if (m_stopping_ && m_tasks_.empty()) break;

                    // get next task from the queue
                    task = std::move(m_tasks_.front());
                    m_tasks_.pop();
                }

                // execute task
                task();
            }
        });
    }
}

void ThreadPool::stop() noexcept
{
    // use mutex lock in critical scope and set m_stopping_ flag true
    {
        std::unique_lock<std::mutex> lock{m_mutex_};
        m_stopping_ = true;
    }

    // wake up all threads
    m_cond_var_.notify_all();

    // wait for all the threads to finish
    for (auto& th : m_threads_) {
        th.join();
    }
}
