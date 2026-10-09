# ✎ Exercise 7. #
Write `template<class T> struct Wrapper { T value; };` and check that `Wrapper w{42};` deduces
without a guide (aggregate CTAD needs C++20 — what does C++17 need?).

_Hint:_ In C++17 aggregates need an explicit guide: `template<class T> Wrapper(T) -> Wrapper<T>;`

