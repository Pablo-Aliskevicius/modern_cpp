# ✎ Exercise 23. # 
Write concept `Hashable = requires(T a){ { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>; };` and a `template<Hashable K> size_t bucket(const K&, size_t n)`. 
_Hint_: Test `with std::string`, `int` and a type without a hash — read the diagnostic.

# ✎ Exercise 24. # 
Add a second show overload constrained on `std::ranges::range` printing the elements. 
Why is there no ambiguity with a `std::string`? 

_Hint_: `std::string` is not integral or floating — only the range overload applies.