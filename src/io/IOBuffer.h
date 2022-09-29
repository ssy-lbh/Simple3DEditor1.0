#ifndef __IO_IOBUFFER__
#define __IO_IOBUFFER__

#include <define.h>

#include <base/Closeable.h>

#include <atomic>

namespace simple3deditor {

class BufferedInputStream : public Object, public ICloseable {
private:
    static constexpr uint DEFAULT_BUFFER_SIZE = (1 << 13);

protected:
    IInputStream* in = NULL;
    volatile byte* buf;
    volatile size_t bufsize;
    volatile std::atomic_flag lock;
    int count;
    int pos;
    int markpos;
    int marklimit;

    IInputStream* GetInIfOpen();
    byte* GetBufIfOpen();
    void Fill();
    int Read1(byte* buf, size_t len);

public:
    BufferedInputStream(IInputStream* in);
    BufferedInputStream(IInputStream* in, size_t size);

    int Read();
    int Read(byte* buf, size_t len);
    size_t Skip(size_t len);
    size_t Available();

    using ICloseable::Close;
};

}

#endif