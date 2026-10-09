#include <iostream>

struct [[nodiscard]] Status
{
    int code;
}; // every function returning Status is nodiscard

[[nodiscard]] int open_file(const char *) { return 3; }

Status do_io() { return {0}; }

void f([[maybe_unused]] int only_in_debug)
{
    [[maybe_unused]] int unused = 5;
}

int classify(int x)
{
    int r = 0;
    switch (x)
    {
    case 1:
        r += 10;
        [[fallthrough]]; // deliberate: no warning
    case 3:
        r += 100;
        // break; // ask for a warning if you forget to break
    case 2:
        r += 1;
        break;
    default:
        break;
    }
    return r;
}

int main()
{
    (void) open_file("y"); // disable warning (this is the "exercise" part)
    open_file("x"); // warning: ignoring return value
    do_io();        // warning: ignoring Status
    f(1);
    std::cout << classify(1) << '\n';
}