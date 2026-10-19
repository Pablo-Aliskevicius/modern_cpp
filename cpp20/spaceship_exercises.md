# ✎ Exercise 26. #

Give `struct Key { std::string tenant; std::uint64_t id; }` a defaulted `<=>`, then use it as the key of `std::map` and in `std::sort`.

# ✎ Exercise 27. # 

Which of `<`, `==` is synthesized for `struct S { int x; auto operator<=>(const S&) const { return x <=> 0; } };`?  
Test it.
