#include "FIFO.h"
int FIFO::access(int p){
    if(s.count(p)) return -1;
    int e=-1;
    if((int)q.size()>=cap){e=q.front();q.pop_front();s.erase(e);}
    q.push_back(p);s.insert(p);return e;
}
