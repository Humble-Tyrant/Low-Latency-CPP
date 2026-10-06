# Utility Library v0 — Week 02

A generic, templated `Stack<T>` and `Queue<T>` built on a shared singly-linked `Node<T>`, with move-only container semantics and a test suite covering correctness, edge cases, and move behavior.

---

## Design Decisions

### Why `explicit` constructors on `Node<T>`

`Node`'s two constructors (lvalue-copy and rvalue-move) are marked `explicit` to prevent the compiler from using them for implicit conversions — e.g. accidentally constructing a `Node<T>` where a `T` was expected, or vice versa, without that conversion being visible at the call site. Since `Node` is an internal implementation detail used only by `Stack`/`Queue` via direct `new Node<T>{...}` calls, this costs nothing and removes a class of silent-conversion bugs.

### Why copy is deleted, not just omitted

Both `Stack` and `Queue` explicitly `delete` their copy constructor and copy assignment operator rather than leaving them to be implicitly generated (which would be wrong — a naive compiler-generated copy would shallow-copy the `head_`/`tail_` pointers, causing a double-free the moment both copies are destroyed). Deleting them makes the container **move-only** by design: copying would require a deep traversal-and-clone of the whole list, which isn't needed for this exercise and is easy to add later if required. Move construction and move assignment are implemented explicitly, stealing the pointers and leaving the source in a valid, empty, reusable state.

### Ownership model

Each container owns its chain of `Node<T>*` exclusively via raw pointers, with `delete` calls confined to the destructor, `pop()`, and the "clear existing" step inside move assignment. No node pointer is ever exposed outside the container, so there's a single, well-defined owner at all times — the same discipline from Week 1's RAII notes, just applied without `unique_ptr`, to practice manual linked-list ownership directly.

---

## Test Coverage

`main.cpp` is a manual test harness (no CTest integration yet — see Week 01 for the CTest pattern this could adopt later) covering:

- **Move construction and move assignment**, for both `Stack` and `Queue`, including:
  - the moved-from container left empty and safely reusable afterward
  - move-assigning into a non-empty container (old contents must be freed, not leaked)
  - move-assigning from/into an empty container
  - self-move-assignment (via an aliased reference, to avoid the compiler's `-Wself-move` diagnostic while still exercising the `this != &other` guard)
  - move-only element types (`std::unique_ptr<int>`), which fail to compile if `push`/`pop` ever accidentally tried to copy an element
- **Scale**: 10,000 sequential pushes/pops on both containers, verifying size, ordering, and that the destructor's iterative cleanup doesn't rely on recursion (which would risk a stack overflow at this depth).
- **`Queue::tail_` reset at the empty boundary** — specifically, draining a queue to exactly 0 elements and confirming a subsequent `push()` doesn't write through a stale `tail_` pointer. Includes a 1,000-cycle drain/refill stress loop and single-element boundary cases (1 → 0 → 1).

All tests pass under `g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -fsanitize=address,undefined`, with no ASan/UBSan reports.

## Known Follow-ups

- `-Wreorder` flagged `Queue`'s move constructor initializer list — member initialization runs in declaration order regardless of how the list is written. Fixed by reordering to match declaration order (`head_`, `tail_`, `size_`).
- No `clear()` method — the interface is intentionally minimal for this exercise; emptying a container currently means popping in a loop.
- No copy support (see Design Decisions above) — a deep-copy implementation is a reasonable future addition if a use case needs it.