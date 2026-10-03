#include "Metrics.h"

using namespace std;

void Metrics::calculate(Result& result)
{
    if (result.workingSet < 0)
        result.workingSet = 0;

    if (result.pollution && result.workingSet == 0)
        result.pollution = false;
}