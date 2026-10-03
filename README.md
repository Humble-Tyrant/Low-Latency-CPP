# Low-Latency C++: Systems & Microstructure Intensive

A production-grade engineering repository tracking low-latency systems programming, modern C++ (C++20), memory models, cache-aware architectures, and high-performance trading infrastructure. 

Structured across the 8-week **Quant Roadmap (Phase A)**.

---

## Environment & Toolchain

- **OS:** Linux (Ubuntu 24.04 LTS / WSL2)
- **Compiler:** GCC 13+ / Clang 16+ (Strict `-std=c++20`)
- **Build System:** CMake 3.20+
- **Compiler Flags:** `-Wall -Wextra -Wpedantic -Wconversion -Wshadow`
- **Sanitizers:** AddressSanitizer (ASan), LeakSanitizer (LSan), UndefinedBehaviorSanitizer (UBSan)

---

## Phase A: Systems & Infrastructure Roadmap

| Week | Module / Project | Architecture & Concepts | Status |
| :---: | :--- | :--- | :---: |
| **01** | [**Memory Bug Lab**](week_01/memory_bug_lab/) | Heap UAF, leaks, off-by-one struct corruption, uninitialized stack reads, dangling references, RAII, Rule of 5 | **Completed (ASan Clean)** |
| **02** | **Utility Library** | Generic containers (`Stack<T>`, `Queue<T>`), custom allocators, templated intrusive lists, MinHeap/PriorityQueue | *Upcoming* |
| **03** | **High-Performance File Indexer (Part 1)** | Systems I/O, `mmap()` vs POSIX `read()`, page tables, TLB impact, inverted index | *Upcoming* |
| **04** | **File Indexer (Part 2) & Benchmark** | Large-scale corpus indexing, page faults, virtual memory replacement policies, memory profiling | *Upcoming* |
| **05** | **Concurrent Systems: Races & Locks** | Thread synchronization, mutex contention, condition variables, thread-safe queue, producer-consumer | *Upcoming* |
| **06** | **Lock-Free Concurrency & Ring Buffers** | Atomics (`std::atomic`), memory orderings, cache lines, lock-free SPSC ring buffer vs mutex queue | *Upcoming* |
| **07** | **Performance Lab: Cache & Architecture** | AoS vs SoA cache locality, false sharing, branch prediction impact, hardware performance counters | *Upcoming* |
| **08** | **Market-Data Simulator & Order Book** | UDP multicast feed, sequence-gap detection, Level-2 order book mechanical matching, network latency | *Upcoming* |

---

## Directory Structure

```text
Low-Latency-CPP/
├── CMakeLists.txt              # Root build configuration with sanitizer toggles
├── README.md                   # Portfolio overview and roadmap tracker
├── .gitignore                  # Build artifact exclusions
└── week_01/
    └── memory_bug_lab/         # Week 01 Deliverable
        ├── CMakeLists.txt      # (Included via root CMake)
        ├── README.md           # Sanitizer diagnostic traces & postmortem report
        ├── NOTES.md            # Systems concept logs (No-Paper System)
        ├── bug1_uaf.cpp / bug1_uaf_refactor.cpp
        ├── bug2_leak.cpp / bug2_leak_refactor.cpp
        ├── bug3_overflow.cpp / bug3_overflow_refactor.cpp
        ├── bug4_uninitialized.cpp / bug4_uninitialized_refactor.cpp
        └── bug5_dangling_reference.cpp / bug5_dangling_reference_refactor.cpp