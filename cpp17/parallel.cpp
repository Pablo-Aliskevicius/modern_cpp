// Pass an execution policy first: seq, par, par_unseq. libstdc++ uses Intel TBB
// underneath, so install libtbb-dev and add `-ltbb.`
// Install with: sudo apt install libtbb-dev
// Compile with: g++ -std=c++17 -O2 -ltbb parallel.cpp 

/*
⚠ Gotcha: reduce requires an associative and commutative operation
(floating-point sums differ by run). 
Exceptions thrown inside a parallel algorithm call std::terminate.
*/

#include <algorithm>
#include <cstdint>
#include <execution>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>


int main() {
  std::vector<std::uint32_t> v(2'000'000);
  std::mt19937 rng(1);
  std::generate(v.begin(), v.end(), rng);
  std::sort(std::execution::par, v.begin(), v.end());
  auto total =
      std::reduce(std::execution::par, v.begin(), v.end(), std::uint64_t{0});
  auto sumsq = std::transform_reduce(
      std::execution::par_unseq, v.begin(), v.end(), 0.0, std::plus<>{},
      [](std::uint32_t x) { return double(x) * x; });
  std::vector<std::uint64_t> prefix(v.size());
  std::inclusive_scan(std::execution::par, v.begin(), v.end(), prefix.begin(),
                      std::plus<>{}, std::uint64_t{0});
  std::cout << total << ' ' << (prefix.back() == total) << ' ' << (sumsq > 0)
            << '\n';
}