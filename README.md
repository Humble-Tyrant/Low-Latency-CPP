# Low-Latency C++: Systems & Microstructure Intensive

A structured repository tracking progressive development in modern C++ (C++20), memory models, cache optimization, and high-performance financial systems engineering.

## Environment & Toolchain
- **OS:** Ubuntu (WSL2)
- **Compiler:** GCC / Clang (C++20 standard)
- **Build System:** CMake 3.20+
- **Flags:** `-Wall -Wextra -Wpedantic -Wconversion -Wshadow`

## Progression & Modules
- **`week-01/` — Core Fundamentals, Invariants & Sorting Baselines**
  - `bubble_sort.cpp`: Naive sorting baseline with loop invariant analysis and early-exit branch optimization.
  - `selection_sort.cpp`: Operational metrics instrumentation comparing algorithmic comparison count vs. memory write traffic (swaps).
  - `tick_invariant_check.cpp`: Market data tick stream validator enforcing price continuity and filtering null/corrupted packets.
- **`week-02/`** *(Upcoming)*: Memory alignment, cache lines, RAII, and move semantics.

## Building Targets
```bash
cmake -B build
cmake --build build

