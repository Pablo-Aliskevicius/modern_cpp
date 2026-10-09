#include <iostream>
#include <utility>

/*
Apply an operator across a parameter pack. Four forms: unary left/right and binary left/right: 
(args + ...), (... + args), (init + ... + args), (args + ... + init).
*/

template <typename... Ts>
auto sum(Ts... ts) { return (ts + ... + 0); }

template <typename... Ts>
bool all_positive(Ts... ts) { return ((ts > 0) && ...); }

template <typename... Ts>
void print_all(const Ts &...ts)
{
    ((std::cout << ts << ' '), ...); // comma fold: do for each, in order
    std::cout << '\n';
}

template <typename F, typename... Ts>
void for_each_arg(F f, Ts &&...ts)
{
    (f(std::forward<Ts>(ts)), ...);
}

int main()
{
    std::cout << sum(1, 2, 3) << ' ' << all_positive(1, 2, -3) << '\n'; // 6 0
    print_all("a", 1, 2.5);
    for_each_arg([](auto v)
                 { std::cout << '[' << v << ']'; }, 1, "x", 3.0);
    std::cout << '\n';
}