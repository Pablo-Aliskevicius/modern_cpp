/*
# ✎ Exercise 18. #
Time `std::sort` vs `std::sort(par, ...)` on 20 M elements with `std::chrono`;
try `-O0` vs `-O2` and note the difference.

Compile with:
g++ -std=c++17 -O2 -ltbb parallel_exercises.cpp
*/

#include <algorithm>
#include <chrono>
#include <execution>
#include <iostream>
#include <random>
#include <vector>

void time_sort() {
    std::vector<int> sequential_values(20'000'000);
    std::mt19937 rng(1);
    std::generate(sequential_values.begin(), sequential_values.end(), rng);
    std::vector<int> parallel_values = sequential_values;

    auto start = std::chrono::high_resolution_clock::now();
    std::sort(sequential_values.begin(), sequential_values.end());
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seq = end - start;
    std::cout << "Sequential sort took: " << elapsed_seq.count() << " seconds\n";

    start = std::chrono::high_resolution_clock::now();
    std::sort(std::execution::par, parallel_values.begin(), parallel_values.end());
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_par = end - start;
    std::cout << "Parallel sort took: " << elapsed_par.count() << " seconds\n";
}


int main() {
    time_sort();
    return 0;
}

/*
O2:

Sequential sort took: 2.22592 seconds
Parallel sort took: 0.98888 seconds

O0:
Sequential sort took: 11.725 seconds
Parallel sort took: 6.32061 seconds

Always twice. 4 processors, lots of processes running.
*/