#include <iostream>
#include <vector>

#include "../cache/fifo.h"
#include "../cache/lru.h"
#include "../workload/workload_generator.h"

void runFIFO(const std::vector<int>& workload) {
    FIFOCache fifo(3);

    for (int page : workload) {
        fifo.access(page);
    }

    std::cout << "\n===== FIFO RESULTS =====\n";
    std::cout << "Hits       : " << fifo.getHits() << "\n";
    std::cout << "Misses     : " << fifo.getMisses() << "\n";
    std::cout << "Hit Ratio  : " << fifo.getHitRatio() << "\n";
}

void runLRU(const std::vector<int>& workload) {
    LRUCache lru(3);

    for (int page : workload) {
        lru.access(page);
    }

    std::cout << "\n===== LRU RESULTS =====\n";
    std::cout << "Hits       : " << lru.getHits() << "\n";
    std::cout << "Misses     : " << lru.getMisses() << "\n";
    std::cout << "Hit Ratio  : " << lru.getHitRatio() << "\n";
}

int main() {
    int count = 20;
    int maxPage = 5;

    std::cout << "===== PAGECACHE WORKLOAD EXPERIMENT =====\n";
    std::cout << "Accesses   : " << count << "\n";
    std::cout << "Page Range : 1-" << maxPage << "\n";

    auto sequential = WorkloadGenerator::generateSequential(count);
    auto random = WorkloadGenerator::generateRandom(count, maxPage);
    auto mixed = WorkloadGenerator::generateMixed(count, maxPage);

    std::cout << "\n----- SEQUENTIAL WORKLOAD -----\n";
    runFIFO(sequential);
    runLRU(sequential);

    std::cout << "\n----- RANDOM WORKLOAD -----\n";
    runFIFO(random);
    runLRU(random);

    std::cout << "\n----- MIXED WORKLOAD -----\n";
    runFIFO(mixed);
    runLRU(mixed);

    return 0;
}