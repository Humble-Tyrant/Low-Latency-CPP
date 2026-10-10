# MinHeap — v2

**Status: new data structure, built on v0/v1.** An array-backed binary min-heap, templated over its backing container, verified with an automated test suite (85,659 checks, 0 failures) including differential testing against `std::priority_queue`.

See `NOTES.md` for the heap invariant, the HFT motivation, and the array-indexing scheme.

---

## API

| Method | Behavior |
|---|---|
| `push(const T&)` / `push(T&&)` | Insert, then sift up to restore the heap invariant |
| `pop()` | Swap root with the last element, shrink, sift down; throws `std::underflow_error` if empty |
| `top()` | Peek at the minimum element (O(1)); throws if empty |
| `size()` / `empty()` | State queries |

`MinHeap<T, Container = std::vector<T>>` is templated on its backing container, defaulting to `std::vector<T>` but not requiring it.

---

## Design Decisions

**Why templated on `Container`, not hardcoded to `std::vector`:** the heap logic (sift-up, sift-down, index math) doesn't care how its storage grows or where it lives — it only needs `operator[]`, `push_back`/`pop_back`, and `size()`. Templating on `Container` keeps that logic decoupled from the storage policy, and lets the exact same heap code run over a growable `std::vector<T>` or a fixed-capacity, allocation-free `FixedBuffer<T, N>` (v1) with no changes.

**Proof this isn't just a theoretical decoupling:** the test suite runs the entire test matrix twice — once with `MinHeap<T, std::vector<T>>`, once with `MinHeap<T, FixedBuffer<T, 1024>>` — including a capacity-aware test (`capacity_behaviour`) that checks overflow is thrown correctly for the bounded backend and that the heap keeps working correctly right up to and after that limit. This is what makes v1 *reusable infrastructure* rather than a one-off exercise.

---

## Testing

`main.cpp` is an **automated test suite** (not a manual print-and-eyeball harness like v0/v1) — a `CHECK`/`CHECK_THROWS` macro records every assertion against a running pass/fail count, and the suite runs end-to-end, templated, against both backends:

- Empty-state behavior and underflow on `top()`/`pop()`
- Single element, interleaved push/pop sequences
- `top()` tracks the minimum correctly through a mixed push sequence
- Full drain comes out in sorted order (verified against `std::sort` on the same input)
- Sorted input, reverse-sorted input, duplicates, all-equal elements
- `INT32_MIN`/`INT32_MAX` and negative values
- `std::string` elements (non-trivial, non-numeric comparison)
- Size bookkeeping through growth and drain
- Reuse after a full drain
- Lvalue and rvalue push overloads
- **Differential testing:** 20 rounds × 800 randomized push/pop operations per round, checked at every step against `std::priority_queue<int, std::vector<int>, std::greater<int>>` as a reference implementation — `top()` and `size()` must match the reference after every single operation, not just at the end
- Capacity behavior specific to the bounded backend: overflow throws and leaves the heap unchanged, and a pop followed by a push correctly reopens space

**Result:** 85,659 checks, 0 failures, compiled under `-Wall -Wextra -Wpedantic -Wconversion -Wshadow -fsanitize=address,undefined` with no warnings and no sanitizer reports.