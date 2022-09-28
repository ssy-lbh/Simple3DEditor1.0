#ifndef __BASE_CLOSEABLE__
#define __BASE_CLOSEABLE__

namespace simple3deditor {

class ICloseable {
public:
    virtual void Close() = 0;
};

}

#endif