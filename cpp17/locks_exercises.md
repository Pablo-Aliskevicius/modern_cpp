# ✎ Exercise 20. #
Add a `try_get_or_insert(k, producer)` to `Cache` that takes the shared lock first and upgrades to exclusive only on a miss. What race must you handle after upgrading?  
  
_Hint:_ Re-check the key after taking the exclusive lock (another writer may have inserted it).