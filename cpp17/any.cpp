// Prefer variant when the set of types is known; use any only at plugin boundaries or in heterogeneous property bags.

#include <any>
#include <iostream>
#include <string>
#include <vector>

int main() {
  std::vector<std::any> bag{42, std::string("text"), 3.5};
  for (const auto &a : bag) {
    if (auto *i = std::any_cast<int>(&a))
      std::cout << "int " << *i << '\n';
    else if (auto *s = std::any_cast<std::string>(&a))
      std::cout << "string " << *s << '\n';
    else
      std::cout << a.type().name() << '\n';
  }
  try {
    std::any_cast<double>(bag[0]);
  } catch (const std::bad_any_cast &) {
    std::cout << "wrong type\n";
  }
}