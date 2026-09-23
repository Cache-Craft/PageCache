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