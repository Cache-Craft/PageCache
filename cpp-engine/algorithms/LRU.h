#pragma once
#include "../include/ReplacementPolicy.h"
#include <list>
#include <unordered_map>
class LRU: public ReplacementPolicy{
    int cap; std::list<int> order; std::unordered_map<int,std::list<int>::iterator> pos;
public:
    explicit LRU(int c):cap(c){}
    int access(int page) override;
};
