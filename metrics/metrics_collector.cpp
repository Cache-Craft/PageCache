#include "metrics_collector.h"

#include <iostream>

MetricsCollector::MetricsCollector(int totalAccesses, int hits, int misses)
    : totalAccesses(totalAccesses),
      hits(hits),
      misses(misses) {
}

double MetricsCollector::getHitRatio() const {
    if (totalAccesses == 0) {
        return 0.0;
    }

    return static_cast<double>(hits) / totalAccesses;
}

double MetricsCollector::getMissRatio() const {
    if (totalAccesses == 0) {
        return 0.0;
    }

    return static_cast<double>(misses) / totalAccesses;
}

void MetricsCollector::printMetrics() const {
    std::cout << "Total Accesses : " << totalAccesses << "\n";
    std::cout << "Hits           : " << hits << "\n";
    std::cout << "Misses         : " << misses << "\n";
    std::cout << "Hit Ratio      : " << getHitRatio() << "\n";
    std::cout << "Miss Ratio     : " << getMissRatio() << "\n";
}