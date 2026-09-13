#include "fifo.h"

#include <iostream>

FIFOCache::FIFOCache(int capacity)
    : capacity(capacity), hits(0), misses(0) {
}

bool FIFOCache::access(int page) {

    // Page already exists in cache
    if (cache.find(page) != cache.end()) {
        hits++;
        return true;
    }

    // Page not found
    misses++;

    // Cache is full
    if (cache.size() >= static_cast<size_t>(capacity)) {

        int oldPage = pages.front();

        pages.pop();

        cache.erase(oldPage);
    }

    // Add new page
    pages.push(page);
    cache.insert(page);

    return false;
}

int FIFOCache::getHits() const {
    return hits;
}

int FIFOCache::getMisses() const {
    return misses;
}

double FIFOCache::getHitRatio() const {

    int total = hits + misses;

    if (total == 0) {
        return 0.0;
    }

    return static_cast<double>(hits) / total;
}

void FIFOCache::printCache() const {

    std::queue<int> temp = pages;

    std::cout << "Cache: [ ";

    while (!temp.empty()) {

        std::cout << temp.front() << " ";

        temp.pop();
    }

    std::cout << "]\n";
}