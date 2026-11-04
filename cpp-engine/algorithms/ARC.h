#pragma once
#include "../include/ReplacementPolicy.h"
#include <list>
#include <unordered_map>
#include <algorithm>

/**
 * Adaptive Replacement Cache (ARC)
 * Reference: Nimrod Megiddo & Dharmendra S. Modha (IBM Almaden Research, FAST 2003)
 *
 * Self-tunes between Recency (T1) and Frequency (T2) using ghost caches (B1, B2).
 * Solves the real-world Cache Pollution problem where large sequential scans
 * evict high-frequency pages in traditional LRU.
 */
class ARC : public ReplacementPolicy {
    int c;       // Fixed cache capacity
    double p;    // Target size for T1 (adaptive tuning parameter)

    // T1: Recent cache pages (resident in cache)
    // B1: Recent ghost cache (metadata only, tracks recent evictions)
    // T2: Frequent cache pages (resident in cache, hit at least twice)
    // B2: Frequent ghost cache (metadata only, tracks frequent evictions)
    std::list<int> T1, B1, T2, B2;

    std::unordered_map<int, std::list<int>::iterator> posT1;
    std::unordered_map<int, std::list<int>::iterator> posB1;
    std::unordered_map<int, std::list<int>::iterator> posT2;
    std::unordered_map<int, std::list<int>::iterator> posB2;

    int replace(int page);

public:
    explicit ARC(int capacity);
    int access(int page) override;
    double getTargetP() const { return p; }
};
