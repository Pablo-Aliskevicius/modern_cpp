#include <iostream>
#include <map>
#include <mutex>

/*
# ✎ Exercise 3. #
Rewrite a loop `auto it = m.find(k); if (it != m.end()) {...}` so it doesn't leak.  
Then guard a `std::map::insert` with a lock in one statement.
*/


int main() {
    std::map<std::string, int> m{{"a", 1}};
    if (auto it = m.find("a"); it != m.end()) {
        std::cout << "Found: " << it->second << '\n';
    } else {
        std::cout << "Not found\n";
    }

    std::mutex mu;
    if (std::lock_guard lk{mu}; !m.empty()) {
        auto [it, inserted] = m.emplace("b", 2);
        std::cout << "Inserted: " << it->first << ", Success: " << std::boolalpha << inserted << '\n';
    }

    return 0;
}