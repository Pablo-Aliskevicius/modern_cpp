// std::optional ★★★⚡

/**
 * Gotcha: optional<T&> isn't allowed in C++17–23 (use a pointer, or std::reference_wrapper); 
 * `*opt` on an empty optional is UB while .value() throws.
 *
 */

 #include <iostream>
#include <map>
#include <optional>
#include <string>

std::optional<std::string> lookup(const std::map<std::string, std::string> &m, const std::string &k)
{
    if (auto it = m.find(k); it != m.end())
        return it->second;
    return std::nullopt;
}

int main()
{
    std::map<std::string, std::string> cfg{{"mode", "fast"}};
    if (auto v = lookup(cfg, "mode"))
        std::cout << *v << '\n'; // contextual bool + deref
    std::cout << lookup(cfg, "x").value_or("<none>") << '\n';
    std::optional<int> n; // empty
    n.emplace(5);
    n = std::nullopt; // reset
    try
    {
        n.value();
    }
    catch (const std::bad_optional_access &)
    {
        std::cout << "empty\n";
    }
}