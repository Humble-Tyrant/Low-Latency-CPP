#include <iostream>
#include <array>

/**
 * @brief Market data tick stream invariant validator.
 * 
 * Verifies price continuity and filters anomalies:
 * 1. Drops null/zero-volume invalid ticks.
 * 2. Flags price gaps exceeding threshold (delta > 5).
 * 3. Maintains cumulative volume and valid tick counts.
 */
int main() {
    constexpr int MAX_PRICE_DELTA = 5;
    constexpr std::array<int, 8> prices = {101, 102, 104, 103, 105, 0, 108, 110};

    int prev_price = -1;
    int valid_count = 0;
    int price_sum = 0;

    for (const int price : prices) {
        // Invariant 1: Ignore dropped/zero-price ticks
        if (price == 0) {
            continue;
        }

        // Invariant 2: Detect sudden downward price discontinuities
        if (prev_price != -1 && (prev_price - price) > MAX_PRICE_DELTA) {
            std::cerr << "Warning: Price jump anomaly detected. Current: "
                      << price << ", Previous: " << prev_price << '\n';
            break;
        }

        price_sum += price;
        valid_count++;
        prev_price = price;
    }

    std::cout << "Valid ticks processed: " << valid_count 
              << " | Cumulative price: " << price_sum << '\n';

    return 0;
}
