#include "cache/CacheManager.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;
static vector<int> parse(string s){vector<int>p;stringstream ss(s);string t;while(getline(ss,t,','))if(!t.empty())p.push_back(stoi(t));return p;}
int main(int argc,char**argv){
    string alg="LRU",pageArg;int cap=4;
    for(int i=1;i<argc;i++){string a=argv[i];if(a=="--algorithm"&&i+1<argc)alg=argv[++i];else if(a=="--cache-size"&&i+1<argc)cap=stoi(argv[++i]);else if(a=="--pages"&&i+1<argc)pageArg=argv[++i];}
    if(cap<1||pageArg.empty()){cerr<<"Usage: --algorithm FIFO|LRU|LFU|CLOCK|ADAPTIVE --cache-size N --pages 1,2,3";return 1;}
    for(char&c:alg)c=toupper((unsigned char)c);
    auto pages=parse(pageArg);CacheManager cm(cap);auto r=cm.run(alg,pages);
    cout<<"{\"algorithm\":\""<<alg<<"\",\"selected_algorithm\":\""<<r.selected<<"\",\"cache_size\":"<<cap<<",\"metrics\":{";
    cout<<"\"total_requests\":"<<r.total<<",\"hits\":"<<r.hits<<",\"misses\":"<<r.misses<<",\"evictions\":"<<r.evictions;
    cout<<",\"hit_ratio\":"<<r.hitRatio<<",\"miss_ratio\":"<<r.missRatio<<",\"avg_latency_ms\":"<<r.latency;
    cout<<",\"working_set_size\":"<<r.workingSet<<",\"pollution_detected\":"<<(r.pollution?"true":"false")<<",\"admission_rejections\":"<<r.admissionRejections<<"},\"access_log\":[";
    for(size_t i=0;i<r.log.size();i++){if(i)cout<<",";auto&a=r.log[i];cout<<"{\"sequence\":"<<a.seq<<",\"page\":"<<a.page<<",\"hit\":"<<(a.hit?"true":"false")<<",\"admitted\":"<<(a.admitted?"true":"false")<<",\"evicted_page\":"<<(a.evicted<0?string("null"):to_string(a.evicted))<<"}";}
    cout<<"]}";
}
