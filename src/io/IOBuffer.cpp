#include <io/IOBuffer.h>

#include <io/IOStream.h>

#include <memory>
#include <atomic>

namespace simple3deditor {

IInputStream* BufferedInputStream::GetInIfOpen(){
    return in;
}

byte* BufferedInputStream::GetBufIfOpen(){
    return const_cast<byte*>(buf);
}

void BufferedInputStream::Fill(){
    byte* buf = GetBufIfOpen();
    int var2;
    if (pos >= bufsize) {
        if (markpos > 0) {
            var2 = pos - markpos;
            memmove(buf, buf + markpos, var2 * sizeof(byte));
            pos = var2;
            markpos = 0;
        } else if (bufsize >= marklimit) {
            markpos = -1;
            pos = 0;
        } else {
            var2 = pos << 1;
            if (var2 > marklimit) {
                var2 = marklimit;
            }

            byte* var3 = new byte[var2];
            memcpy(var3, buf, pos * sizeof(byte));
            buf = var3;
        }
    }

    count = pos;
    var2 = GetInIfOpen()->Read(buf + pos, bufsize);
    if (var2 > 0) {
        count = var2 + pos;
    }
}

int BufferedInputStream::Read1(byte* buf, size_t len){
    while(std::atomic_flag_test_and_set(&lock));
    int var4 = count - pos;
    if (var4 <= 0) {
        if (len >= bufsize && markpos < 0) {
            std::atomic_flag_clear(&lock);
            return GetInIfOpen()->Read(buf + pos, len);
        }

        Fill();
        var4 = count - pos;
        if (var4 <= 0) {
            std::atomic_flag_clear(&lock);
            return -1;
        }
    }

    int var5 = var4 < len ? var4 : len;
    memcpy(buf, buf + pos, var5 * sizeof(byte));
    pos += var5;
    std::atomic_flag_clear(&lock);
    return var5;
}

BufferedInputStream::BufferedInputStream(IInputStream* in) : BufferedInputStream(in, DEFAULT_BUFFER_SIZE) {}

BufferedInputStream::BufferedInputStream(IInputStream* in, size_t size) {
    markpos = -1;
    buf = new byte[size];
    bufsize = size;
}

int BufferedInputStream::Read(){
    while(std::atomic_flag_test_and_set(&lock));
    if (pos >= count) {
        Fill();
        if (pos >= count) {
            std::atomic_flag_clear(&lock);
            return -1;
        }
    }
    std::atomic_flag_clear(&lock);
    return GetBufIfOpen()[pos++] & 0xFF;
}

int BufferedInputStream::Read(byte* buf, size_t len){
    while(std::atomic_flag_test_and_set(&lock));
    int var4 = 0;

    IInputStream* var6;
    do {
        int var5 = Read1(buf, len);
        if (var5 <= 0) {
            std::atomic_flag_clear(&lock);
            return var4 == 0 ? var5 : var4;
        }

        var4 += var5;
        if (var4 >= len) {
            std::atomic_flag_clear(&lock);
            return var4;
        }

        var6 = in;
    } while(var6 == NULL || var6->Available() > 0);

    std::atomic_flag_clear(&lock);
    return var4;
}

size_t BufferedInputStream::Skip(size_t len){
    while(std::atomic_flag_test_and_set(&lock));
    size_t var3 = (size_t)(count - pos);
    if (var3 <= 0) {
        if (markpos < 0) {
            std::atomic_flag_clear(&lock);
            return GetInIfOpen()->Skip(len);
        }

        Fill();
        var3 = (size_t)(count - pos);
        if (var3 <= 0) {
            std::atomic_flag_clear(&lock);
            return 0L;
        }
    }

    size_t var5 = var3 < len ? var3 : len;
    pos = (int)((size_t)pos + var5);
    std::atomic_flag_clear(&lock);
    return var5;
}

size_t BufferedInputStream::Available(){
    while(std::atomic_flag_test_and_set(&lock));
    size_t ret = GetInIfOpen()->Available() + (count - pos);
    std::atomic_flag_clear(&lock);
    return ret;
}

}