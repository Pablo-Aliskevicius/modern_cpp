#include <cstddef>
#include <iostream>
 
struct Options {
    std::size_t block   = 4096;
    bool        sync    = false;
    int         retries = 3;
    const char* name    = "default";
};
 
void open(const Options& o) {
    std::cout << o.name << ' ' << o.block << ' ' << o.sync << ' ' << o.retries << '\n';
}


// Python-style designated initializers are supported in C++20, 
// but the order of the fields must follow the declaration order.
int main() {
    open({});                                 // all defaults
    open({.block = 8192, .sync = true});      // pick what you need, rest defaulted
    open({.retries = 5, .name = "journal"});  // order must follow declaration order
    // open({.sync = true, .block = 1});      // error: out of order
}
