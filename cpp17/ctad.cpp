// Class template argument deduction (CTAD) ★★★

#include <array>
#include <iterator>
#include <mutex>
#include <utility>
#include <vector>

template <typename T>
struct Range
{
    template <typename It>
    Range(It b, It e) : v(b, e) {}
    std::vector<T> v;
};

// Deduction guide: tells the compiler how to get T from the constructor arguments.
template <typename It>
Range(It, It) -> Range<typename std::iterator_traits<It>::value_type>;

int main()
{
    std::pair p{1, 2.5};    // pair<int,double>
    std::vector v{1, 2, 3}; // vector<int>
    std::array a{1, 2, 3};  // array<int,3>
    std::mutex m;
    std::scoped_lock lk{m}; // scoped_lock<std::mutex>
    std::vector<long> src{4, 5, 6};

    Range r{src.begin(), src.end()}; // Range<long> thanks to the guide

    return r.v.size() == 3 && v.size() == 3 && a.size() == 3 ? 0 : 1;
}