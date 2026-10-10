# ✎ Exercise 11. #
Use a `constexpr` lambda to build a `std::array<int, 5>` of squares at compile time (std::array operator[] is constexpr-writable in C++17).  

_Hint:_ Define `constexpr auto make = []{ std::array<int,5> a{}; for (int i = 0; i < 5; ++i) a[i] = i*i; return a; };` and `static_assert(make()[4] == 16);`