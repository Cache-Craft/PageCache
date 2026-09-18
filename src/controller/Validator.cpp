#include "Validator.h"

using namespace std;

bool Validator::validate(const ExperimentConfig& config) {

    if (config.cacheSize <= 0) {
        return false;
    }

    if (config.numberOfAccesses <= 0) {
        return false;
    }

    if (config.policy.empty()) {
        return false;
    }

    if (config.workloadType.empty()) {
        return false;
    }

    return true;
}