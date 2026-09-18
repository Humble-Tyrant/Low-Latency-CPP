#include <iostream>
#include <array>
#include <utility>

/**
 * @brief Selection sort on bid order prices with operational metrics.
 * 
 * Loop Invariant:
 * At the start of outer loop iteration i, the subarray [0, i - 1] contains the 
 * i smallest elements in strictly sorted order. Every remaining element in [i, n - 1] 
 * is greater than or equal to bids[i - 1].
 * 
 * Systems Consideration:
 * Unlike Bubble Sort (which performs up to O(n^2) swaps in the inner loop), 
 * Selection Sort strictly isolates memory writes to the outer loop, guaranteeing 
 * at most n - 1 swaps. In write-sensitive or cache-constrained architectures, 
 * minimizing memory mutation is a key optimization.
 */
int main() {
    std::array<int, 7> bids = {450, 120, 800, 230, 100, 650, 300};

    std::cout << "Unsorted bids: ";
    for (const int bid : bids) {
        std::cout << bid << ' ';
    }
    std::cout << '\n';

    int comparisons = 0;
    int swaps = 0;
    const std::size_t n = bids.size();

    for (std::size_t i = 0; i < n - 1; ++i) {
        std::size_t min_index = i;
        for (std::size_t j = i + 1; j < n; ++j) {
            ++comparisons;
            if (bids[j] < bids[min_index]) {
                min_index = j;
            }
        }
        if (min_index != i) {
            std::swap(bids[i], bids[min_index]);
            ++swaps;
        }
    }

    std::cout << "Sorted bids:   ";
    for (const int bid : bids) {
        std::cout << bid << ' ';
    }
    std::cout << '\n';

    std::cout << "Operational Metrics -> Comparisons: " << comparisons 
              << " | Swaps (Memory Writes): " << swaps << '\n';

    return 0;
}
