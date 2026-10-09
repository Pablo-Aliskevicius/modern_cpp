# ✎ Exercise 3. #
Rewrite a loop `auto it = m.find(k); if (it != m.end()) {...}` so it doesn't leak.  
Then guard a `std::map::insert` with a lock in one statement.
