/*
✎ Exercise 7. Write template<class T> struct Wrapper { T value; }; and check that Wrapper w{42}; deduces
without a guide (aggregate CTAD needs C++20 — what does C++17 need?).
Hint: In C++17 aggregates need an explicit guide: template<class T> Wrapper(T) -> Wrapper<T>;

*/

#include <type_traits>
#include <string_view>
#include <iostream>

template<class T> struct Wrapper {
    T value;
    Wrapper(T v) { value = v; } // Instead of type guide
};

// No guide is needed in C++20, but in C++17 we need to provide one explicitly:
// template<class T> Wrapper(T) -> Wrapper<T>;
bool test_wrapper() {
    Wrapper w{42}; // should deduce Wrapper<int>
    Wrapper float_wrapper{3.14f};
    Wrapper view_wrapper{std::string_view{"hello"}};
    static_assert(std::is_same_v<decltype(w), Wrapper<int>>,
                  "Wrapper should deduce T as int");
    static_assert(std::is_same_v<decltype(float_wrapper), Wrapper<float>>,
                  "Wrapper should deduce T as float");
    static_assert(std::is_same_v<decltype(view_wrapper), Wrapper<std::string_view>>,
                  "Wrapper should deduce T as std::string_view");
    return w.value == 42 && float_wrapper.value == 3.14f &&
           view_wrapper.value == "hello";
}

int main() {
    if (test_wrapper()) {
        std::cout << "All tests passed!\n";
        return 0;
    } else {
        std::cout << "Some tests failed.\n";
        return 1;
    }
}