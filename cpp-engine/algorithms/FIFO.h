#pragma once
#include "../include/ReplacementPolicy.h"
#include <deque>
#include <unordered_set>
class FIFO: public ReplacementPolicy{
    int cap; std::deque<int> q; std::unordered_set<int> s;
public:
    explicit FIFO(int c):cap(c){}
    int access(int page) override;
};
