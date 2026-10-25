/*
# ✎ Exercise 33. #
From a `std::vector<std::string>` of file names, keep those ending in ".dat" (use `.ends_with`), take the first 3, and print them using views only.

# ✎ Exercise 34. #
Using `rv::iota(1)`, `rv::filter` and `rv::take`, print the first 10 numbers divisible by 7.
*/
#include <vector>
#include <string>
#include <ranges>
#include <iostream>

namespace rv = std::views;

void exercise33() {
    std::vector<std::string> files{"a.dat", "b.txt", "c.dat", "d.doc", "e.dat", "f.dat"};
    auto dat_files = files | rv::filter([](const std::string& s) { return s.ends_with(".dat"); })
                           | rv::take(3);
    for (const auto& f : dat_files) std::cout << f << ' ';
    std::cout << '\n';
}

void exercise34() {
    auto divisible_by_7 = rv::iota(1) | rv::filter([](int x) { return x % 7 == 0; })
                                      | rv::take(10);
    for (int x : divisible_by_7) std::cout << x << ' ';
    std::cout << " [1]\n";

    // Mine. No filter, just iota and multiply.
    auto divisible_by_7_alt = rv::iota(1, 11) | rv::transform([](int x) { return x * 7; });
    for (int x : divisible_by_7_alt) std::cout << x << ' ';
    std::cout << " [2]\n";
}

int main() {
    exercise33();
    exercise34();
}