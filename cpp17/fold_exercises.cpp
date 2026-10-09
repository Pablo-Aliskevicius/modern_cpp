/*

✎ Exercise 5. 
Write `template<class... Ts> bool any_of_equal(const T& x, Ts... ts)` returning whether x
equals any ts using `||` fold.

✎ Exercise 6. 
Write a variadic `join(sep, args...)` that prints values separated by sep with no trailing separator.
Hint: Use a lambda with a bool first flag, called from a comma fold.

*/
#include <iostream>
#include <string_view>


template <typename T, typename... Ts>
bool any_of_equal(const T& x, Ts... ts) 
{ 
    return ((x == ts) || ...); 
}

/* Return whether all the test cases got the expected results */
bool test_any_of_equal() {
    const bool first_passes = !any_of_equal(1, 2, 3, 4);
    const bool second_passes = any_of_equal(1, 2, 1, 4);
    const bool third_passes = any_of_equal(
        std::string_view{"hello"}, std::string_view{"world"}, std::string_view{"hello"});

    return first_passes && second_passes && third_passes;
}


template <typename... Ts>
void join(std::string_view sep, const Ts&... ts)
{
    bool first = true;
    auto print_with_sep = [&](const auto& v) {
        if (!first) std::cout << sep;
        std::cout << v;
        first = false;
    };
    (print_with_sep(ts), ...);
    std::cout << '\n';
}

/* Should return whether all the test cases got the expected results */
bool test_join() {
    std::cout << "Testing join function:\n";
    join(", ", 1, 2, 3);
    join(" - ", "hello", "world");
    return true;
}

int main() {
    const bool any_of_equal_passes = test_any_of_equal();
    const bool join_passes = test_join();

    if (any_of_equal_passes && join_passes) {
        std::cout << "All tests passed!\n";
        return 0;
    } else {
        std::cout << "Some tests failed.\n";
        return 1;
    }
}