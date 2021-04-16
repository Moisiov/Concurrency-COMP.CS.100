#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <thread>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <queue>

/**
 * @brief The ThreadPool class handles threads and tasks.
 */
class ThreadPool
{
public:
    using Task = std::function<void()>;

    /**
     * @brief ThreadPool constructor creates a ThreadPool object.
     * Constructor checks the defined amount of threads in the system
     * and adds that many threads to the pool. If supported threads
     * is not defined, number of threads defaults to 8.
     */
    ThreadPool();

    /**
     * @brief ThreadPool constructor overload allows ThreadPool to be
     * created with any given number of threads.
     * @param thread_count number of threads
     */
    explicit ThreadPool(std::size_t thread_count);

    ~ThreadPool();

private:
    std::vector<std::thread> m_threads_;
    std::condition_variable m_cond_var_;
    std::mutex m_mutex_;
    bool m_stopping_ = false;
    std::queue<Task> m_tasks_;

    void start(std::size_t thread_count);
    void stop() noexcept;
};

#endif // THREADPOOL_H
