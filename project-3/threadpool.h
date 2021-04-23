#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <thread>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <future>

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

    /**
     * @brief ThreadPool destructor calls stop().
     */
    ~ThreadPool();

    /**
     * @brief enqueue puts a new task in the task queue.
     * @param task is a void function
     */
    void enqueue(Task task);

private:
    /**
     * @brief m_threads_ holds all the threads in thread pool
     */
    std::vector<std::thread> m_threads_;

    /**
     * @brief m_cond_var_ is used to notify threads
     */
    std::condition_variable m_cond_var_;

    /**
     * @brief m_mutex_ mutex lock for ThreadPool
     */
    std::mutex m_mutex_;

    /**
     * @brief m_stopping_ flag indicating if ThreadPool is stopping (destructor called)
     */
    bool m_stopping_ = false;

    /**
     * @brief m_tasks_ holds all the functions waiting to be executed
     */
    std::queue<Task> m_tasks_;

    /**
     * @brief start starts the thread pool with given number of threads
     * @param thread_count number of threads
     */
    void start(std::size_t thread_count);

    /**
     * @brief stop waits for all the tasks and threads to finish and
     * sets m_stopping_ flag to true
     */
    void stop() noexcept;
};

#endif // THREADPOOL_H
