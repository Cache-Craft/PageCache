#pragma once
#include "../include/ReplacementPolicy.h"
#include <memory>
#include <vector>
#include <string>
struct Access{int seq;int page;bool hit;int evicted;bool admitted;};
struct Result{
    int total=0,hits=0,misses=0,evictions=0,workingSet=0,admissionRejections=0;
    double hitRatio=0,missRatio=0,latency=0;bool pollution=false;
    std::string selected;
    std::vector<Access> log;
};
class CacheManager{
    int cap;
public:
    explicit CacheManager(int c):cap(c){}
    Result run(const std::string& algorithm,const std::vector<int>& pages);
};
