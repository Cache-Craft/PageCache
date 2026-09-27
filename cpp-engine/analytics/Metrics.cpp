#include "Metrics.h"

using namespace std;

void Metrics::calculate(Result& result)
{
    if (result.evictions < 0)
        result.evictions = 0;

    if (result.latency < 0)
        result.latency = 0;
}