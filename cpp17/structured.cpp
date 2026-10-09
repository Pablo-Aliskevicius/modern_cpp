#include <cstdint>
#include <iostream>
#include <map>
#include <string>

struct Extent { std::uint64_t offset; std::uint32_t length; };
Extent find_extent(std::uint64_t block) { return {block * 4096, 4096}; }

int main() {
    auto [off, len] = find_extent(7); // struct -> two names
    std::cout << off << ' ' << len << '\n';
    std::map<std::string, int> refcount{{"obj-a", 2}, {"obj-b", 5}};
    for (auto& [name, rc] : refcount) { // reference into the map's pair
        rc += 1;
        std::cout << name << '=' << rc << '\n';
    }

    auto [it, inserted] = refcount.emplace("obj-c", 1); // pair<iterator,bool>
    std::cout << it->first << ' ' << std::boolalpha << inserted << '\n';
}
