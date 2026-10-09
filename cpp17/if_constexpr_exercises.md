# ✎ Exercise 4. #
Add a `std::vector<T>` branch to put (count, then each element recursively). 
Check the byte count for {1,2,3} of uint8_t.  
  
_Hint:_ `is_same_v` won't match a vector; write a small trait `is_vector<T>` or use a helper overload.