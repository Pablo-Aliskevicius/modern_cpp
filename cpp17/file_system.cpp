// std::filesystem ★★★⚡

/*
⚠ Gotcha: fs::path is not a string: path / "abs" with an absolute right operand
replaces the left. Use .string() for APIs that take const char*.
*/

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main() {
  fs::path root = fs::temp_directory_path() / "cpp_demo";
  fs::create_directories(root / "a" / "b");
  std::ofstream(root / "a" / "x.log") << "hello";
  std::ofstream(root / "a" / "b" / "y.dat") << "world!";
  for (const auto &e : fs::recursive_directory_iterator(root)) {
    std::cout << e.path().lexically_relative(root).string();
    if (e.is_regular_file())
      std::cout << " " << e.file_size() << "B ext=" << e.path().extension();
    std::cout << '\n';
  }
  std::error_code ec; // non-throwing overloads
  fs::path nonexist = root / "nope";
  std::cout << fs::exists(nonexist, ec) << ' ' << ec.value() << '\n';
  auto removed = fs::remove_all(root, ec);
  std::cout << removed << " removed\n";
}