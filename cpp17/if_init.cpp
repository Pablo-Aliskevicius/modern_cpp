#include <iostream>
#include <map>
#include <mutex>
#include <string>

std::map<std::string, int> cache{{"a", 1}};
std::mutex mu;

int main() {
    if (auto it = cache.find("a"); it != cache.end()) {
        std::cout << "hit " << it->second << '\n';
    } else {
        std::cout << "miss\n"; // 'it' is still in scope here
    }
    if (std::lock_guard lk{mu}; !cache.empty()) { // lock held for the whole if/else
        std::cout << cache.size() << " entries\n";
    }
    switch (auto n = cache.size(); n) {
        case 0: std::cout << "empty\n"; break;
        default: std::cout << "non-empty\n";
    }
    return 0;
}