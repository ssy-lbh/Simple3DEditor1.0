#ifndef __BASE_CLOSEABLE__
#define __BASE_CLOSEABLE__

class ICloseable {
public:
    virtual void Close() = 0;
};

#endif