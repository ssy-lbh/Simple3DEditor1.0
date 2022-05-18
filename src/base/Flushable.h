#ifndef __BASE_FLUSHABLE__
#define __BASE_FLUSHABLE__

namespace simple3deditor {

class IFlushable {
public:
    virtual void Flush() = 0;
};

}

#endif