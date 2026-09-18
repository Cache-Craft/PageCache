#include "workload_generator.h"

#include <random>

std::vector<int> WorkloadGenerator::generateSequential(int count) {
    std::vector<int> workload;

    for (int i = 1; i <= count; ++i) {
        workload.push_back(i);
    }

    return workload;
}

std::vector<int> WorkloadGenerator::generateRandom(int count, int maxPage) {
    std::vector<int> workload;

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(1, maxPage);

    for (int i = 0; i < count; ++i) {
        workload.push_back(distribution(generator));
    }

    return workload;
}

std::vector<int> WorkloadGenerator::generateMixed(int count, int maxPage) {
    std::vector<int> workload;

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(1, maxPage);

    for (int i = 0; i < count; ++i) {
        if (i % 2 == 0) {
            workload.push_back((i / 2) % maxPage + 1);
        } else {
            workload.push_back(distribution(generator));
        }
    }

    return workload;
}