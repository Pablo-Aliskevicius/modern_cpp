#include <algorithm>
#include <cctype>
#include <compare>
#include <iostream>
#include <set>
#include <string>
 
/*
⚠ Gotcha:  For floating-point members the defaulted <=> returns partial_ordering (NaN is unordered). 
Declare it std::strong_ordering explicitly if you want a compile error instead of that silent weakening.
*/

struct Version {
    int major, minor, patch;
    auto operator<=>(const Version&) const = default;  // memberwise, also gives ==
};
 
struct Caseless {
    std::string s;
    std::weak_ordering operator<=>(const Caseless& o) const {
        // stricmp is accent- and locale-sensitive, so we implement a simple case-insensitive comparison here
        return std::lexicographical_compare_three_way(
            s.begin(), s.end(), o.s.begin(), o.s.end(),
            [](char a, char b) { return std::tolower(a) <=> std::tolower(b); });
    }
    // not synthesized for custom <=>
    bool operator==(const Caseless& o) const { return (*this <=> o) == 0; }
};
 
int main() {
    Version a{1, 4, 2}, b{1, 10, 0};
    std::cout << (a < b) << (a == b) << (a >= b) << '\n';           // 100
    std::set<Version> releases{b, a};
    std::cout << releases.begin()->minor << '\n';                   // 4
 
    Caseless x{"Hello"}, y{"hELLO"};
    std::cout << (x == y) << std::is_eq(x <=> y) << '\n';           // 11
    auto r = 3 <=> 5;                                               // std::strong_ordering
    std::cout << std::is_lt(r) << '\n';
}
