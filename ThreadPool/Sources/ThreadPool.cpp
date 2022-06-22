#include "../Headers/ThreadPool.h"
ThreadPool::ThreadPool(){}
ThreadPool::ThreadPool(const int n_threads) : m_threads(std::vector<std::thread>(n_threads)), m_shutdown(false) {
}

// Inits thread pool
void ThreadPool::init(const size_t& threads_number) {
    if(m_threads.empty()){
        m_threads = std::vector<std::thread>(threads_number);
    }
    else{
        this->shutdown();
        m_threads = std::vector<std::thread>(threads_number);
        m_shutdown = false;
    }
    for (int i = 0; i < m_threads.size(); ++i) {
        m_threads[i] = std::thread(ThreadWorker(this, i));
    }
}
void ThreadPool::init(){
    if(m_threads.empty()){
        return;
    }
    for (int i = 0; i < m_threads.size(); ++i) {
        m_threads[i] = std::thread(ThreadWorker(this, i));
    }
}

// Waits until threads finish their current task and shutdowns the pool
void ThreadPool::shutdown() {
    m_shutdown = true;
    m_conditional_lock.notify_all();

    for (int i = 0; i < m_threads.size(); ++i) {
        if(m_threads[i].joinable()) {
            m_threads[i].join();
        }
    }
}

// Submit a function to be executed asynchronously by the pool
template<typename F, typename...Args>
auto ThreadPool::submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
    // Create a function with bounded parameters ready to execute
    std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    // Encapsulate it into a shared ptr in order to be able to copy construct / assign
    auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

    // Wrap packaged task into void function
    std::function<void()> wrapper_func = [task_ptr]() {
        (*task_ptr)();
    };

    // Enqueue generic wrapper function
    m_queue.enqueue(wrapper_func);

    // Wake up one thread if its waiting
    m_conditional_lock.notify_one();

    // Return future from promise
    return task_ptr->get_future();
}