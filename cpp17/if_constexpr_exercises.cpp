/*
# ✎ Exercise 4. #
Add a `std::vector<T>` branch to put (count, then each element recursively). 
Check the byte count for {1,2,3} of uint8_t.  
  
_Hint:_ `is_same_v` won't match a vector; write a small trait `is_vector<T>` or use a helper overload.
*/

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

template <typename T>
struct is_vector : std::false_type {}; 

template <typename T, typename A>
struct is_vector<std::vector<T, A>> : std::true_type {};


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
    else if constexpr (is_vector<T>::value)
    {
        put(out, static_cast<std::uint32_t>(v.size())); // count prefix
        for (const auto &elem : v)
        {
            put(out, elem);
        }
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
    auto vec = std::vector<std::uint8_t>{1, 2, 3};
    std::cout << "Is vector: " << std::boolalpha << is_vector<decltype(vec)>::value << '\n';
    put(wire, std::vector<std::uint8_t>{1, 2, 3});

    std::cout << wire.size() << " bytes\n"; // 2 + 4 + 5 + (1 x 6)= 17
    std::cout 
    << wire[10] << ' ' 
    << (int)wire[11] << ' ' 
    << (int)wire[13] << ' ' 
    << (int)wire[14] << ' '
    << (int)wire[15] << ' '
    << (int)wire[16] << ' '
    << (int)wire[17] << ' '
    << '\n';
}