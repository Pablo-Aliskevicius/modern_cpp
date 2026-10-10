/*
✎ Exercise 12.

Write `std::optional<int> parse_int(std::string_view)` using `std::from_chars` (see §5.15).  
Return nullopt on trailing garbage.
*/
#include <charconv>
#include <optional>
#include <string_view>
#include <iostream>

std::optional<int> parse_int(std::string_view sv)
{
    int value;
    auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
    if (ec == std::errc() && ptr == sv.data() + sv.size())
        return value;
    return std::nullopt;
}

bool test_parse_int()
{
    return parse_int("123") == 123 && parse_int("123x") == std::nullopt;
}

int main()
{
    std::cout << (test_parse_int() ? "PASS" : "FAIL") << '\n';
    return 0;
}