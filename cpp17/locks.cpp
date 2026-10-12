// std::scoped_lock, std::shared_mutex ★★★⚡

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