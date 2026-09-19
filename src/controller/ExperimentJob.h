#include "ExperimentConfig.h"

using namespace std;

class ExperimentJob {
private:
    ExperimentConfig config;

public:
    ExperimentJob(const ExperimentConfig& config);

    ExperimentConfig getConfig() const;
};