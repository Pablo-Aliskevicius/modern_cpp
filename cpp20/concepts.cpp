#include <concepts> 
#include <cstddef> 
#include <iostream> 
#include <string> 
#include <vector> 

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

static_assert(ByteContainer<std::string>); 
static_assert(ByteContainer<std::vector<char>>); 
static_assert(!ByteContainer<int>); 

int main() { 
	std::cout << checksum(std::string("abc")) << ' ' << align_up(4097u, 4096u) << ' ' 
		<< half(3.0) << '\n'; 
	show(7); 
	show(2.5); 
	// align_up(1.5, 2.0); // error: 'double' does not satisfy 'integral' (one clear line) 
}