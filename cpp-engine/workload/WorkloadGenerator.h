#pragma once
#include <vector>
#include <string>
class WorkloadGenerator{
public:
    static std::vector<int> sequential(int count,int uniquePages);
    static std::vector<int> randomWorkload(int count,int uniquePages);
    static std::vector<int> locality(int count,int uniquePages);
};
