/*
✎ Exercise 35.  
Write template<rg::range R> auto to_vector(R&& r) 
(C++20 has no ranges::to; that is C++23) 
using rg::copy and std::back_inserter.

*/
#include <algorithm>
#include <iostream> 
#include <ranges>
#include <string>   
#include <vector>
#include <set>

namespace rg = std::ranges;
namespace rv = std::views;

template<rg::range R> auto to_vector(R&& r) {
    std::vector<rg::range_value_t<R>> v;
    v.reserve(rg::distance(r)); // optional, but avoids reallocations
    rg::copy(r, std::back_inserter(v));
    return v;
}

void exercise35() {
    std::set<int> v{1, 2, 3, 4};
    auto v2 = to_vector(v | rv::filter([](int x) { return x % 2 == 0; }));
    for (int x : v2) std::cout << x << ' ';
    std::cout << '\n';
}

int main() {
    exercise35();
}

