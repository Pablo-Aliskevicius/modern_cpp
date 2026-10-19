/*

✎ Exercise 30.  
Write a constexpr function that splits "a,b,c" into a std::vector<std::string> 
and returns its size; static_assert it equals 3.

*/

#include <string>
#include <vector>
#include <iostream>

constexpr std::size_t count_commas(const std::string& s) {
    std::size_t count = 0;
    for (char c : s) {
        if (c == ',') {
            ++count;
        }
    }
    return count + 1; // number of elements is number of commas + 1
}

constexpr std::size_t split_and_count(const std::string& s) {
    std::vector<std::string> result;
    std::size_t start = 0;
    for (std::size_t i = 0; i <= s.size(); ++i) {
        if (i == s.size() || s[i] == ',') {
            result.push_back(s.substr(start, i - start));
            start = i + 1;
        }
    }
    return result.size();
}

static_assert(split_and_count("a,b,c") == 3);
static_assert(split_and_count("one,two,three,four") == 4);
static_assert(split_and_count("") == 1); // empty string is one element
static_assert(split_and_count("single") == count_commas("single") ); // single element
static_assert(split_and_count("a,b,c") == count_commas("a,b,c") ); // check against count_commas

int main() {
    std::cout << "Size of split: " << split_and_count("a,b,c") << '\n';
    return 0;
}