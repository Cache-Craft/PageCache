#include "Clock.h"
int Clock::access(int p){
    auto it=pos.find(p);
    if(it!=pos.end()){f[it->second].ref=true;return -1;}
    while(true){
        if(f[hand].page==-1){f[hand]={p,true};pos[p]=hand;hand=(hand+1)%cap;return -1;}
        if(!f[hand].ref){
            int e=f[hand].page;pos.erase(e);f[hand]={p,true};pos[p]=hand;hand=(hand+1)%cap;return e;
        }
        f[hand].ref=false;hand=(hand+1)%cap;
    }
}
