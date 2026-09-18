#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "ExperimentConfig.h"

using namespace std;

class Validator {
public:
    static bool validate(const ExperimentConfig& config);
};

#endif