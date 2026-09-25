#include "LFU.h"
int LFU::access(int p){
    ++timer;auto it=data.find(p);
    if(it!=data.end()){it->second.freq++;it->second.time=timer;return -1;}
    int e=-1;
    if((int)data.size()>=cap){
        auto v=data.begin();
        for(auto x=data.begin();x!=data.end();++x)
            if(x->second.freq<v->second.freq||(x->second.freq==v->second.freq&&x->second.time<v->second.time))v=x;
        e=v->first;data.erase(v);
    }
    data[p]={1,timer};return e;
}
