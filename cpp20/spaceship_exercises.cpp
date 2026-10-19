#include <compare>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>


/*
# ✎ Exercise 26. #

Give `struct Key { std::string tenant; std::uint64_t id; }` a defaulted `<=>`, 
then use it as the key of `std::map` and in `std::sort`.
*/

struct Key { 
    std::string tenant; 
    std::uint64_t id; 
    auto operator<=>(const Key&) const = default;
};

/*
# ✎ Exercise 27. # 

Which of `<`, `==` is synthesized for `struct S { int x; auto operator<=>(const S&) const { return x <=> 0; } };`?  
Test it.
*/

struct S { 
    int x; 
    auto operator<=>(const S&) const { return x <=> 0; } 
};

int main() {
    Key k1{"tenant1", 1}, k2{"tenant1", 2};
    std::map<Key, std::string> m{{k1, "value1"}, {k2, "value2"}};
    for (const auto& [key, value] : m) {
        std::cout << key.tenant << " " << key.id << ": " << value << '\n';
    }

    S s1{5}, s2{-3};
    // Test synthesized operators
    std::cout << (s1 < s2) << '\n'; 

    std::vector<Key> keys{k2, k1, {"tenant2", 0}, {"tenant1", 0}};
    std::sort(keys.begin(), keys.end());
    for (const auto& key : keys) {
        std::cout << key.tenant << " " << key.id << '\n';
    }

    // Custom <=> does not synthesize ==, so this will not compile if uncommented
    // error C2676: binary '==': 'S' does not define this operator or a conversion to a 
    // type acceptable to the predefined operator
    // std::cout << (s1 == s2) << '\n'; 

    return 0;
}