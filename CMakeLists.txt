cmake_minimum_required(VERSION 3.20)
project(LowLatencyCPP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Strict Compiler Warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()

# Sanitizer Configuration
option(ENABLE_ASAN "Enable AddressSanitizer and UndefinedBehaviorSanitizer" ON)
if(ENABLE_ASAN AND NOT MSVC)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address,undefined)
endif()

# ==============================================================================
# Week 01: Memory Bug Lab — Broken Baselines
# ==============================================================================
set(LAB_DIR "week_01/memory_bug_lab")

add_executable(bug1_uaf                   ${LAB_DIR}/bug1_uaf.cpp)
add_executable(bug2_leak                  ${LAB_DIR}/bug2_leak.cpp)
add_executable(bug3_overflow              ${LAB_DIR}/bug3_overflow.cpp)
add_executable(bug4_uninitialized         ${LAB_DIR}/bug4_uninitialized.cpp)
add_executable(bug5_dangling_reference    ${LAB_DIR}/bug5_dangling_reference.cpp)

# ==============================================================================
# Week 01: Memory Bug Lab — RAII Refactors
# ==============================================================================
add_executable(bug1_uaf_refactor                ${LAB_DIR}/bug1_uaf_refactor.cpp)
add_executable(bug2_leak_refactor               ${LAB_DIR}/bug2_leak_refactor.cpp)
add_executable(bug3_overflow_refactor           ${LAB_DIR}/bug3_overflow_refactor.cpp)
add_executable(bug4_uninitialized_refactor      ${LAB_DIR}/bug4_uninitialized_refactor.cpp)
add_executable(bug5_dangling_reference_refactor ${LAB_DIR}/bug5_dangling_reference_refactor.cpp)

# ==============================================================================
# Week 02: Utility Library Targets
# ==============================================================================
set(WK2_DIR "week_02")

add_executable(utility_v0_tests ${WK2_DIR}/utility_library_v0/main.cpp)
target_include_directories(utility_v0_tests PRIVATE ${WK2_DIR}/utility_library_v0)

add_executable(utility_v1_tests ${WK2_DIR}/utility_library_v1/main.cpp)
target_include_directories(utility_v1_tests PRIVATE ${WK2_DIR}/utility_library_v1)

add_executable(utility_v2_tests ${WK2_DIR}/utility_library_v2/main.cpp)
target_include_directories(utility_v2_tests PRIVATE ${WK2_DIR}/utility_library_v2)