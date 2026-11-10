#pragma once
#include "../include/ReplacementPolicy.h"
#include <vector>
#include <unordered_map>
class Clock: public ReplacementPolicy{
    struct F{int page=-1;bool ref=false;};
    int cap,hand=0;std::vector<F> f;std::unordered_map<int,int> pos;
public:
    explicit Clock(int c):cap(c),f(c){}
    int access(int page) override;
};
