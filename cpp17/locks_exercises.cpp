// std::scoped_lock, std::shared_mutex ★★★⚡

/*
✎ Exercise 20. 
Add a try_get_or_insert(k, producer) to Cache that takes the
shared lock first and upgrades to exclusive only on a miss. 
What race must you handle after upgrading? 
Hint: 
Re-check the key after taking the exclusive lock
(another writer may have inserted it).

*/

#include <functional>
#include <iostream>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

struct Account {
  std::mutex m;
  long balance = 100;
};

void transfer(Account &a, Account &b, long x) {
  std::scoped_lock lk{a.m, b.m}; // deadlock-free for any order of arguments
  a.balance -= x;
  b.balance += x;
}

class Cache {
  mutable std::shared_mutex m_;
  std::unordered_map<std::string, int> map_;

public:
  std::optional<int> get(const std::string &k) const {
    std::shared_lock lk{m_}; // many readers in parallel
    if (auto it = map_.find(k); it != map_.end())
      return it->second;
    return std::nullopt;
  }

  std::optional<int> try_get_or_insert(const std::string &k, const std::function<int()> &producer) {
    auto return_value = get(k); // try to get the value with shared lock
    if (return_value)
      return return_value; // value found, return it

    // Upgrade to exclusive lock for writing
    std::unique_lock lk{m_};
    // Re-check the key after taking the exclusive lock
    if (auto it = map_.find(k); it != map_.end())
      return it->second; // another writer may have inserted it
    int value = producer(); // produce the value
    map_[k] = value; // insert the new value
    return value;
  }
  
  void put(const std::string &k, int v) {
    std::unique_lock lk{m_}; // exclusive writer
    map_[k] = v;
  }
};

int main() {
  Account a, b;
  std::vector<std::thread> ts;
  for (int i = 0; i < 4; ++i)
    ts.emplace_back([&, i] {
      for (int j = 0; j < 1000; ++j)
        (i % 2 ? transfer(a, b, 1) : transfer(b, a, 1));
    });
  for (auto &t : ts)
    t.join();
  std::cout << a.balance << " + " << b.balance << " = " << a.balance + b.balance
            << '\n'; // 200 always
  Cache c;
  c.put("x", 1);
  std::cout << c.get("x").value_or(-1) << '\n';
}