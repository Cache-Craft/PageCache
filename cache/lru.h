#ifndef LRU_H
#define LRU_H

#include <list>
#include <unordered_map>

class LRUCache {
private:
    int capacity;
    int hits;
    int misses;

    // Front = least recently used
    // Back  = most recently used
    std::list<int> pages;

    // Page -> position in the list
    std::unordered_map<int, std::list<int>::iterator> cache;

public:
    LRUCache(int capacity);

    bool access(int page);

    int getHits() const;
    int getMisses() const;
    double getHitRatio() const;

    void printCache() const;
};

#endif