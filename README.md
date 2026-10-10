# Low-Latency C++: Systems & Engineering Log

A structured engineering repository tracking hands-on implementations in modern C++ (C++20), memory models, cache-aware systems, and systems programming fundamentals.

---

## Environment & Toolchain

- **OS:** Linux (Ubuntu 24.04 LTS / WSL2)
- **Compiler:** GCC 13+ / Clang 16+ (`-std=c++20`)
- **Build System:** CMake 3.20+
- **Compiler Flags:** `-Wall -Wextra -Wpedantic -Wconversion -Wshadow`
- **Sanitizers:** AddressSanitizer (ASan), LeakSanitizer (LSan), UndefinedBehaviorSanitizer (UBSan)

---

## Repository Modules

### [Week 01: Memory Bug Lab](week_01/)
An investigation into five fundamental C++ memory defects. Each bug is isolated in a standalone broken baseline, diagnosed via runtime sanitizers (ASan, LSan, UBSan) or static analysis, and resolved using modern C++ RAII and type-safe abstractions.
- **Detailed Postmortem & Traces:** See [`week_01/README.md`](week_01/README.md).

### [Week 02: Utility Library](week_02/)
A progressive implementation of generic containers transitioning from linked-node dynamic storage to allocation-free, cache-conscious data structures.
- **v0:** Singly-linked `Stack<T>` and `Queue<T>` with Rule of 5 move-only semantics.
- **v1:** Stack-allocated `FixedBuffer<T, Capacity>` on `std::array` eliminating heap allocator jitter.
- **v2:** Array-backed `MinHeap<T, Container>` decoupled from storage, cross-verified with 85,659 differential testing checks against `std::priority_queue`.
- **Architectural Overview:** See [`week_02/README.md`](week_02/README.md).

---

## Directory Structure

```text
Low-Latency-CPP/
├── CMakeLists.txt                 # Root build configuration with sanitizer toggles
├── README.md                      # Repository overview and completed modules
├── .gitignore                     # Build artifact exclusions
├── build/                         # Generated CMake build files and test executables
├── week_01/                       # Week 01: Memory Bug Lab
│   ├── README.md                  # Sanitizer diagnostics and postmortem report
│   ├── NOTES.md                   # Systems programming concept log
│   └── memory_bug_lab/            # Broken baselines and RAII refactors
└── week_02/                       # Week 02: Generic Utility Library
    ├── README.md                  # Architecture and implementation overview
    ├── NOTES.md                   # Engineering concept log
    ├── utility_library_v0/        # Linked Stack<T> and Queue<T>
    ├── utility_library_v1/        # Fixed-capacity FixedBuffer<T, Capacity>
    └── utility_library_v2/        # Generic MinHeap<T, Container>
```
