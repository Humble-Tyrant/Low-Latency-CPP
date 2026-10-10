// Tests for MinHeap<T, Container> against two backends:
//   1. std::vector<T>          (growable)
//   2. FixedBuffer<T, N>       (fixed capacity, throws overflow_error when full)
//
// Build:  g++ -std=c++17 -Wall -Wextra -O1 -g -fsanitize=address,undefined main.cpp -o test_min_heap
// Run:    ./test_min_heap

#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#include "fixed_size_buffer.hpp"
#include "min_heap.hpp"

// ---------- tiny test harness ----------
static int g_checks = 0;
static int g_failed = 0;

#define CHECK(cond)                                                              \
    do {                                                                         \
        ++g_checks;                                                              \
        if (!(cond)) {                                                           \
            ++g_failed;                                                          \
            std::cerr << "  FAIL " << __FILE__ << ":" << __LINE__ << "  " #cond  \
                      << "\n";                                                   \
        }                                                                        \
    } while (0)

#define CHECK_THROWS(expr, ExType)                                               \
    do {                                                                         \
        ++g_checks;                                                              \
        bool caught = false;                                                     \
        try { expr; } catch (const ExType&) { caught = true; } catch (...) {}    \
        if (!caught) {                                                           \
            ++g_failed;                                                          \
            std::cerr << "  FAIL " << __FILE__ << ":" << __LINE__                \
                      << "  expected " #ExType " from " #expr "\n";              \
        }                                                                        \
    } while (0)

#define RUN(fn, name)                                                            \
    do {                                                                         \
        int before = g_failed;                                                   \
        fn;                                                                      \
        std::cout << (g_failed == before ? "[PASS] " : "[FAIL] ") << name        \
                  << "\n";                                                       \
    } while (0)

// ---------- backend traits: how to name a heap type for each container ----------
constexpr std::size_t kCap = 1024;  // FixedBuffer capacity used in the tests

template <typename T> struct VectorBackend {
    using Heap = MinHeap<T, std::vector<T>>;
    static constexpr bool bounded = false;
};
template <typename T> struct FixedBackend {
    using Heap = MinHeap<T, FixedBuffer<T, kCap>>;
    static constexpr bool bounded = true;
};

// drain a heap into a vector (smallest first)
template <typename H, typename T = int>
std::vector<T> drain(H& h) {
    std::vector<T> out;
    while (!h.empty()) {
        out.push_back(h.top());
        h.pop();
    }
    return out;
}

// ---------- the suite, templated on backend ----------
template <template <typename> class Backend>
struct Suite {
    using IntHeap = typename Backend<int>::Heap;

    static void empty_state() {
        IntHeap h;
        CHECK(h.empty());
        CHECK(h.size() == 0);
        CHECK_THROWS(h.top(), std::underflow_error);
        CHECK_THROWS(h.pop(), std::underflow_error);
    }

    static void single_element() {
        IntHeap h;
        h.push(42);
        CHECK(!h.empty());
        CHECK(h.size() == 1);
        CHECK(h.top() == 42);
        h.pop();
        CHECK(h.empty());
        CHECK_THROWS(h.top(), std::underflow_error);
    }

    static void top_tracks_minimum() {
        IntHeap h;
        const int in[] = {5, 12, 8, 15, 20, 3, 9};  // 5,12,8,15,20 is the NOTES.md example
        int expected_min = in[0];
        for (int v : in) {
            h.push(v);
            expected_min = std::min(expected_min, v);
            CHECK(h.top() == expected_min);
        }
        CHECK(h.size() == 7);
    }

    static void pops_in_sorted_order() {
        IntHeap h;
        std::vector<int> in = {9, 4, 7, 1, 8, 2, 6, 3, 5, 0};
        for (int v : in) h.push(v);
        auto out = drain(h);
        std::sort(in.begin(), in.end());
        CHECK(out == in);
    }

    static void already_sorted_and_reverse_sorted() {
        {
            IntHeap h;
            for (int i = 0; i < 100; ++i) h.push(i);
            auto out = drain(h);
            CHECK(std::is_sorted(out.begin(), out.end()));
            CHECK(out.size() == 100);
        }
        {
            IntHeap h;
            for (int i = 100; i > 0; --i) h.push(i);
            auto out = drain(h);
            CHECK(std::is_sorted(out.begin(), out.end()));
            CHECK(out.front() == 1 && out.back() == 100);
        }
    }

    static void duplicates() {
        IntHeap h;
        for (int v : {3, 1, 3, 1, 2, 2, 3, 1}) h.push(v);
        auto out = drain(h);
        CHECK((out == std::vector<int>{1, 1, 1, 2, 2, 3, 3, 3}));
    }

    static void all_equal() {
        IntHeap h;
        for (int i = 0; i < 20; ++i) h.push(7);
        CHECK(h.size() == 20);
        auto out = drain(h);
        CHECK(std::all_of(out.begin(), out.end(), [](int v) { return v == 7; }));
    }

    static void negatives_and_extremes() {
        IntHeap h;
        for (int v : {0, -5, 100, -100, 5, INT32_MIN, INT32_MAX, -1}) h.push(v);
        CHECK(h.top() == INT32_MIN);
        auto out = drain(h);
        CHECK(std::is_sorted(out.begin(), out.end()));
        CHECK(out.back() == INT32_MAX);
    }

    static void interleaved_push_pop() {
        IntHeap h;
        h.push(10); h.push(4); h.push(7);
        CHECK(h.top() == 4);
        h.pop();
        CHECK(h.top() == 7);
        h.push(1);
        CHECK(h.top() == 1);
        h.push(8);
        h.pop();                // removes 1
        CHECK(h.top() == 7);
        h.pop();                // removes 7
        CHECK(h.top() == 8);
        h.pop();                // removes 8
        CHECK(h.top() == 10);
        CHECK(h.size() == 1);
    }

    static void size_bookkeeping() {
        IntHeap h;
        for (int i = 0; i < 50; ++i) {
            h.push(i);
            CHECK(h.size() == static_cast<std::size_t>(i + 1));
        }
        for (int i = 49; i >= 0; --i) {
            h.pop();
            CHECK(h.size() == static_cast<std::size_t>(i));
        }
        CHECK(h.empty());
    }

    static void reuse_after_drain() {
        IntHeap h;
        for (int v : {3, 2, 1}) h.push(v);
        drain(h);
        CHECK(h.empty());
        for (int v : {9, 8, 7}) h.push(v);
        CHECK(h.top() == 7);
        CHECK(h.size() == 3);
    }

    static void lvalue_and_rvalue_push() {
        IntHeap h;
        int a = 5;
        h.push(a);              // const T&
        h.push(3);              // T&&
        h.push(std::move(a));   // T&&
        CHECK(h.size() == 3);
        CHECK(h.top() == 3);
    }

    static void strings() {
        using StrHeap = typename Backend<std::string>::Heap;
        StrHeap h;
        for (const char* s : {"pear", "apple", "zebra", "mango", "banana"}) h.push(std::string(s));
        std::string prev = h.top();
        h.pop();
        while (!h.empty()) {
            CHECK(prev <= h.top());
            prev = h.top();
            h.pop();
        }
    }

    // Reference model: compare against std::priority_queue (min-heap) on random op streams.
    static void randomized_vs_priority_queue() {
        std::mt19937 rng(12345);
        std::uniform_int_distribution<int> val(-1000, 1000);
        std::uniform_int_distribution<int> coin(0, 2);  // 2/3 push, 1/3 pop

        for (int round = 0; round < 20; ++round) {
            IntHeap mine;
            std::priority_queue<int, std::vector<int>, std::greater<int>> ref;
            for (int step = 0; step < 800; ++step) {
                bool do_pop = (coin(rng) == 0) && !ref.empty();
                if (do_pop) {
                    CHECK(mine.top() == ref.top());
                    mine.pop();
                    ref.pop();
                } else if (ref.size() < kCap) {   // stay within FixedBuffer capacity
                    int v = val(rng);
                    mine.push(v);
                    ref.push(v);
                }
                CHECK(mine.size() == ref.size());
                if (!ref.empty()) CHECK(mine.top() == ref.top());
            }
            while (!ref.empty()) {
                CHECK(mine.top() == ref.top());
                mine.pop();
                ref.pop();
            }
            CHECK(mine.empty());
        }
    }

    // Capacity behaviour differs by backend.
    static void capacity_behaviour() {
        IntHeap h;
        if constexpr (Backend<int>::bounded) {
            for (std::size_t i = 0; i < kCap; ++i) h.push(static_cast<int>(kCap - i));
            CHECK(h.size() == kCap);
            CHECK(h.top() == 1);
            // Overflow: must throw and leave the heap intact.
            CHECK_THROWS(h.push(-1), std::overflow_error);
            CHECK(h.size() == kCap);
            CHECK(h.top() == 1);
            // After a pop there is room again.
            h.pop();
            h.push(-1);
            CHECK(h.top() == -1);
            CHECK(h.size() == kCap);
        } else {
            for (std::size_t i = 0; i < kCap * 4; ++i) h.push(static_cast<int>(kCap * 4 - i));
            CHECK(h.size() == kCap * 4);   // vector grows past any "capacity"
            CHECK(h.top() == 1);
        }
    }

    static void run_all(const char* tag) {
        std::cout << "=== MinHeap<int, " << tag << "> ===\n";
        RUN(empty_state(),                    "empty state / underflow throws");
        RUN(single_element(),                 "single element");
        RUN(top_tracks_minimum(),             "top() tracks minimum while pushing");
        RUN(pops_in_sorted_order(),           "pops come out sorted");
        RUN(already_sorted_and_reverse_sorted(), "sorted & reverse-sorted input");
        RUN(duplicates(),                     "duplicates");
        RUN(all_equal(),                      "all-equal elements");
        RUN(negatives_and_extremes(),         "negatives & INT_MIN/INT_MAX");
        RUN(interleaved_push_pop(),           "interleaved push/pop");
        RUN(size_bookkeeping(),               "size bookkeeping");
        RUN(reuse_after_drain(),              "reuse after draining");
        RUN(lvalue_and_rvalue_push(),         "lvalue & rvalue push overloads");
        RUN(strings(),                        "std::string elements");
        RUN(randomized_vs_priority_queue(),   "randomized vs std::priority_queue");
        RUN(capacity_behaviour(),             "capacity behaviour");
        std::cout << "\n";
    }
};

int main() {
    Suite<VectorBackend>::run_all("std::vector<int>");
    Suite<FixedBackend>::run_all("FixedBuffer<int, 1024>");

    std::cout << g_checks << " checks, " << g_failed << " failed\n";
    return g_failed == 0 ? 0 : 1;
}