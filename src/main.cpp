#include <iostream>
#include <vector>

#include "../cache/fifo.h"
#include "../cache/lru.h"
#include "../workload/workload_generator.h"
#include "../metrics/metrics_collector.h"
void runFIFO(const std::vector<int>& workload) {
    FIFOCache fifo(3);

    for (int page : workload) {
        fifo.access(page);
    }

    std::cout << "\n===== FIFO RESULTS =====\n";

    MetricsCollector metrics(
        static_cast<int>(workload.size()),
        fifo.getHits(),
        fifo.getMisses()
    );

    metrics.printMetrics();
}
void runLRU(const std::vector<int>& workload) {
    LRUCache lru(3);

    for (int page : workload) {
        lru.access(page);
    }

    std::cout << "\n===== LRU RESULTS =====\n";

    MetricsCollector metrics(
        static_cast<int>(workload.size()),
        lru.getHits(),
        lru.getMisses()
    );

    metrics.printMetrics();
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