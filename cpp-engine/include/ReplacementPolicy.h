#pragma once
class ReplacementPolicy {
public:
    virtual ~ReplacementPolicy()=default;
    virtual int access(int page)=0;
};
