#include "AdaptiveController.h"
#include <unordered_map>
std::string AdaptiveController::select(const std::vector<int>&p){
    if(p.size()<2)return "LRU";
    int seq=0;std::unordered_map<int,int> f;
    for(size_t i=0;i<p.size();++i){f[p[i]]++;if(i&&p[i]==p[i-1]+1)seq++;}
    double sr=(double)seq/(p.size()-1);int repeated=0;
    for(auto&x:f)if(x.second>=3)repeated++;
    double rr=f.empty()?0:(double)repeated/f.size();
    if(sr>.60)return "FIFO";
    if(rr>.35)return "LFU";
    return "LRU";
}
