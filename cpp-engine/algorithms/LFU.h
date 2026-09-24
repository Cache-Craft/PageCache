#pragma once
#include "../include/ReplacementPolicy.h"
#include <unordered_map>
class LFU: public ReplacementPolicy{
    struct E{int freq;long long time;};
    int cap;long long timer=0;std::unordered_map<int,E> data;
public:
    explicit LFU(int c):cap(c){}
    int access(int page) override;
};
