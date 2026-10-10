// std::string_view ★★★⚡

/*
⚠ Gotcha: A string_view is not null-terminated and does not own anything. Don't store one that points into a
std::string that may be modified/destroyed, and don't pass sv.data() to C APIs expecting \0.
*/

#include <iostream>
// #include <string>
#include <string_view>
#include <vector>

constexpr std::string_view kScheme = "s3://";

std::vector<std::string_view> split(std::string_view s, char sep) {
  std::vector<std::string_view> out;
  while (!s.empty()) {
    auto pos = s.find(sep);
    out.push_back(s.substr(0, pos)); // no allocation, no copy
    if (pos == std::string_view::npos)
      break;
    s.remove_prefix(pos + 1);
  }
  return out;
}

int main() {
  std::string_view url = "s3://bucket/dir/key.txt";
  if (url.substr(0, kScheme.size()) == kScheme)
    url.remove_prefix(kScheme.size());
  for (auto part : split(url, '/'))
    std::cout << '[' << part << ']';
  std::cout << '\n';
  // DANGER: view of a temporary
  // std::string_view bad = std::string("temp"); // dangling after the semicolon
}