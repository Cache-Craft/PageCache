#include <iostream>

#include "../cache/fifo.h"

int main() {

    FIFOCache cache(3);

    int pages[] = {1, 2, 3, 1, 4, 2, 5};

    int totalPages = sizeof(pages) / sizeof(pages[0]);

    for (int i = 0; i < totalPages; i++) {

        int page = pages[i];

        bool hit = cache.access(page);

        std::cout << "Accessing page " << page;

        if (hit) {
            std::cout << " -> HIT\n";
        } else {
            std::cout << " -> MISS\n";
        }

        cache.printCache();
    }

    std::cout << "\n===== FIFO RESULTS =====\n";

    std::cout << "Hits: "
              << cache.getHits() << "\n";

    std::cout << "Misses: "
              << cache.getMisses() << "\n";

    std::cout << "Hit Ratio: "
              << cache.getHitRatio() << "\n";

    return 0;
}