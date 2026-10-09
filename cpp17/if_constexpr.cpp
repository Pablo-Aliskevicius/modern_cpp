#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>

template <typename T>
void put(std::string &out, const T &v)
{
    if constexpr (std::is_integral_v<T>)
    { // little-endian fixed width
        for (std::size_t i = 0; i < sizeof(T); ++i)
            out.push_back(static_cast<char>((v >> (8 * i)) & 0xFF));
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
        put(out, static_cast<std::uint32_t>(v.size())); // length prefix
        out += v;
    }
    else
    {
        static_assert(sizeof(T) == 0, "unsupported type"); // fires only if instantiated
    }
}

int main()
{
    std::string wire;
    put(wire, std::uint16_t{0x1234});
    put(wire, std::string("hello"));
    std::cout << wire.size() << " bytes\n"; // 2 + 4 + 5 = 11
}