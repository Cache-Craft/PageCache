#ifndef METRICS_COLLECTOR_H
#define METRICS_COLLECTOR_H

class MetricsCollector {
private:
    int totalAccesses;
    int hits;
    int misses;

public:
    MetricsCollector(int totalAccesses, int hits, int misses);

    double getHitRatio() const;
    double getMissRatio() const;

    void printMetrics() const;
};

#endif