cmake_minimum_required(VERSION 3.20)
project(LowLatencyCPP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()

# Week 01 Targets
add_executable(bubble_sort week-01/bubble_sort.cpp)
add_executable(selection_sort week-01/selection_sort.cpp)
add_executable(tick_invariant_check week-01/tick_invariant_check.cpp)
