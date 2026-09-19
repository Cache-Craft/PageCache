#include <iostream>

#include "ExperimentConfig.h"
#include "Validator.h"
#include "ExperimentJob.h"

using namespace std;

int main() {

    ExperimentConfig config;

    config.cacheSize = 3;
    config.numberOfAccesses = 20;
    config.policy = "FIFO";
    config.workloadType = "Random";

    if (Validator::validate(config)) {

        cout << "Configuration is valid!" << endl;

        ExperimentJob job(1, config);

        cout << "\n===== EXPERIMENT JOB =====" << endl;
        cout << "Job ID           : " << job.getJobId() << endl;
        cout << "Cache Size       : " << job.getConfig().cacheSize << endl;
        cout << "Number of Access : "
             << job.getConfig().numberOfAccesses << endl;
        cout << "Policy           : "
             << job.getConfig().policy << endl;
        cout << "Workload Type    : "
             << job.getConfig().workloadType << endl;

    } else {

        cout << "Configuration is invalid!" << endl;
    }

    return 0;
}