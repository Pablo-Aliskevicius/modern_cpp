// Polymorphic allocators (std::pmr) ★★⚡

// An allocator type that is the same for every container, backed by a
// memory_resource you choose at run time : arena, pool, or counting.

// Exercise 19.
// Make Counting also record peak bytes
// Compare std::pmr::vector<int> growth against std::vector<int> with reserve.

#include <array>
#include <cstddef>
#include <iostream>
#include <memory_resource>
#include <string>
#include <vector>

struct Counting : std::pmr::memory_resource {
  std::pmr::memory_resource *up = std::pmr::new_delete_resource();
  std::size_t allocs = 0;
  std::size_t live_bytes = 0;
  std::size_t peak_bytes = 0;
  void *do_allocate(std::size_t b, std::size_t a) override {
    void *p = up->allocate(b, a);
    ++allocs;
    live_bytes += b;
    if (live_bytes > peak_bytes)
      peak_bytes = live_bytes;
    std::cout << "allocating " << b << " bytes, alignment " << a
              << ", total allocations: " << allocs
              << ", live bytes: " << live_bytes
              << ", peak bytes: " << peak_bytes << '\n';
    return p;
  }
  void do_deallocate(void *p, std::size_t b, std::size_t a) override {
    live_bytes -= b;
    std::cout << "deallocating " << b << " bytes, alignment " << a
              << ", total allocations: " << allocs
              << ", live bytes: " << live_bytes << '\n';
    up->deallocate(p, b, a);
  }
  bool do_is_equal(const std::pmr::memory_resource &o) const noexcept override {
    return this == &o;
  }
};

int main() {
  Counting heap;
  // Scoped arena: stack buffer, no upstream allocations
  {
    alignas(std::max_align_t) std::array<std::byte, 8192> stack;
    // bump allocator
    std::pmr::monotonic_buffer_resource arena{stack.data(), stack.size(), &heap};
    std::pmr::vector<std::pmr::string> names{&arena}; // elements inherit &arena automatically
    for (int i = 0; i < 20; ++i)
      names.emplace_back(
          "a name long enough to defeat the small-string optimisation #" +
          std::to_string(i));
    std::cout << names.size() << " names, upstream allocations: " << heap.allocs << '\n'; // 0
  } // everything freed at once

  std::cout << "creating pool\n"; // 0
  std::pmr::unsynchronized_pool_resource pool{&heap}; // size-class pools, single-thread
  std::pmr::vector<int> v{&pool};
  std::cout << "assigning 1000 elements\n"; // 0
  v.assign(1000, 1);
  std::cout << "pool allocations: " << heap.allocs << '\n';

  constexpr int count = 1000;
  Counting growth_counter;
  {
    std::pmr::vector<int> growing_values{&growth_counter};
    for (int i = 0; i < count; ++i)
      growing_values.push_back(i);
    std::cout << "PMR vector growth: " << growth_counter.allocs
              << " allocations, " << growth_counter.peak_bytes
              << " peak bytes, capacity " << growing_values.capacity() << '\n';
  }

  std::vector<int> reserved_values;
  reserved_values.reserve(count);
  std::size_t capacity_changes = 0;
  for (int i = 0; i < count; ++i) {
    const auto old_capacity = reserved_values.capacity();
    reserved_values.push_back(i);
    if (reserved_values.capacity() != old_capacity)
      ++capacity_changes;
  }
  std::cout << "std::vector with reserve: " << capacity_changes
            << " capacity changes during insertion, capacity "
            << reserved_values.capacity() << '\n';
}

