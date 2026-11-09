#pragma once
#include "../include/ReplacementPolicy.h"
#include <memory>
#include <vector>
#include <string>

struct FrameInfo {
    int frame;
    int page;
    bool dirty;
    int freq;
    bool ref;
};

struct Access {
    int seq;
    int page;
    char op; // 'R' for Read, 'W' for Write
    bool hit;
    int evicted;
    bool dirtyEviction;
    bool admitted;
    std::vector<FrameInfo> frames;
};

struct StorageProfile {
    std::string name = "NVMe SSD";
    double ramHitMs = 0.0001;      // 100 ns DRAM access
    double storageReadMs = 0.080;  // 80 microseconds NVMe read
    double storageWriteMs = 0.250; // 250 microseconds NVMe writeback
};

struct Result {
    int total = 0;
    int hits = 0;
    int misses = 0;
    int readHits = 0;
    int writeHits = 0;
    int evictions = 0;
    int cleanEvictions = 0;
    int dirtyEvictions = 0;
    int writebacks = 0;
    int workingSet = 0;
    int admissionRejections = 0;

    double hitRatio = 0.0;
    double missRatio = 0.0;
    double avgLatencyMs = 0.0;
    double p95LatencyMs = 0.0;
    double p99LatencyMs = 0.0;
    double ioTimeSavedMs = 0.0;
    double ssdWearSavedKb = 0.0; // 4KB per coalesced write

    bool pollution = false;
    std::string selected;
    std::string storageProfileName = "NVMe SSD";

    std::vector<Access> log;
};

class CacheManager {
    int cap;
    StorageProfile storage;

public:
    explicit CacheManager(int c, const std::string& profile = "nvme");
    Result run(
        const std::string& algorithm,
        const std::vector<int>& pages,
        const std::vector<char>& ops = {}
    );
};
