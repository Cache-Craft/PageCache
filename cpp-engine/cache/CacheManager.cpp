#include "CacheManager.h"
#include "../algorithms/FIFO.h"
#include "../algorithms/LRU.h"
#include "../algorithms/LFU.h"
#include "../algorithms/Clock.h"
#include "../algorithms/ARC.h"
#include "../adaptive/AdaptiveController.h"

#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <numeric>

using namespace std;

CacheManager::CacheManager(int c, const string& profile) : cap(c) {
    string p = profile;
    for (char& ch : p) ch = tolower((unsigned char)ch);

    if (p == "hdd" || p == "sata") {
        storage = {"SATA HDD", 0.0001, 8.50, 12.00};
    } else if (p == "ebs" || p == "cloud") {
        storage = {"Cloud Block EBS", 0.0001, 1.20, 2.50};
    } else {
        storage = {"NVMe Gen4 SSD", 0.0001, 0.080, 0.250};
    }
}

static unique_ptr<ReplacementPolicy> makePolicy(
    const string& algorithm,
    int capacity
) {
    if (algorithm == "FIFO") return make_unique<FIFO>(capacity);
    if (algorithm == "LRU") return make_unique<LRU>(capacity);
    if (algorithm == "LFU") return make_unique<LFU>(capacity);
    if (algorithm == "CLOCK") return make_unique<Clock>(capacity);
    if (algorithm == "ARC") return make_unique<ARC>(capacity);

    throw invalid_argument("Unsupported replacement algorithm: " + algorithm);
}

Result CacheManager::run(
    const string& algorithm,
    const vector<int>& pages,
    const vector<char>& ops
) {
    if (cap < 1) throw invalid_argument("Cache capacity must be at least 1");
    if (pages.empty()) throw invalid_argument("Page reference sequence cannot be empty");

    string selectedAlgorithm = algorithm;
    if (algorithm == "ADAPTIVE") {
        selectedAlgorithm = AdaptiveController::select(pages, cap);
    }

    auto policy = makePolicy(selectedAlgorithm, cap);

    Result r;
    r.total = static_cast<int>(pages.size());
    r.selected = selectedAlgorithm;
    r.storageProfileName = storage.name;

    // Physical cache frames tracking
    vector<FrameInfo> framesTable(cap);
    for (int i = 0; i < cap; ++i) {
        framesTable[i] = {i, -1, false, 0, false};
    }

    unordered_map<int, int> pageToFrame; // page -> frame index
    vector<int> freeFrames;
    for (int i = cap - 1; i >= 0; --i) freeFrames.push_back(i);

    vector<double> latencies;
    latencies.reserve(pages.size());
    double rawTotalIoMs = 0.0;

    for (size_t i = 0; i < pages.size(); ++i) {
        int page = pages[i];
        char op = (i < ops.size() && (ops[i] == 'W' || ops[i] == 'w')) ? 'W' : 'R';

        bool hit = (pageToFrame.find(page) != pageToFrame.end());
        int evictedPage = policy->access(page);

        bool isDirtyEviction = false;
        int targetFrame = -1;

        if (hit) {
            targetFrame = pageToFrame[page];
            framesTable[targetFrame].freq++;
            framesTable[targetFrame].ref = true;
            if (op == 'W') {
                framesTable[targetFrame].dirty = true;
                r.writeHits++;
                // Coalesced write saves SSD wear (4KB)
                r.ssdWearSavedKb += 4.0;
            } else {
                r.readHits++;
            }
            r.hits++;
        } else {
            // Cache Miss
            if (evictedPage != -1) {
                r.evictions++;
                auto it = pageToFrame.find(evictedPage);
                if (it != pageToFrame.end()) {
                    targetFrame = it->second;
                    if (framesTable[targetFrame].dirty) {
                        isDirtyEviction = true;
                        r.dirtyEvictions++;
                        r.writebacks++;
                    } else {
                        r.cleanEvictions++;
                    }
                    pageToFrame.erase(it);
                }
            } else if (!freeFrames.empty()) {
                targetFrame = freeFrames.back();
                freeFrames.pop_back();
            }

            if (targetFrame != -1) {
                framesTable[targetFrame].frame = targetFrame;
                framesTable[targetFrame].page = page;
                framesTable[targetFrame].dirty = (op == 'W');
                framesTable[targetFrame].freq = 1;
                framesTable[targetFrame].ref = true;
                pageToFrame[page] = targetFrame;
            }
        }

        // Calculate Access Latency
        double accessLatency = 0.0;
        if (hit) {
            accessLatency = storage.ramHitMs;
        } else {
            accessLatency = storage.ramHitMs + storage.storageReadMs;
            if (isDirtyEviction) {
                // Dirty page writeback penalty before frame can be reused
                accessLatency += storage.storageWriteMs;
            }
        }
        latencies.push_back(accessLatency);

        // Raw storage baseline without cache
        rawTotalIoMs += (op == 'W') ? storage.storageWriteMs : storage.storageReadMs;

        // Snapshot frames for simulation visualizer (limit to first 500 for fast payload)
        vector<FrameInfo> frameSnapshot;
        if (i < 500) {
            frameSnapshot = framesTable;
        }

        r.log.push_back({
            static_cast<int>(i + 1),
            page,
            op,
            hit,
            evictedPage,
            isDirtyEviction,
            true,
            frameSnapshot
        });
    }

    r.misses = r.total - r.hits;
    r.hitRatio = static_cast<double>(r.hits) / r.total;
    r.missRatio = static_cast<double>(r.misses) / r.total;

    double totalLatency = std::accumulate(latencies.begin(), latencies.end(), 0.0);
    r.avgLatencyMs = totalLatency / r.total;
    r.ioTimeSavedMs = std::max(0.0, rawTotalIoMs - totalLatency);

    // Compute percentiles (p95 and p99)
    vector<double> sortedLatencies = latencies;
    std::sort(sortedLatencies.begin(), sortedLatencies.end());
    size_t p95Idx = static_cast<size_t>(sortedLatencies.size() * 0.95);
    size_t p99Idx = static_cast<size_t>(sortedLatencies.size() * 0.99);
    r.p95LatencyMs = sortedLatencies[min(p95Idx, sortedLatencies.size() - 1)];
    r.p99LatencyMs = sortedLatencies[min(p99Idx, sortedLatencies.size() - 1)];

    // Working set calculation
    unordered_set<int> workingSet;
    int windowSize = min(50, static_cast<int>(pages.size()));
    for (int i = static_cast<int>(pages.size()) - windowSize; i < static_cast<int>(pages.size()); ++i) {
        workingSet.insert(pages[i]);
    }
    r.workingSet = static_cast<int>(workingSet.size());

    // Cache pollution detection
    unordered_set<int> uniquePages(pages.begin(), pages.end());
    r.pollution = (static_cast<double>(uniquePages.size()) / r.total > 0.60) && (r.hitRatio < 0.35);

    return r;
}
