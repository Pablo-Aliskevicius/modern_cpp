// std::from_chars / std::to_chars ★★★⚡

#include <charconv>
#include <iostream>
#include <optional>
#include <string_view>

std::optional<int> parse_int(std::string_view s, int base = 10) {
  int v{};
  const char *first = s.data();
  const char *last = first + s.size();
  auto [ptr, ec] = std::from_chars(first, last, v, base);
  if (ec != std::errc{} || ptr != last)
    return std::nullopt; // error or trailing junk
  return v;
}

int main() {
  std::cout << parse_int("1234").value_or(-1) << ' '
            << parse_int("12x").value_or(-1) << ' '
            << parse_int("ff", 16).value_or(-1) << '\n';
  char buf[64];
  auto [end, ec] =
      std::to_chars(buf, buf + sizeof buf, 3.14159); // shortest round-trip form
  std::cout << std::string_view(buf, end - buf) << '\n';
  double d{};
  std::from_chars("2.5e3", "2.5e3" + 5, d);
  std::cout << d << '\n';
}