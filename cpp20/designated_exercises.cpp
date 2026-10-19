/*
# ✎ Exercise 28. #

Create an aggregate `Endpoint { std::string host; int port = 80; bool tls = false; }` and a function taking it; call it three ways.

*/
#include <string>
#include <iostream>

struct Endpoint {
    std::string host; // no default, must be provided
    int port = 80;
    bool tls = false;
};

void connect(const Endpoint& endpoint) {
    std::cout << "Connecting to " << endpoint.host << ":" << endpoint.port << " (TLS: " << endpoint.tls << ")\n";
}

int main() {
    connect({"example.com"}); // all defaults
    connect({.host = "example.org", .port = 443, .tls = true}); // specify all fields
    connect({.host = "example.net", .tls = true}); // specify some fields, rest defaulted
    return 0;
}