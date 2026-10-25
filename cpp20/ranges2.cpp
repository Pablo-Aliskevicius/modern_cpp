// writing range-generic code & pitfalls ★★⚡

#include <algorithm>
#include <concepts>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
 
namespace rg = std::ranges;
namespace rv = std::views;
 
template <rg::input_range R>
    requires std::integral<rg::range_value_t<R>>
// works for vectors, arrays, views, generators...
auto total(R&& r) {
    rg::range_value_t<R> s{}; 
    // Integer overflow is possible, but we don't care here
    for (auto x : r) s += x;
    return s;
}
 
// split into pieces (C++23 can build a string_view straight from the subrange)
std::vector<std::string_view> words(std::string_view text) {
    std::vector<std::string_view> out;
    for (auto piece : text | rv::split(' ')) {
        out.emplace_back(&*piece.begin(), static_cast<std::size_t>(rg::distance(piece)));
    }
    return out;
}
 
int main() {
    std::vector<int> v{1, 2, 3, 4};
    // 10 7
    std::cout << total(v) << ' ' << total(v | rv::filter([](int x) { return x > 2; })) << '\n';
 
    for (auto w : words("zero copy views")) std::cout << '[' << w << ']';
    std::cout << '\n';
 
    // Pitfall 1: algorithms on temporaries return std::ranges::dangling, not an iterator
    auto d = rg::find(std::vector{1, 2, 3}, 2);
    static_assert(std::is_same_v<decltype(d), rg::dangling>);
 
    // Pitfall 2: a view does not own its data; keep the underlying container alive
    // a filter view caches begin(): not const-iterable
    auto f = v | rv::filter([](int x) { return x % 2 == 0; });
    for (int x : f) std::cout << x << ' ';   // OK: non-const iteration
    std::cout << '\n';
}
