#include "lru.h"

#include <iostream>

LRUCache::LRUCache(int capacity)
    : capacity(capacity), hits(0), misses(0) {
}

bool LRUCache::access(int page) {

    auto it = cache.find(page);

    // Page already exists -> HIT
    if (it != cache.end()) {

        hits++;

        // Remove from its old position
        pages.erase(it->second);

        // Add to the back because it is now
        // the most recently used page
        pages.push_back(page);

        // Update its position in the map
        cache[page] = std::prev(pages.end());

        return true;
    }

    // Page not found -> MISS
    misses++;

    // Cache full -> remove least recently used page
    if (pages.size() >= static_cast<size_t>(capacity)) {

        int oldPage = pages.front();

        pages.pop_front();

        cache.erase(oldPage);
    }

    // Insert new page as most recently used
    pages.push_back(page);

    cache[page] = std::prev(pages.end());

    return false;
}

int LRUCache::getHits() const {
    return hits;
}

int LRUCache::getMisses() const {
    return misses;
}

double LRUCache::getHitRatio() const {

    int total = hits + misses;

    if (total == 0) {
        return 0.0;
    }

    return static_cast<double>(hits) / total;
}

void LRUCache::printCache() const {

    std::cout << "Cache: [ ";

    for (int page : pages) {
        std::cout << page << " ";
    }

    std::cout << "]\n";
}