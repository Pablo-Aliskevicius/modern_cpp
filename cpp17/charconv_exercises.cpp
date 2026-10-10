/*
✎ Exercise 17.
Write `std::string to_hex(uint64_t)` using `to_chars(..., 16);` check round-trip with parse_int.
*/
#include <charconv>
#include <cstdint>
#include <iostream>
#include <optional>
#include <string>  

std::string to_hex(uint64_t value) {
    char buf[17]; // 16 hex digits + null terminator
    auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), static_cast<unsigned long long>(value), 16);
    if (ec != std::errc{}) {
        throw std::runtime_error("Failed to convert to hex");
    }
    return std::string(buf, ptr - buf);
}

std::optional<uint64_t> parse_int(const std::string& text, int base) {
    uint64_t value;
    auto [ptr, ec] = std::from_chars(text.data(), text.data() + text.size(), value, base);
    if (ec != std::errc{} || ptr != text.data() + text.size()) {
        return std::nullopt;
    }
    return value;
}

bool test_to_hex() {
    uint64_t value = 1234567890123456789ULL;
    std::string hex_str = to_hex(value);
    auto parsed_value_opt = parse_int(hex_str, 16);
    if (!parsed_value_opt.has_value()) {
        std::cerr << "Failed to parse hex string: " << hex_str << '\n';
        return false;
    }
    return parsed_value_opt.value() == value;
}

int main() {
    if (test_to_hex()) {
        std::cout << "All tests passed!\n";
        return 0;
    } else {
        std::cout << "Some tests failed.\n";
        return 1;
    }
}