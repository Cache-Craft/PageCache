#ifndef EXPERIMENT_CONFIG_H
#define EXPERIMENT_CONFIG_H

#include <string>

using namespace std;

struct ExperimentConfig {
    int cacheSize;
    int numberOfAccesses;
    string policy;
    string workloadType;
};

#endif