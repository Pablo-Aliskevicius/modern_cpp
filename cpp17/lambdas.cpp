// constexpr lambdas and [*this] capture ★★
#include <functional>
#include <iostream>

constexpr auto kib = [](std::size_t n)
{ return n * 1024; };

static_assert(kib(4) == 4096); // lambdas are implicitly constexpr when possible

struct Worker
{
    int id = 7;
    auto make_task()
    {
        return [*this]
        { return id; };
    } // captures a COPY of the object
    auto make_bad()
    {
        return [this]
        { return id; };
    } // captures the pointer: dangles
};

int main()
{
    std::function<int()> task;
    {
        Worker w;
        task = w.make_task();
    } // w is gone, the task owns its copy
    std::cout << task() << '\n'; // 7
}