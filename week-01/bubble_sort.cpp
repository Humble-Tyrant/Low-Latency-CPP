#include <iostream>
#include <array>
#include <utility>

/**
 * @brief Naive bubble sort on bid price levels.
 * 
 * Loop Invariant:
 * After i outer loop passes, the last i elements are guaranteed
 * to be the largest elements in correct sorted order.
 * 
 * Complexity:
 * - Best Case (Pre-sorted): O(n) via early exit flag
 * - Worst/Average Case: O(n^2)
 */
int main() {
    std::array<int, 7> bids = {450, 120, 800, 230, 100, 650, 300};

    std::cout << "Unsorted bids: ";
    for (const int bid : bids) {
        std::cout << bid << ' ';
    }
    std::cout << '\n';

    const std::size_t n = bids.size();
    for (std::size_t i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (std::size_t j = 0; j < n - 1 - i; ++j) {
            if (bids[j] > bids[j + 1]) {
                std::swap(bids[j], bids[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) {
            break; // Early exit: array is already sorted
        }
    }

    std::cout << "Sorted bids:   ";
    for (const int bid : bids) {
        std::cout << bid << ' ';
    }
    std::cout << '\n';

    return 0;
}
