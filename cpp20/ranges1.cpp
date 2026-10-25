/*

Ranges bring:P 
(1) range-based algorithms in std::ranges:: that take a container instead of iterator pairs
(2) projections to sort/search by a member
(3) lazy views composed with |. Nothing is copied or computed until you iterate.

*/

#include <algorithm>
#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <vector>
 
namespace rg = std::ranges;
namespace rv = std::views;
 
struct Person { std::string name; int age; };
 
int main() {
    std::vector<int> v{5, 3, 8, 1, 9, 2, 6};
    rg::sort(v);                                             // no begin()/end()
 
    auto evens_sq = v | rv::filter([](int x) { return x % 2 == 0; })
                      | rv::transform([](int x) { return x * x; });
    for (int x : evens_sq) std::cout << x << ' ';            // 4 36 64   (computed lazily here)
    std::cout << '\n';
 
    // 5 4 3 2 1  (iota is infinite!)
    for (int x : rv::iota(1) | rv::take(5) | rv::reverse) std::cout << x << ' ';
    std::cout << '\n';
 
    std::vector<Person> ps{{"Ann", 31}, {"Bob", 25}, {"Cy", 40}};
    rg::sort(ps, {}, &Person::age);                          // projection: compare by age
    for (const auto& n : ps | rv::transform(&Person::name)) std::cout << n << ' ';
    std::cout << '\n';
 
    if (auto it = rg::find(ps, "Cy", &Person::name); it != ps.end()) std::cout << it->age << '\n';
 
    std::map<std::string, int> sizes{{"a", 10}, {"b", 20}};
    for (int s : sizes | rv::values) std::cout << s << ' ';
    std::cout << '\n';
    std::cout << rg::count_if(v, [](int x) { return x > 4; }) << '\n';
}
