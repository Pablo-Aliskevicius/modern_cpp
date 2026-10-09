# ✎ Exercise 1. #  
Write `std::tuple<bool, int, std::string> try_parse(std::string_view)` and consume it with a
structured binding. Then use one on a `std::array<int,3>`.
_Hint:_ Arrays, tuple-likes and structs with all-public non-static members all work; the number of names must match exactly.

# ✎ Exercise 2. # 
Iterate a `std::map<std::string, std::vector<int>>` printing each key and the vector size using only structured bindings.
