/*

constexpr may run at compile time; consteval must (immediate function). 
consteval is only for functions, not variables. 

constinit guarantees a variable is initialized statically — it removes 
the static-initialization-order fiasco — but, unlike constexpr, stays mutable.

*/

#include <array>
#include <cstdint>
#include <iostream>
#include <string_view>
 
consteval std::array<std::uint32_t, 256> make_crc_table() {
    std::array<std::uint32_t, 256> t{};
    for (std::uint32_t i = 0; i < 256; ++i) {
        std::uint32_t c = i;
        for (int k = 0; k < 8; ++k) c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1;
        t[i] = c;
    }
    return t;
}
constexpr auto kTable = make_crc_table();               // built by the compiler, lives in .rodata
 
constexpr std::uint32_t crc32(std::string_view s) {
    std::uint32_t crc = 0xFFFFFFFFu;
    for (unsigned char ch : s) crc = kTable[(crc ^ ch) & 0xFF] ^ (crc >> 8);
    return ~crc;
}
static_assert(crc32("123456789") == 0xCBF43926u);       // checked at compile time
 
constinit thread_local std::uint64_t t_ops = 0;          // no dynamic-init guard on every access
 
consteval int checked_size(int n) { return n > 0 ? n : throw "size must be positive"; }

/*
✎ Exercise 29.  
Generate at compile time a std::array<std::uint8_t,256> popcount table with consteval, and static_assert three entries.

Popcount (short for population count), also known as the Hamming weight or sideways sum, 
is an operation that counts the number of set bits (1s) in a binary value
*/

consteval std::array<std::uint8_t,256> popcount() {
    std::array<std::uint8_t,256> t{};
    for (std::uint32_t i = 0; i < 256; ++i) {
        std::uint8_t c = 0;
        for (int k = 0; k < 8; ++k) c += (i >> k) & 1;
        t[i] = c;
    }
    return t;
}

constexpr auto kPopcountTable = popcount();

static_assert(kPopcountTable[0] == 0);
static_assert(kPopcountTable[1] == 1);
static_assert(kPopcountTable[255] == 8);

// end of exercise 29

int main() {
    ++t_ops;
    std::cout << std::hex << crc32("hello") << ' ' << std::dec << t_ops << '\n';
    std::array<int, checked_size(8)> buf{};
    std::cout << buf.size() << '\n';
    // std::array<int, checked_size(-1)> bad;        // compile error: expression is not a constant
}
