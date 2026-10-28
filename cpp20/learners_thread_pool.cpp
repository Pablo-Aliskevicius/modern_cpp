/*
A better implementation can be found at 
https://github.com/ptsouchlos/thread-pool/blob/main/include/thread_pool/thread_pool.h

This is just an example, showing how to implement a thread pool using C++ 20 features.

Single queue, may cause contention if many threads are waiting for tasks.  
A more advanced implementation could use a work-stealing queue (deque) per thread to reduce contention.
The destructor discards items remaining in the queue upon shutdown, workers exit instantly.
This behavior mirrors human workers' behavior.

*/

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <concepts>
#include <memory>
#include <chrono>

class ThreadPool {
public:
    // Initializes the pool with system-recommended core count or a default minimum
    explicit ThreadPool(size_t threads = std::max(1u, std::thread::hardware_concurrency())) {
        for (size_t i = 0; i < threads; ++i) {
            // std::jthread passes its own std::stop_token automatically to the worker loop
            workers_.emplace_back([this](std::stop_token stop_tok) {
                this->worker_loop(stop_tok);
            });
        }
    }

    // Explicit destructor required to clear the queue before threads auto-join
    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            // Clearing the queue prevents worker threads from processing leftover jobs after stop request
            std::queue<std::function<void()>> empty;
            std::swap(tasks_, empty);
        }
        // When workers_ goes out of scope, jthreads request stop and automatically join
    }

    // Disable copying and moving to ensure safety of the background threads
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    // Enqueue arbitrary tasks using C++20 templates and perfect forwarding
    template <typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<std::invoke_result_t<F, Args...>> 
    {
        using return_type = std::invoke_result_t<F, Args...>;

        // Package the task with its arguments into a shared_ptr
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
        std::future<return_type> res = task->get_future();
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            
            // Type-erase the execution using a lambda
            tasks_.emplace([task]() { (*task)(); });
        }
        
        // Notify one waiting worker thread
        cv_.notify_one();
        return res;
    }

private:
    void worker_loop(std::stop_token stop_token) {
        while (!stop_token.stop_requested()) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(queue_mutex_);
                
                // C++20 condition_variable_any seamlessly integrates with a std::stop_token.
                // It wakes up automatically if a task arrives OR if the thread is requested to stop.
                cv_.wait(lock, stop_token, [this]() {
                    return !tasks_.empty();
                });

                if (stop_token.stop_requested() && tasks_.empty()) {
                    return;
                }

                task = std::move(tasks_.front());
                tasks_.pop();
            }

            // Execute the task outside the lock to allow concurrent processing
            task();
        }
    }

    std::vector<std::jthread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable_any cv_;
};

// Example usage demonstration
int main() {
    ThreadPool pool(4);

    // Enqueue basic lambdas
    auto f1 = pool.enqueue([]() { return 42; });
    auto f2 = pool.enqueue([](int x) { return x * 10; }, 5);

    std::cout << "Result 1: " << f1.get() << "\n";
    std::cout << "Result 2: " << f2.get() << "\n";

    return 0;
}