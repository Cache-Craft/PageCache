#include <iostream>
#include "../cache/lru.h"

void testBasicLRU() {

    LRUCache cache(3);

    // First three pages -> MISS
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

    // Page 1 is used again -> HIT
    if (cache.access(1) != true) {
        std::cout << "Test failed: page 1 should be HIT\n";
        return;
    }

    // Cache is full.
    // LRU should remove page 2.
    if (cache.access(4) != false) {
        std::cout << "Test failed: page 4 should be MISS\n";
        return;
    }

    // Page 2 should have been removed.
    if (cache.access(2) != false) {
        std::cout << "Test failed: page 2 should have been evicted\n";
        return;
    }

    std::cout << "Basic LRU test: PASSED\n";
}

int main() {

    std::cout << "===== LRU TESTS =====\n";

    testBasicLRU();

    return 0;
}