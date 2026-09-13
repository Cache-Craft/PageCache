#ifndef FIFO_H
#define FIFO_H

#include <queue>
#include <unordered_set>

class FIFOCache {
private:
    int capacity;
    int hits;
    int misses;

    std::queue<int> pages;
    std::unordered_set<int> cache;

public:
    FIFOCache(int capacity);

    bool access(int page);

    int getHits() const;
    int getMisses() const;
    double getHitRatio() const;

    void printCache() const;
};

#endif