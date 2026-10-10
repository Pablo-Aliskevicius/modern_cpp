/*
# ✎ Exercise 15. #
Write `std::pair<std::string_view, std::string_view> split_once(std::string_view, char)` and use it to parse `"key=value"`.

*/

#include <iostream>
#include <string_view> 

std::pair<std::string_view, std::string_view> split_once(std::string_view s, char sep) {
  auto pos = s.find(sep);
  if (pos == std::string_view::npos)
    return {s, {}};
  return {s.substr(0, pos), s.substr(pos + 1)};
}

bool test_split_once() {
  const auto [key, value] = split_once("key=value", '=');
  return key == "key" && value == "value";
}

int main() {
  if (test_split_once()) {
    std::cout << "All tests passed!\n";
    return 0;
  }
    std::cout << "Some tests failed.\n";
    return 1;
}
