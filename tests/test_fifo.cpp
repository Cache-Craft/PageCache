#include <iostream>
#include "../cache/fifo.h"

void testBasicFIFO() {

    FIFOCache cache(3);

    // First three pages should be misses
    if (cache.access(1) != false) {
        std::cout << "Test failed: page 1 should be MISS\n";
        return;
    }

    if (cache.access(2) != false) {
        std::cout << "Test failed: page 2 should be MISS\n";
        return;
    }

    if (cache.access(3) != false) {
        std::cout << "Test failed: page 3 should be MISS\n";
        return;
    }

    // Page 1 is already present
    if (cache.access(1) != true) {
        std::cout << "Test failed: page 1 should be HIT\n";
        return;
    }

    // Cache is full.
    // FIFO should remove page 1.
    if (cache.access(4) != false) {
        std::cout << "Test failed: page 4 should be MISS\n";
        return;
    }

    // Page 1 should have been removed.
    if (cache.access(1) != false) {
        std::cout << "Test failed: page 1 should have been evicted\n";
        return;
    }

    std::cout << "Basic FIFO test: PASSED\n";
}

int main() {

    std::cout << "===== FIFO TESTS =====\n";

    testBasicFIFO();

    return 0;
}