#include "Metrics.h"
// Metrics are calculated inside CacheManager in this version.
// This translation unit exists so the analytics module is independently extensible.
void Metrics::calculate(Result&) {}
