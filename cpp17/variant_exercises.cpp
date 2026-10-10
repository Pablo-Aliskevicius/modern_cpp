// std::variant and std::visit ★★★⚡

#include <cstdint>
#include <iostream>
#include <string>
#include <variant>

struct Read
{
    std::uint64_t off;
    std::uint32_t len;
};

struct Write
{
    std::uint64_t off;
    std::string data;
};

struct Flush
{
};

struct Delete
{
    std::string key;
};

using Op = std::variant<Read, Write, Flush, Delete>;

template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
}; // C++17 using-pack

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>; // guide
std::string describe(const Op &op)
{
    return std::visit(overloaded{
                          [](const Read &r)
                          { return "read " + std::to_string(r.off) + "+" + std::to_string(r.len); },
                          [](const Write &w)
                          {
                              return "write " + std::to_string(w.off) + " (" + std::to_string(w.data.size()) + "B)";
                          },
                          [](const Flush &)
                          { return std::string("flush"); },
                          [](const Delete &d) // Need to add this to avoid the (incredibly unhelpful) compile error about an unhandled alternative
                          { return "delete " + d.key; }
                      },
                      op); // compile error if an alternative is unhandled
}

int main()
{
    Op ops[] = {Read{0, 4096}, Write{8, "hello"}, Flush{}, Delete{"key1"}};
    for (const Op &o : ops)
        std::cout << describe(o) << '\n';
    if (auto *w = std::get_if<Write>(&ops[1]))
        std::cout << w->data << '\n';
    std::cout << std::holds_alternative<Flush>(ops[2]) << '\n';
}
