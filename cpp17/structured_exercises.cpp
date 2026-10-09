#include <array>
#include <charconv>
#include <iostream>
#include <map> 
#include <string>
#include <string_view>
#include <tuple>
#include <vector>


/*
✎ Exercise 1.
Write `std::tuple<bool, int, std::string> try_parse(std::string_view)` and consume it with a
structured binding. Then use one on a `std::array<int,3>`.

_Hint:_ Arrays, tuple-likes and structs with all-public non-static members all work; 
the number of names must match exactly.

Return a tuple with three elements: a boolean indicating success, the parsed integer (or 0 on failure), and an error message (or empty string on success).
*/
/**
@param sv The string view to parse as an integer.
@return A tuple containing a boolean indicating success, the parsed integer (or 0 on failure), and an error message (or empty string on success).
*/
std::tuple<bool, int, std::string> try_parse(std::string_view sv) {
    int value = 0;
    const auto [end, error] = std::from_chars(sv.data(), sv.data() + sv.size(), value);

    if (error == std::errc::invalid_argument) {
        return {false, 0, "Input is not an integer"};
    }
    if (error == std::errc::result_out_of_range) {
        return {false, 0, "Integer is out of range"};
    }
    if (end != sv.data() + sv.size()) {
        return {false, 0, "Input contains trailing characters"};
    }

    return {true, value, {}};
}

/*
✎ Exercise 2.
Iterate a `std::map<std::string, std::vector<int>>` printing each key and the vector size 
using only structured bindings.
*/

void print_map(const std::map<std::string, std::vector<int>>& m) {
    for (const auto& [key, vec] : m) {
        std::cout << key << ": " << vec.size() << '\n';
    }
}


int main() {
    // Exercise 1: use `helper` to reuse the structured binding names.
    auto helper = try_parse("bad input");
    auto& [success, value, error_message] = helper;
    std::cout << "Success: " << std::boolalpha << success << ", Value: " << value 
              << ", Error: " << error_message << '\n';

    helper = try_parse("123");
    std::cout << "Success: " << std::boolalpha << success << ", Value: " << value 
              << ", Error: " << error_message << '\n';

    std::array<int, 3> arr = {1, 2, 3};
    auto [a, b, c] = arr;
    std::cout << "Array values: " << a << ", " << b << ", " << c << '\n';

    // Exercise 2
    std::map<std::string, std::vector<int>> my_map = {
        {"first", {1, 2, 3}},
        {"second", {4, 5}},
        {"third", {6}}
    };
    print_map(my_map);

    return 0;
}