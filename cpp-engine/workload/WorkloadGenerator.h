#pragma once

#include <vector>
#include <string>

using namespace std;

class WorkloadGenerator
{
public:
    static vector<int> sequential(int count, int uniquePages);
    static vector<int> randomWorkload(int count, int uniquePages);
    static vector<int> locality(int count, int uniquePages);
};