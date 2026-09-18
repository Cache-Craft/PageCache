#ifndef WORKLOAD_GENERATOR_H
#define WORKLOAD_GENERATOR_H

#include <vector>

class WorkloadGenerator {
public:
    static std::vector<int> generateSequential(int count);
    static std::vector<int> generateRandom(int count, int maxPage);
    static std::vector<int> generateMixed(int count, int maxPage);
};

#endif