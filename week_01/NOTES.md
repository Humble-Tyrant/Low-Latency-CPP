# Memory Bug Lab — Concept Notes

  

## ASan (`-fsanitize=address`)

  

- AddressSanitizer is a runtime instrumentation tool (supported by both GCC and Clang) that detects out-of-bounds memory access, use-after-free, and similar memory-safety violations as they happen.

- `-fsanitize=address` is what instruments the binary and triggers the detection — this alone is sufficient to catch the bug.

- `-g` is compiled in alongside it only to add debug symbols, so the crash report points to readable file/line locations instead of raw memory addresses. Without `-g`, ASan still catches the violation; the report is just harder to read.

- No sanitizer flags → no runtime checks, and the program may run "normally" while silently corrupting memory or leaking it.

  

## Explicit Ownership

  

- A raw pointer carries no information about ownership. `int* p = create();` tells you `p` points at an object `create()` returned, but not *who is responsible* for freeing it.

- This ambiguity is the root cause of leaks, double-frees, and use-after-free bugs — multiple owners may assume someone else will clean up, or no one does.

- RAII resolves this by binding ownership to a scope rather than leaving it implicit.

  

## RAII (Resource Acquisition Is Initialization)

  

- An object's lifetime is bound to its enclosing scope (`{ ... }`) for stack-allocated objects.

- Manual memory management is the direct cause of leaks, use-after-free, and double-free bugs — RAII removes the need for manual `delete` calls entirely.

- Guarantees deterministic cleanup: as long as an object is on the stack, it's alive; when its function's stack frame is popped, destructors run automatically, in reverse order of construction.

- This determinism is why RAII is the default discipline in low-latency / HFT systems — cleanup timing is predictable, not left to a garbage collector.

  

## `struct` vs `class`

  

- `struct`: appropriate for a passive data container (DTO) with no invariants to protect.

- `class`: appropriate when a type manages a resource or enforces an internal invariant.

- The only language-level difference: members and base classes default to `public` in a `struct`, `private` in a `class`.

  

## The Rule of Five

  

If a class explicitly manages a resource and requires a custom destructor, all five special member functions should be explicitly defined or deleted:

  

1. Destructor — `~T()`

2. Copy constructor — `T(const T& other)`

3. Copy assignment operator — `T& operator=(const T& other)`

4. Move constructor — `T(T&& other) noexcept`

5. Move assignment operator — `T& operator=(T&& other) noexcept`

  

`noexcept` is a promise to the compiler that the function will not throw — move constructors/assignments should generally be `noexcept` so standard containers can safely use them during reallocation.

  

## Uninitialized Stack Memory

  

- A local variable declared on the stack is **not** zero-initialized by default in C++.

- Its initial value is whatever bits were left behind by the previous function that used that stack address.

- Reading an uninitialized variable is undefined behavior (UB) — it may appear to "work" and still be wrong.

  

**Mitigation:**

- Supply sensible default values directly in struct definitions.

- Prefer aggregate/value initialization (`T obj{};`) over leaving a stack variable declared but unset.

  

## Dangling References

  

- A reference is an alias to an existing object's memory — it has no independent storage.

- Returning a reference to a local (an rvalue bound to a function-local object) gives the caller a reference to memory that no longer exists once the function returns: a **dangling reference**.

- Reading through it accesses a dead stack frame. If another function is called immediately after, its new frame can silently overwrite that exact memory address, corrupting the data without any visible crash.

  

## Copy Elision: RVO / NRVO

  

- RVO (Return Value Optimization) and NRVO (Named RVO) apply specifically to **returning by value**. The compiler constructs the returned object directly in the caller's destination slot, skipping any copy or move entirely.

  - RVO: an unnamed temporary is returned directly, e.g. `return RiskLimits{...};` — copy elision here is *guaranteed* since C++17.

  - NRVO: a named local is returned, e.g. `RiskLimits r{...}; return r;` — elision is permitted but not guaranteed by the standard; most modern compilers perform it anyway.

- This is unrelated to returning by reference or pointer. Returning a reference never constructs or elides anything — it just hands back an alias, which is exactly the mechanism that produces a dangling reference if the referent doesn't outlive the caller.

- Separately, for small objects (roughly ≤16 bytes, platform-dependent), the ABI can return the value directly in registers (e.g. `rax`/`rdx` on x86-64), avoiding a memory write entirely. Larger objects are typically returned via a hidden pointer to caller-allocated storage. This register-vs-memory choice is independent of RVO/NRVO — it's about how a by-value return is physically transmitted, not whether a copy was elided.