#include "CacheManager.h"
#include "../algorithms/FIFO.h"
#include "../algorithms/LRU.h"
#include "../algorithms/LFU.h"
#include "../algorithms/Clock.h"
#include "../adaptive/AdaptiveController.h"
#include <unordered_set>
#include <memory>
#include <algorithm>

static std::unique_ptr<ReplacementPolicy> makePolicy(const std::string&a,int c){
    if(a=="FIFO")return std::make_unique<FIFO>(c);
    if(a=="LRU")return std::make_unique<LRU>(c);
    if(a=="LFU")return std::make_unique<LFU>(c);
    return std::make_unique<Clock>(c);
}
Result CacheManager::run(const std::string&algorithm,const std::vector<int>&pages){
    Result r;r.total=(int)pages.size();r.selected=algorithm=="ADAPTIVE"?AdaptiveController::select(pages):algorithm;
    auto policy=makePolicy(r.selected,cap);std::unordered_set<int> current;
    for(size_t i=0;i<pages.size();++i){
        int p=pages[i];bool hit=current.count(p);
        bool admitted=true;
        // Admission heuristic: reject obvious one-time cold pages only when cache is under pressure.
        if(!hit && current.size()>=(size_t)cap && i+1<pages.size()){
            int future=0;for(size_t j=i+1;j<pages.size()&&j<=i+10;++j)if(pages[j]==p)future++;
            if(future==0)admitted=false;
        }
        int e=-1;if(admitted)e=policy->access(p);
        if(e!=-1){current.erase(e);r.evictions++;}
        if(admitted)current.insert(p);
        if(hit)r.hits++;r.log.push_back({(int)i+1,p,hit,e,admitted});
    }
    r.misses=r.total-r.hits;r.admissionRejections=0;
    for(auto&a:r.log)if(!a.admitted)r.admissionRejections++;
    r.hitRatio=(double)r.hits/r.total;r.missRatio=(double)r.misses/r.total;
    double totalLatency=0;for(auto&a:r.log)totalLatency+=a.hit?1.0:10.0;totalLatency+=2.0*r.evictions;r.latency=totalLatency/r.total;
    std::unordered_set<int> ws;int w=std::min(20,(int)pages.size());
    for(int i=(int)pages.size()-w;i<(int)pages.size();++i)ws.insert(pages[i]);r.workingSet=ws.size();
    std::unordered_set<int> unique(pages.begin(),pages.end());
    r.pollution=((double)unique.size()/r.total>.65&&r.hitRatio<.30);
    return r;
}
