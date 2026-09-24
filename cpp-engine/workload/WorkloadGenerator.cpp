#include "WorkloadGenerator.h"
#include <random>

using namespace std;

vector<int> WorkloadGenerator::sequential(int n, int u)
{
    vector<int> p;

    for (int i = 0; i < n; i++)
    {
        p.push_back(i % u);
    }

    return p;
}

vector<int> WorkloadGenerator::randomWorkload(int n, int u)
{
    mt19937 g(42);
    uniform_int_distribution<int> d(0, u - 1);

    vector<int> p;

    for (int i = 0; i < n; i++)
    {
        p.push_back(d(g));
    }

    return p;
}
    vector<int> WorkloadGenerator::locality(int n, int u)
   {
    mt19937 g(42);
    uniform_int_distribution<int> all(0, u - 1);

    int hot = max(1, u / 5);

    uniform_int_distribution<int> h(0, hot - 1);
    uniform_real_distribution<double> x(0, 1);

    vector<int> p;

    for (int i = 0; i < n; i++)
        p.push_back(x(g) < 0.8 ? h(g) : all(g));

    return p;
}
