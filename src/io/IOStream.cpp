#include <io/IOStream.h>

namespace simple3deditor {

size_t IInputStream::Available() {
    return 0;
}

void IInputStream::Flush(){}
void IInputStream::Close(){}

void IOutputStream::Flush(){}
void IOutputStream::Close(){}

}