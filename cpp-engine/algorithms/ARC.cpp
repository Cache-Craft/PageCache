#include "ARC.h"

ARC::ARC(int capacity) : c(capacity), p(0.0) {}

int ARC::replace(int page) {
    int evicted = -1;
    bool inB2 = (posB2.find(page) != posB2.end());

    if (!T1.empty() && ((inB2 && T1.size() == static_cast<size_t>(p)) || (T1.size() > static_cast<size_t>(p)))) {
        // Evict LRU of T1 to MRU of B1
        evicted = T1.back();
        T1.pop_back();
        posT1.erase(evicted);

        B1.push_front(evicted);
        posB1[evicted] = B1.begin();
    } else if (!T2.empty()) {
        // Evict LRU of T2 to MRU of B2
        evicted = T2.back();
        T2.pop_back();
        posT2.erase(evicted);

        B2.push_front(evicted);
        posB2[evicted] = B2.begin();
    }
    return evicted;
}

int ARC::access(int page) {
    int evicted = -1;

    // Case 1: Page in T1 or T2 (Cache Hit)
    auto itT1 = posT1.find(page);
    if (itT1 != posT1.end()) {
        T1.erase(itT1->second);
        posT1.erase(itT1);
        T2.push_front(page);
        posT2[page] = T2.begin();
        return -1; // Hit, no eviction
    }

    auto itT2 = posT2.find(page);
    if (itT2 != posT2.end()) {
        T2.erase(itT2->second);
        T2.push_front(page);
        posT2[page] = T2.begin();
        return -1; // Hit, no eviction
    }

    // Case 2: Page in B1 (Ghost hit in B1)
    // Means Recency (T1) was not large enough; increase target p
    auto itB1 = posB1.find(page);
    if (itB1 != posB1.end()) {
        double delta1 = (B1.size() >= B2.size()) ? 1.0 : (static_cast<double>(B2.size()) / B1.size());
        p = std::min(static_cast<double>(c), p + delta1);
        evicted = replace(page);

        B1.erase(itB1->second);
        posB1.erase(itB1);

        T2.push_front(page);
        posT2[page] = T2.begin();
        return evicted;
    }

    // Case 3: Page in B2 (Ghost hit in B2)
    // Means Frequency (T2) was not large enough; decrease target p
    auto itB2 = posB2.find(page);
    if (itB2 != posB2.end()) {
        double delta2 = (B2.size() >= B1.size()) ? 1.0 : (static_cast<double>(B1.size()) / B2.size());
        p = std::max(0.0, p - delta2);
        evicted = replace(page);

        B2.erase(itB2->second);
        posB2.erase(itB2);

        T2.push_front(page);
        posT2[page] = T2.begin();
        return evicted;
    }

    // Case 4: Complete Cache Miss
    if (T1.size() + B1.size() == static_cast<size_t>(c)) {
        if (T1.size() < static_cast<size_t>(c)) {
            if (!B1.empty()) {
                int del = B1.back();
                B1.pop_back();
                posB1.erase(del);
            }
            evicted = replace(page);
        } else {
            if (!T1.empty()) {
                evicted = T1.back();
                T1.pop_back();
                posT1.erase(evicted);
            }
        }
    } else if (T1.size() + B1.size() < static_cast<size_t>(c)) {
        size_t total = T1.size() + T2.size() + B1.size() + B2.size();
        if (total >= static_cast<size_t>(c)) {
            if (total == static_cast<size_t>(2 * c)) {
                if (!B2.empty()) {
                    int del = B2.back();
                    B2.pop_back();
                    posB2.erase(del);
                } else if (!B1.empty()) {
                    int del = B1.back();
                    B1.pop_back();
                    posB1.erase(del);
                }
            }
            evicted = replace(page);
        }
    }

    T1.push_front(page);
    posT1[page] = T1.begin();
    return evicted;
}
