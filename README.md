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

- **Defects Covered:**
  - Heap Use-After-Free (UAF) & explicit ownership semantics
  - Heap memory leak across conditional exit branches
  - Stack buffer overflow & silent adjacent struct member corruption
  - Uninitialized stack variables & sanitizer detection boundaries
  - Dangling references to expired stack frames & copy elision (RVO)
- **Patterns Applied:** Custom RAII handles, Rule of Five (`noexcept` move contracts, deleted copy operations), `std::unique_ptr`, `std::array` aggregate assignment, value semantics.
- **Detailed Postmortem & Traces:** See [`week_01/README.md`](week_01/README.md).
- **Engineering Concept Log:** See [`week_01/NOTES.md`](week_01/NOTES.md).

---

## Directory Structure

```text
Low-Latency-CPP/
├── CMakeLists.txt              # Root build configuration with sanitizer toggles
├── README.md                   # Repository overview
├── .gitignore                  # Build artifact exclusions
└── week_01/                    # Memory Bug Lab
    ├── README.md               # Sanitizer diagnostic traces & postmortem report
    ├── NOTES.md                # Systems concept logs
    └── memory_bug_lab/         # Implementations (broken baselines & RAII refactors)
