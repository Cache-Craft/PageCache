#include "WorkloadGenerator.h"
#include <random>
std::vector<int> WorkloadGenerator::sequential(int n,int u){std::vector<int>p;for(int i=0;i<n;i++)p.push_back(i%u);return p;}
std::vector<int> WorkloadGenerator::randomWorkload(int n,int u){std::mt19937 g(42);std::uniform_int_distribution<int>d(0,u-1);std::vector<int>p;for(int i=0;i<n;i++)p.push_back(d(g));return p;}
std::vector<int> WorkloadGenerator::locality(int n,int u){std::mt19937 g(42);std::uniform_int_distribution<int>all(0,u-1);int hot=std::max(1,u/5);std::uniform_int_distribution<int>h(0,hot-1);std::uniform_real_distribution<double>x(0,1);std::vector<int>p;for(int i=0;i<n;i++)p.push_back(x(g)<.8?h(g):all(g));return p;}
