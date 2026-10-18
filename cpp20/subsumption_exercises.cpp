#include <concepts>
#include <cstddef>
#include <iostream>
#include <span>
 
/*
&&/|| in constraints give a partial order: the more constrained overload wins automatically. 
Use it to express capability hierarchies (a readable device ⊂ a read/write device) without tag types.

⚠ Gotcha:  Subsumption only works when you compose named concepts with &&/||. 
Repeating the same requires(...) text in two overloads makes them unrelated and the call ambiguous.

# ✎ Exercise 25. #
Add `Seekable` and a use overload for `ReadWritable` && `Seekable`. Verify the most specific version wins.
*/

template <typename T>
concept Readable = requires(T t, std::span<std::byte> b) {
    { t.read(b) } -> std::same_as<std::size_t>;
};
 
template <typename T>
concept ReadWritable = Readable<T> && requires(T t, std::span<const std::byte> b) {
    { t.write(b) } -> std::same_as<std::size_t>;
};

template <typename T>
concept Seekable = ReadWritable<T> && requires(T t, std::span<const std::byte> b) {
    { t.seek(b) } -> std::same_as<std::size_t>;
};


struct RoDevice { std::size_t read(std::span<std::byte> b)        { return b.size(); } };
struct RwDevice : RoDevice { std::size_t write(std::span<const std::byte> b) { return b.size(); } };
struct RwsDevice : RwDevice { std::size_t seek(std::span<const std::byte> b) { return b.size(); } };
 
void use(Readable auto&)    { std::cout << "read-only path\n"; }
void use(ReadWritable auto&) { std::cout << "read/write path\n"; }   // more constrained: preferred
void use(Seekable auto&) { std::cout << "seekable path\n"; }   // most constrained: preferred
 
template <typename T>
struct Stack {
    T pop() requires std::default_initializable<T> { return T{}; }    // member constrained individually
};
 
int main() {
    RoDevice a; RwDevice b; RwsDevice c;
    use(a);          // read-only path
    use(b);          // read/write path (no ambiguity thanks to subsumption)
    use(c);          // seekable path (most specific)
}
