#include <iostream>
#include <vector>

#include "../cache/fifo.h"
#include "../cache/lru.h"
#include "../file/file_reader.h"

void runFIFO(const std::vector<int>& trace) {
    FIFOCache fifo(3);

    for (int page : trace) {
        fifo.access(page);
    }

    std::cout << "\n===== FIFO RESULTS =====\n";
    std::cout << "Hits       : " << fifo.getHits() << "\n";
    std::cout << "Misses     : " << fifo.getMisses() << "\n";
    std::cout << "Hit Ratio  : " << fifo.getHitRatio() << "\n";
}

void runLRU(const std::vector<int>& trace) {
    LRUCache lru(3);

    for (int page : trace) {
        lru.access(page);
    }

    std::cout << "\n===== LRU RESULTS =====\n";
    std::cout << "Hits       : " << lru.getHits() << "\n";
    std::cout << "Misses     : " << lru.getMisses() << "\n";
    std::cout << "Hit Ratio  : " << lru.getHitRatio() << "\n";
}

int main() {
    try {
        std::vector<int> trace =
            FileReader::readAccessTrace("data/sample_trace.txt");

        std::cout << "===== PAGECACHE FILE TRACE =====\n";
        std::cout << "Total Accesses : " << trace.size() << "\n";

        runFIFO(trace);
        runLRU(trace);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}