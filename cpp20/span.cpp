/*
⚠ Gotcha:  span has no bounds checking in C++20 (at() arrives in C++26); 
with libstdc++ assertions (-D_GLIBCXX_ASSERTIONS) operator[] is checked.
*/

#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <span>
#include <vector>
#include <thread>
 
std::uint32_t sum_bytes(std::span<const std::byte> data) {         // pointer + size in one parameter
    std::uint32_t s = 0;
    for (std::byte b : data) s += std::to_integer<std::uint32_t>(b);
    return s;
}
 
void process_chunks(std::span<const int> all, std::size_t chunk) {
    while (!all.empty()) {
        auto c = all.first(std::min(chunk, all.size()));            // view of a prefix
        std::cout << c.size() << ' ';
        all = all.subspan(c.size());                                // advance
    }
    std::cout << '\n';
}
 
/*
✎ Exercise 41.  
Write std::pair<std::span<int>, std::span<int>> split_half(std::span<int>) 
and use it to implement a parallel sum with two std::jthreads.
*/

std::pair<std::span<int>, std::span<int>> split_half(std::span<int> data) {
    // Implementation of split_half function
    // This function will split the input span into two halves and return them as a pair of spans.
    // The implementation is left as an exercise for the reader.
    auto mid = data.size() / 2;
    return {data.first(mid), data.subspan(mid)};
}

int sum_span(std::span<int> data) {
    std::cout << "sum_span: " << data.size() << '\n';
    std::uint32_t s = 0;
    for (int i : data) s += i;
    return s;
}

int sum_parallel(std::span<int> data) {
    auto [first_half, second_half] = split_half(data);
    std::uint32_t sum1 = 0, sum2 = 0;

    std::jthread t1([&]() { sum1 = sum_span(first_half); });
    std::jthread t2([&]() { sum2 = sum_span(second_half); });

    t1.join();
    t2.join();

    return sum1 + sum2;
}

// End of exercise 41.

int main() {
    std::array<std::byte, 4> a{std::byte{1}, std::byte{2}, std::byte{3}, std::byte{4}};
    std::vector<std::byte> v(10, std::byte{1});
    std::byte raw[3] = {std::byte{9}, std::byte{9}, std::byte{9}};
    // one signature, any contiguous source
    std::cout << sum_bytes(a) << ' ' << sum_bytes(v) << ' ' << sum_bytes(raw) << '\n';
 
    std::vector<int> ints{1, 2, 3, 4, 5, 6, 7};
    process_chunks(ints, 3);                                        // 3 3 1
 
    auto bytes = std::as_bytes(std::span{ints});                    // reinterpret as read-only bytes
    std::cout << bytes.size() << '\n';                              // 28
    for (auto b : bytes) std::cout << std::to_integer<int>(b) << ' ';
    std::cout << '\n';
 
    // static extent: size known at compile time
    std::span<int, 4> fixed{ints.data(), 4};
    static_assert(decltype(fixed)::extent == 4);

    std::cout << sum_parallel(ints) << '\n'; // exercise 41: parallel sum of ints using split_half and jthreads

}
