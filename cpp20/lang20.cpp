/*

6.8 Language grab-bag ★★

Small features worth knowing: 
using enum, 
init-statement in range-for, 
[[likely]], [[no_unique_address]], [[nodiscard("reason")]], 
class-type template parameters (e.g. string literal template parameters: we only had integers and such before), 
explicit(bool):
* explicit(true) behaves identically to the traditional explicit keyword, preventing 
  implicit conversions and copy-initialization.
* explicit(false) makes the constructor or operator implicit, allowing implicit conversions.
  Before C++20, wrapper types in generic programming—such as std::pair or std::optional 
  required writing multiple overloads of the exact same constructor using std::enable_if_t 
  to conditionally include or exclude the explicit.
parenthesised aggregate initialization, (instead of {})
char8_t -> UTF-8 code units

*/

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
 
enum class State { idle, busy, done };
 
std::string to_str(State s) {
    switch (s) {
        using enum State;                                  // no State:: prefix inside
        case idle: return "idle";
        case busy: return "busy";
        case done: return "done";
    }
    return "?";
}
 
/*
✎ Exercise 32.  
Replace to_str with a constexpr std::string_view version 
and add static_assert(to_sv(State::done) == "done");.
*/

constexpr std::string_view to_sv(State s) {
    switch (s) {
        using enum State;                                  // no State:: prefix inside
        case idle: return "idle";
        case busy: return "busy";
        case done: return "done";
    }
    return "?";
}

static_assert(to_sv(State::done) == "done");

// End of exercise 32

struct Empty {}; // size zero
struct Compact { [[no_unique_address]] Empty e; int x; }; // no padding for empty member
// static_assert(sizeof(Compact) == sizeof(int)); // empty member should cost nothing: true for g++, false for MSVC

// Old version of the standard: empty member costs space
struct Fat { Empty e; int x; }; // padding for empty member
static_assert(sizeof(Fat) > sizeof(int));                 // empty member costs space

 
struct Agg { int a; int b; };
 
// structural class type -> usable as template argument
template <std::size_t N> struct FixedStr {
    char v[N];
    constexpr FixedStr(const char (&s)[N]) { std::copy_n(s, N, v); }
};
template <FixedStr S> struct Tag { static constexpr std::string_view name = S.v; };
 
template <typename T> struct Box {
    T v;
    template <typename U> explicit(!std::is_convertible_v<U, T>) Box(U u) : v(u) {}
};
 
[[nodiscard("the descriptor leaks if ignored")]] int open_fd() { return 3; }
 
int main() {
    std::cout << to_str(State::busy) << '\n';
 
    std::vector<int> v{1, 2, 3};
    for (auto n = v.size(); auto& x : v) x *= static_cast<int>(n);   // init-statement in range-for
 
    if (v[0] > 100) [[unlikely]] std::cout << "rare path\n";
 
    auto p = std::make_unique<Agg>(1, 2);                   // parenthesised aggregate init
    std::cout << p->a + p->b << ' ' << Tag<"hello">::name << '\n';
 
    Box<long> b = 5;                                        // int -> long convertible: implicit allowed
    std::cout << b.v << '\n';

    Box<int> b2 = 3.14;                                    // double -> int: implicit allowed
    Box<double> b3 = 3;                                    // int -> double: implicit allowed
 
    int fd = open_fd();
    // open_fd(); // warning C4858: discarding return value: the descriptor leaks if ignored
    std::u8string_view u = u8"héllo";                  // char8_t strings are distinct from char
    std::cout << fd << ' ' << u.size() << '\n';             // 3 6 (UTF-8 bytes)
}
