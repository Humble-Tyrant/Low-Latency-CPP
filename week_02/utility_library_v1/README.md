# Fixed-Size Buffer — v1

**Status: performance revision of v0.** `FixedBuffer<T, Capacity>` is a stack-allocated, fixed-capacity container backed by `std::array<T, Capacity>` — zero heap allocation, no pointer-chasing, full cache locality.

---

## Why This Exists

v0's `Stack<T>`/`Queue<T>` are correct but allocate a new heap `Node<T>` on every `push()`. Two costs follow from that:

- **Allocation latency:** `new` calls into the OS/allocator on every insert, introducing jitter that's unacceptable on a hot path in a low-latency system.
- **Cache misses:** heap nodes are scattered across RAM; walking a linked list means chasing pointers, and each jump is a potential L1/L2 cache miss.

`FixedBuffer<T, Capacity>` trades flexibility (fixed, compile-time capacity instead of unbounded growth) for predictable, contiguous, allocation-free storage — the standard tradeoff in latency-sensitive systems where a known upper bound on size is acceptable.

---

## API

| Method | Behavior |
|---|---|
| `push(const T&)` / `push(T&&)` | Copy or move an element in; throws `std::overflow_error` if full |
| `pop()` | Removes the last element; throws `std::underflow_error` if empty |
| `front()` / `back()` | Access first/last element (const and non-const overloads); throws if empty |
| `operator[](idx)` | Unchecked access |
| `at(idx)` | Bounds-checked access (checked against current **size**, not capacity); throws `std::out_of_range` |
| `clear()` | Resets size to 0 |
| `size()` / `capacity()` / `empty()` / `full()` | State queries |
| `push_back()` / `pop_back()` | Aliases for `push()`/`pop()`, so the type is interoperable with code written against standard-container naming |

All operations are O(1). No allocation ever occurs after the `FixedBuffer` itself is constructed.

---

## Design Decisions

**Why `std::array<T, Capacity>` instead of a raw array:** `std::array` gives bounds-safe size tracking, value semantics (copy/move the whole buffer correctly if ever needed), and integrates with the standard library, at zero runtime cost over a C-style array.

**Why throw instead of silently failing on overflow:** `push()` on a full buffer and `pop()`/`front()`/`back()` on an empty one throw rather than returning a sentinel or asserting. This keeps failure handling explicit and testable — the test suite verifies every error path with `CHECK_THROWS`-style assertions, including that a **failed** push leaves the buffer's state completely unchanged (size, front, back all unaffected).

**Copy vs. move push:** both `push(const T&)` and `push(T&&)` are provided, same pattern as v0's `Stack`/`Queue`. Tested explicitly with `std::unique_ptr<int>` elements to confirm move-only types are handled correctly and never silently copied.

---

## Known Limitation

`pop()` and `clear()` decrement `size_` but do not reset the vacated slot(s). For trivial types (`int`, etc.) this is invisible. For an owning type like `std::unique_ptr<T>`, the object in a "popped" slot stays alive — referenced by a stale element in the backing array — until that slot is overwritten by a future `push()` or the whole buffer is destroyed. Its destructor does not run at the logical pop point.
---

## Testing

`main.cpp` covers, under `-Wall -Wextra -Wpedantic -Wconversion -Wshadow -fsanitize=address,undefined`, with zero warnings and zero sanitizer reports:

- Push/pop/front/back correctness, including in-place mutation through references
- Overflow on push (`std::overflow_error`) and underflow on pop/front/back (`std::underflow_error`), including on `const` overloads, with state verified unchanged after a failed operation
- Move-only element type (`std::unique_ptr<int>`) — push via `std::move`, move out via `std::move(buffer.front())`, verified no copy occurs (same pointer address before and after)
- `at()` bounds-checking against current size (not capacity) vs. unchecked `operator[]`
- `clear()` and reuse afterward
- 1,000-cycle fill/drain stress loop
- Capacity-1 edge case

---

## Used By

`MinHeap<T, Container>` (v2) is tested against `FixedBuffer<T, 1024>` as an alternative backend to `std::vector<T>`, confirming this type is reusable general-purpose infrastructure rather than a one-off exercise.