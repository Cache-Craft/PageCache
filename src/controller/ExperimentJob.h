#include "ExperimentConfig.h"

using namespace std;

class ExperimentJob {
private:
    int jobId;
    ExperimentConfig config;

public:
    ExperimentJob(int id, ExperimentConfig experimentConfig);

    int getJobId() const;
    ExperimentConfig getConfig() const;
};