#include "LRU.h"
int LRU::access(int p){
    auto it=pos.find(p);
    if(it!=pos.end()){order.erase(it->second);order.push_front(p);pos[p]=order.begin();return -1;}
    int e=-1;
    if((int)order.size()>=cap){e=order.back();pos.erase(e);order.pop_back();}
    order.push_front(p);pos[p]=order.begin();return e;
}
