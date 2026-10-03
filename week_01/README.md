# Memory Bug Lab — Week 01

Five isolated C++ memory defects reproduced under AddressSanitizer (ASan), LeakSanitizer (LSan) and UndefinedBehaviorSanitizer (UBSan), followed by deterministic RAII and type-safe refactors.

## Build & Reproduction Guide

All targets (the five broken baselines and their `_refactor` versions) are defined in the root `CMakeLists.txt`. It builds with C++20 and strict warnings (`-Wall -Wextra -Wpedantic -Wconversion -Wshadow`). Sanitizers are on by default (`-DENABLE_ASAN=ON` adds `-fsanitize=address,undefined`; LeakSanitizer runs as part of ASan).

```bash
# Configure and build every baseline and refactor
cmake -B build -S . -DENABLE_ASAN=ON
cmake --build build

# Run one target
./build/bug1_uaf
./build/bug1_uaf_refactor
```

A single file can also be compiled by hand:

```bash
g++ -g -fsanitize=address bugN_name.cpp -o bugN_name && ./bugN_name
```

The "Detected with" lines below use this single-file command.

## Summary

| # | Bug | Detected by | Fix |
|---|-----|-------------|-----|
| 1 | Heap use-after-free | ASan | RAII handle with move semantics |
| 2 | Heap memory leak | LSan (part of ASan) | `std::unique_ptr` |
| 3 | Stack buffer overflow (off-by-one) | ASan | `std::array` + whole-array assignment |
| 4 | Uninitialized variable | Code review (ASan missed it) | In-class default initializers |
| 5 | Dangling reference | ASan + compiler warning | Return by value |

## Bug Postmortems

### Bug 1: Heap Use-After-Free

**File:** `bug1_uaf.cpp` → fixed in `bug1_uaf_refactor.cpp`

**Symptom:** AddressSanitizer aborts with `heap-use-after-free` when `main` reads `data->price` after `create_update()` returns.

**Root cause:** `create_update()` calls `delete update;` before `return update;`, handing an already-freed pointer back to the caller.

**Detected with:** `g++ -g -fsanitize=address bug1_uaf.cpp -o bug1_uaf && ./bug1_uaf`

```text
ERROR: AddressSanitizer: heap-use-after-free on address 0x503000000048
READ of size 8 at 0x503000000048 thread T0
    #0 in main bug1_uaf.cpp:18
freed by thread T0 here:
    #0 in operator delete(void*, unsigned long)
    #1 in create_update(double, unsigned int) bug1_uaf.cpp:12
previously allocated by thread T0 here:
    #0 in operator new(unsigned long)
    #1 in create_update(double, unsigned int) bug1_uaf.cpp:11
```

**Fix:** wrapped the allocation in a `MarketUpdateHandle` RAII class following the Rule of Five. Copy operations are `= delete`, the move constructor and move assignment are `noexcept` and null out the moved-from handle, and the destructor calls `delete` exactly once.

---

### Bug 2: Heap Memory Leak

**File:** `bug2_leak.cpp` → fixed in `bug2_leak_refactor.cpp`

**Symptom:** LeakSanitizer reports a 24-byte leak at exit when an order is rejected (`price > 1000.0`).

**Root cause:** `process_order()` allocates an `Order` with `new`, but the early `return` in the rejection branch skips the `delete` at the end of the function.

**Detected with:** `g++ -g -fsanitize=address bug2_leak.cpp -o bug2_leak && ./bug2_leak`

```text
ERROR: LeakSanitizer: detected memory leaks
Direct leak of 24 byte(s) in 1 object(s) allocated from:
    #0 in operator new(unsigned long)
    #1 in process_order(double, unsigned int) bug2_leak.cpp:11
    #2 in main bug2_leak.cpp:23
SUMMARY: AddressSanitizer: 24 byte(s) leaked in 1 allocation(s).
```

**Fix:** replaced raw `new` / `delete` with `std::make_unique<Order>()`, so destruction is bound to scope exit on every return path.

---

### Bug 3: Stack Buffer Overflow (Off-by-One)

**File:** `bug3_overflow.cpp` → fixed in `bug3_overflow_refactor.cpp`

**Symptom:** AddressSanitizer reports an out-of-bounds read inside `update_bids()`.

**Root cause:** the loop uses `i <= count` instead of `i < count`. With `count == 5`, the sixth iteration reads `new_bids[5]` past the end of `bids` and writes `depth->bid_prices[5]`. Because `ask_prices` sits directly after `bid_prices` in the struct, that write silently lands in `ask_prices[0]`. ASan does not flag it, since it stays inside the same object.

**Detected with:** `g++ -g -fsanitize=address bug3_overflow.cpp -o bug3_overflow && ./bug3_overflow`

```text
ERROR: AddressSanitizer: stack-buffer-overflow on address 0x7fb94e600048
READ of size 8 at 0x7fb94e600048 thread T0
    #0 in update_bids(MarketDepth*, double const*, unsigned long) bug3_overflow.cpp:11
    #1 in main bug3_overflow.cpp:20
Address 0x7fb94e600048 is located in stack of thread T0 at offset 72 in frame
  [32, 72) 'bids' (line 18) <== Memory access at offset 72 overflows this variable
```

**Fix:** replaced the C-style arrays with `std::array<double, 5>` and dropped index-based iteration in favor of direct copy assignment (`depth->bid_prices = new_bids;`). The size is part of the type, so there is no loop bound left to get wrong.

---

### Bug 4: Uninitialized Stack Variable

**File:** `bug4_uninitialized.cpp` → fixed in `bug4_uninitialized_refactor.cpp`

**Symptom:** non-deterministic branching in `send_order()`, because `order.side` holds stack garbage. In the recorded run it printed `Rejected! Unknown order side`.

**Root cause:** `Orders order;` default-initializes the struct and leaves every field indeterminate. `order_id`, `price` and `quantity` are written afterwards, but `side` is read without ever being written.

**Detected with:** not caught by the sanitizer. Compiled and run with `g++ -g -fsanitize=address bug4_uninitialized.cpp -o bug4_uninitialized && ./bug4_uninitialized`, which ran without any report. Found by code review.

```text
AddressSanitizer did NOT report this bug. It checks address validity and redzones,
not whether a value was initialized (UBSan does not check this either). Observed output: "Rejected! Unknown order side"
```

**Fix:** added in-class default member initializers (`uint64_t order_id{}; char side{};`) and value-initialized the instance (`Orders order{};`), so every field starts at a known zero value.

---

### Bug 5: Dangling Reference to Stack Frame

**File:** `bug5_dangling_reference.cpp` → fixed in `bug5_dangling_reference_refactor.cpp`

**Symptom:** segmentation fault (SEGV) on a read at address `0x8`.

**Root cause:** `get_default_limits()` returns a `const RiskLimits&` bound to a stack-local object. Once the function returns, its frame is gone and the reference dangles. GCC warns about it and compiles the returned address to a null pointer, so reading `limits.quantity` (offset 8) faults at `0x8`.

**Detected with:** `g++ -g -fsanitize=address bug5_dangling_reference.cpp -o bug5_dangling_reference && ./bug5_dangling_reference`

```text
bug5_dangling_reference.cpp:11:12: warning: reference to local variable 'limits' returned [-Wreturn-local-addr]
ERROR: AddressSanitizer: SEGV on unknown address 0x000000000008
The signal is caused by a READ memory access.
Hint: address points to the zero page.
    #0 in main bug5_dangling_reference.cpp:17
```

**Fix:** changed the return type to a value (`RiskLimits`). Guaranteed copy elision (C++17) constructs the returned temporary directly in the caller's variable, with no copy cost.