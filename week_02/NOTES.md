# Week 02 Engineering Log & Concept Notes

### Class & Function Templates
- Concept: Compile-time blueprints instantiated lazily on demand by the compiler; zero runtime overhead.
- Approach: Placed all template definitions in header files (`.hpp`) so the compiler can stamp out specializations at the call site.
- Gotcha: Linker duplicate symbol errors are avoided via weak symbols (COMDAT folding); over-instantiating across types can cause code bloat.
- Complexity: Compile-time instantiation; O(1) runtime dispatch.

### Allocator Latency Jitter vs Fixed Contiguous Storage
- Concept: Heap allocation (`new`) delegates to the OS/glibc allocator, introducing nondeterministic latency spikes on the hot path.
- Approach: Designed `FixedBuffer<T, Capacity>` using `std::array` to achieve 100% stack locality and zero runtime allocations.
- Gotcha: Memory is bounded at compile time; vacated slots in raw arrays require careful destruction semantics when managing owning types (`std::unique_ptr`).
- Complexity: Strict O(1) push/pop, zero heap overhead.

### Complete Binary Heap & Cache Locality
- Concept: Flattening a binary tree into a 1D array eliminates tree-node pointer chasing.
- Approach: Index navigation using arithmetic: `parent(i) = (i - 1) / 2`, `left(i) = 2i + 1`, `right(i) = 2i + 2`.
- Gotcha: Templating on `Container` allows the algorithm to swap between unbounded `std::vector` and bounded `FixedBuffer` without modifying heap logic.
- Complexity: O(log N) push/pop, O(1) top/peek.