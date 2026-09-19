#include "ExperimentJob.h"

using namespace std;

ExperimentJob::ExperimentJob(const ExperimentConfig& config) {
    this->config = config;
}

ExperimentConfig ExperimentJob::getConfig() const {
    return config;
}