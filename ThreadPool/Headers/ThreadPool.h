#pragma once

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "SafeQueue.h"

class ThreadPool {
private:
    class ThreadWorker {
    private:
        int m_id;
        ThreadPool * m_pool;
    public:
        ThreadWorker(ThreadPool * pool, const int id)
                : m_pool(pool), m_id(id) {
        }

        void operator()() {
            std::function<void()> func;
            bool dequeued;
            while (!m_pool->m_shutdown) {
                {
                    std::unique_lock<std::mutex> lock(m_pool->m_conditional_mutex);
                    if (m_pool->m_queue.empty()) {
                        m_pool->m_conditional_lock.wait(lock);
                    }
                    dequeued = m_pool->m_queue.dequeue(func);
                }
                if (dequeued) {
                    func();
                }
            }
        }
    };

    bool m_shutdown;
    SafeQueue<std::function<void()>> m_queue;
    std::vector<std::thread> m_threads;
    std::mutex m_conditional_mutex;
    std::condition_variable m_conditional_lock;
public:
    ThreadPool();

    ThreadPool(const int n_threads);
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&)  = delete;

    ThreadPool & operator=(const ThreadPool &)  = delete;
    ThreadPool & operator=(ThreadPool &&) = delete;

    // Inits thread pool
    void init(const size_t& threads_number);
    void init();
    // Waits until threads finish their current task and shutdowns the pool
    void shutdown();

    // Submit a function to be executed asynchronously by the pool
    template<typename F, typename...Args>
    auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))>;
};
