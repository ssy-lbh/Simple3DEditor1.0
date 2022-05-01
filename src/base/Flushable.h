#ifndef __BASE_FLUSHABLE__
#define __BASE_FLUSHABLE__

class IFlushable {
public:
    virtual void Flush() = 0;
};

#endif