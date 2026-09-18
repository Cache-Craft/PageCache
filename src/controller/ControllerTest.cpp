#include <iostream>
#include "ExperimentConfig.h"
#include "Validator.h"

using namespace std;

int main() {

    ExperimentConfig config;

    config.cacheSize = 3;
    config.numberOfAccesses = 20;
    config.policy = "FIFO";
    config.workloadType = "Random";

    if (Validator::validate(config)) {
        cout << "Configuration is valid!" << endl;
    } else {
        cout << "Configuration is invalid!" << endl;
    }

    return 0;
}