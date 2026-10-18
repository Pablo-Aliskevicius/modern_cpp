#include <concepts> 
#include <cstddef> 
#include <iostream> 
#include <string> 
#include <vector> 
#include <ranges>
//#include <string_literals>

template <typename T> 
concept ByteContainer = requires(const T& c) { 
	{ c.data() } -> std::convertible_to<const char*>; // expression must be valid AND convert 
	{ c.size() } -> std::convertible_to<std::size_t>; 
}; 

std::size_t checksum(const ByteContainer auto& c) { // abbreviated template syntax 
	std::size_t s = 0; 
	for (std::size_t i = 0; i < c.size(); ++i) 
		s += static_cast<unsigned char>(c.data()[i]); 
	return s; 
} 

template <std::integral T> T align_up(T v, T a) { return (v + a - 1) / a * a; } 
template <typename T> requires std::floating_point<T> // requires-clause form 
T half(T x) { return x / 2; } 

void show(std::integral auto x) { std::cout << "int " << x << '\n'; } 
void show(std::floating_point auto x) { std::cout << "fp " << x << '\n'; } // overload by constraint 
void show(std::ranges::range auto&& r) { 
	std::cout << "range of size " << std::ranges::size(r) << '\n'
	<< "    elements: ";
	for (const auto& e : r)
		std::cout << e << ' ';
	std::cout << '\n';
}

static_assert(ByteContainer<std::string>); 
static_assert(ByteContainer<std::vector<char>>); 
static_assert(!ByteContainer<int>); 

template <typename T>
concept Hashable = requires(const T& t) { 
	{ std::hash<T>{}(t) } -> std::convertible_to<std::size_t>; 
};

template<Hashable K> size_t bucket(const K&, size_t n) { return std::hash<K>{}(K{}) % n; }

struct Point { 
	int x, y; 
};

int main() { 
	using namespace std::string_literals;

	std::cout << checksum(std::string("abc")) << ' ' << align_up(4097u, 4096u) << ' ' 
		<< half(3.0) << '\n'; 
	show(7); 
	show(2.5); 
	// align_up(1.5, 2.0); // error: 'double' does not satisfy 'integral' (one clear line) 

	show(bucket(std::string("abc"), 1024));
	show(bucket(42, 10));
	// show(bucket(Point{3, 4}, 10)); // 'bucket': no matching overloaded function found

	auto v = std::vector<int>{1, 2, 3, 4, 5};
	show(v);
	show(std::ranges::subrange{v.rbegin(), v.rend()}); // range of reverse iterators
	show("hello"s); // range of const char

}