/*

`constexpr` grows up: std::vector, std::string, virtual, try ★★
Dynamic allocation is allowed in constant evaluation as long as everything 
is freed before the evaluation ends. That unlocks ordinary algorithms (sieve, 
sort, parsing) written exactly as at run time.

⚠ Gotcha:  A constexpr std::vector variable (that survives to run time) is 
still an error; allocation must be transient. Return a std::array or a size/flat 
result from the compile-time computation.

*/

#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>
 
constexpr std::size_t count_primes(unsigned n) {
    // n < 256; otherwise char is not enough to hold the sieve.
    std::vector<char> sieve(n + 1, 1);                    // allocated at compile time...
    std::size_t count = 0;
    for (unsigned i = 2; i <= n; ++i) {
        if (!sieve[i]) continue;
        ++count;
        // Start by i * i: e.g. if i==7, then 7*2, 7*3, 7*4, 7*5, 7*6 are already marked by 2,3,2,5,2
        for (unsigned j = i * i; j <= n; j += i) sieve[j] = 0;
    }
    return count;                                         // ...and freed before returning
}
static_assert(count_primes(100) == 25);
 
constexpr std::string shout(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](char c) { return c >= 'a' && c <= 'z' ? char(c - 32) : c; });
    return s + "!";
}
// use the string only inside constant evaluation
static_assert(shout("ok").size() == 3);
 
struct Shape {
    virtual constexpr int sides() const = 0;              // constexpr virtual
    virtual constexpr ~Shape() = default;
};
struct Tri : Shape { constexpr int sides() const override { return 3; } };
constexpr int poly() { Tri t; const Shape& s = t; return s.sides(); }
static_assert(poly() == 3);
 
constexpr int safe_div(int a, int b) {
    // try/catch allowed (throwing is not)
    try { if (b == 0) throw 1; return a / b; } catch (int) { return -1; }
}
 
int main() { return safe_div(4, 2) == 2 ? 0 : 1; }
