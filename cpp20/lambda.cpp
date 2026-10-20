#include <iostream>
#include <set>
#include <utility>
#include <vector>
 
int main() {
    // template lambda: name the type, constrain it
    auto first_of = []<typename T>(const std::vector<T>& v) -> const T& { return v.front(); };
    std::vector<int> vi{4, 5};
    std::vector<std::string> vs{"hello", "world"};
    std::cout << first_of(vi) << ' ' << first_of(vs) << '\n';
 
    // capture a whole parameter pack (by move)
    // Loop is unrolled at compile time, so no runtime overhead
    auto make_summer = [](auto... xs) {
        return [... xs = std::move(xs)] { return (xs + ... + 0); };
    };
    std::cout << make_summer(1, 2, 3)() << '\n';           // 6
 
    // captureless lambdas are default-constructible and usable in types
    std::set<int, decltype([](int a, int b) { return a > b; })> descending{1, 3, 2};
    std::cout << *descending.begin() << '\n';              // 3
 
    // explicit this capture when also copying everything else
    struct S { int n = 9; auto get() { return [=, this] { return n; }; } };
    std::cout << S{}.get()() << '\n';
}
