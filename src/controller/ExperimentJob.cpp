#include "ExperimentJob.h"

using namespace std;

ExperimentJob::ExperimentJob(int id, ExperimentConfig experimentConfig) {
    jobId = id;
    config = experimentConfig;
}

int ExperimentJob::getJobId() const {
    return jobId;
}

ExperimentConfig ExperimentJob::getConfig() const {
    return config;
}