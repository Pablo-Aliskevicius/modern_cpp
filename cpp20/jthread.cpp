/*
6.16 std::jthread and std::stop_token ★★★⚡
jthread joins in its destructor (no more terminate() from an un-joined std::thread) 
and carries a cooperative-cancellation stop_token. 
condition_variable_any::wait accepts a token, so a blocked consumer wakes up when 
stop is requested.

*/

#include <atomic>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <optional>
#include <stop_token>
#include <thread>
 
class Queue {
    std::mutex m_;
    std::condition_variable_any cv_;
    std::deque<int> q_;
public:
    void push(int x) { { std::lock_guard l{m_}; q_.push_back(x); } cv_.notify_one(); }
    std::optional<int> pop(std::stop_token st) {
        std::unique_lock l{m_};
        if (!cv_.wait(l, st, [&] { return !q_.empty(); })) return std::nullopt;   // stop requested
        int x = q_.front(); q_.pop_front();
        return x;
    }
};
 
int basic_implementation() {
    Queue q;
    std::atomic<int> sum{0};
    {
        std::jthread consumer([&](std::stop_token st) {
            // exits when stop is requested while idle
            while (auto x = q.pop(st)) sum += *x;
        });
        std::stop_callback cb(consumer.get_stop_token(), [] { std::cout << "stop requested\n"; });
 
        for (int i = 1; i <= 100; ++i) q.push(i);
        std::cout << "pushed 1..100\n";
        while (sum != 5050) std::this_thread::yield();           // wait for the work (demo only)
    }                                                            // ~jthread: request_stop() then join()
    std::cout << "with one thread: " << sum << '\n';                                    // 5050
    return sum;
}

/*
✎ Exercise 42.  
Add a second consumer thread and a stop_source shared by both; 
call request_stop() from the main thread 
and verify both exit.
*/

void consumer(Queue& q, std::stop_token st, std::atomic<int>& sum) {
    while (auto x = q.pop(st)) {
        sum += *x;
        // simulate work
        std::this_thread::sleep_for(std::chrono::milliseconds(((unsigned int)rand()) & 0x3F)); 
    }
}

void exercise_42() {
    Queue q;
    std::atomic<int> sum{0};
    std::stop_source stop_src;
    std::jthread consumer1(consumer, std::ref(q), stop_src.get_token(), std::ref(sum));
    std::jthread consumer2(consumer, std::ref(q), stop_src.get_token(), std::ref(sum));
 
    for (int i = 1; i <= 100; ++i) q.push(i);
    std::cout << "pushed 1..100\n";
    // while (sum != 5050) std::this_thread::yield();           // wait for the work (demo only)
    stop_src.request_stop();                                 // request both consumers to exit
    consumer1.join();
    consumer2.join();
    std::cout << "with 2 threads: " << sum << '\n';                                    // 5050
    stop_src.request_stop();                                 // request both consumers to exit
}


// end of exercise 42.
int main() {
    basic_implementation();
    exercise_42();
    return 0;
}
