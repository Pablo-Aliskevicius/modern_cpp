# ✎ Exercise 5. #
Write `template<class... Ts> bool any_of_equal(const T& x, Ts... ts)` returning whether `x` equals any ts using `||` fold.

# ✎ Exercise 6. #
Write a variadic `join(sep, args...)` that prints values separated by sep with no trailing separator.
_Hint:_ Use a lambda with a `bool` first flag, called from a comma fold.