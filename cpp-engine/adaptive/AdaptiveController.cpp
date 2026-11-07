#include "AdaptiveController.h"
#include <unordered_map>
#include <vector>
#include <algorithm>

std::string AdaptiveController::select(const std::vector<int>& pages, int capacity) {
    if (pages.size() < 2) return "LRU";

    int seqCount = 0;
    std::unordered_map<int, int> freq;

    for (size_t i = 0; i < pages.size(); ++i) {
        freq[pages[i]]++;
        if (i > 0 && (pages[i] == pages[i - 1] + 1 || pages[i] == pages[i - 1])) {
            seqCount++;
        }
    }

    double seqRatio = static_cast<double>(seqCount) / (pages.size() - 1);

    // Compute Zipfian frequency skew (top 20% pages share of requests)
    std::vector<int> counts;
    counts.reserve(freq.size());
    for (const auto& kv : freq) {
        counts.push_back(kv.second);
    }
    std::sort(counts.rbegin(), counts.rend());

    size_t top20Count = std::max<size_t>(1, counts.size() / 5);
    int top20Sum = 0;
    for (size_t i = 0; i < top20Count && i < counts.size(); ++i) {
        top20Sum += counts[i];
    }
    double skewness = static_cast<double>(top20Sum) / pages.size();

    // Check for cyclic thrashing pattern (stride pattern near capacity + 1)
    bool cyclicThrashing = false;
    if (capacity > 1 && pages.size() >= static_cast<size_t>((capacity + 1) * 2)) {
        int cycleLen = capacity + 1;
        int matchCount = 0;
        for (size_t i = cycleLen; i < pages.size(); ++i) {
            if (pages[i] == pages[i - cycleLen]) matchCount++;
        }
        if (static_cast<double>(matchCount) / (pages.size() - cycleLen) > 0.70) {
            cyclicThrashing = true;
        }
    }

    // Decision Logic:
    // 1. High sequentiality (Table scan / streaming) -> ARC (scan-resistant, avoids pollution)
    if (seqRatio > 0.45) {
        return "ARC";
    }

    // 2. Cyclic thrashing (Belady / LRU killer) -> CLOCK or ARC
    if (cyclicThrashing) {
        return "CLOCK";
    }

    // 3. High skewness with repeated accesses (OLTP Database) -> ARC / LFU
    if (skewness > 0.65) {
        return "ARC";
    }

    // 4. Standard moderate temporal locality
    if (freq.size() <= static_cast<size_t>(capacity * 2)) {
        return "LRU";
    }

    // 5. Default robust self-tuning policy
    return "ARC";
}
