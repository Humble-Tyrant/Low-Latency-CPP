# Utility Library — Week 02

A progression of generic, modern C++ (C++20) data structures transitioning from standard dynamic allocation to cache-conscious, allocation-free architectures.

---

## Architectural Progression

```text
v0: Linked Structures (Stack<T>, Queue<T>)
    └── Heap-allocated per node; pointer-chasing; baseline correctness & Rule of 5
v1: Contiguous Storage (FixedBuffer<T, Capacity>)
    └── Zero heap allocation; std::array backing; L1/L2 cache locality; bounded capacity
v2: Algorithmic Decoupling (MinHeap<T, Container>)
    └── Binary min-heap parameterized on storage policy; differential testing (85k+ checks)
```