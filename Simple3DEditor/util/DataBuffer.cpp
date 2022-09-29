#include <util/DataBuffer.h>

#include <cstring>

#include <util/os/Log.h>

namespace simple3deditor {

DataBuffer::DataBuffer(){
    data = new char[32];
    size = 32;
    ptr = 0;
}

DataBuffer::DataBuffer(DataBuffer &&d){
    size = d.size;
    data = new char[size];
    ptr = d.ptr;
    rptr = d.rptr;
    memcpy(data, d.data, ptr);
}

DataBuffer::DataBuffer(const DataBuffer &d){
    size = d.size;
    data = new char[size];
    ptr = d.ptr;
    rptr = d.rptr;
    memcpy(data, d.data, ptr);
}

DataBuffer &DataBuffer::operator=(DataBuffer &&d){
    delete[] data;
    size = d.size;
    data = new char[size];
    ptr = d.ptr;
    rptr = d.rptr;
    memcpy(data, d.data, ptr);
    return *this;
}

DataBuffer &DataBuffer::operator=(const DataBuffer &d){
    delete[] data;
    size = d.size;
    data = new char[size];
    ptr = d.ptr;
    rptr = d.rptr;
    memcpy(data, d.data, ptr);
    return *this;
}

DataBuffer::DataBuffer(size_t len){
    data = new char[len];
    size = len;
    ptr = 0;
}

DataBuffer::DataBuffer(const void* buf, size_t len){
    data = new char[len];
    size = len;
    ptr = 0;
    memcpy(data, buf, len);
}

DataBuffer::DataBuffer(const void* buf, size_t len, bool commit){
    if (commit){
        data = (char*)const_cast<void*>(buf);
        size = len;
        ptr = len;
    }else{
        data = new char[len];
        size = len;
        ptr = 0;
        memcpy(data, buf, len);
    }
}

DataBuffer::~DataBuffer(){
    if (data) delete[] data;
}

void DataBuffer::Check(size_t reserve){
    if (ptr + reserve < size)
        return;
    size <<= 1;
    while (ptr + reserve >= size)
        size <<= 1;
    char* newData = new char[size];
    memcpy(newData, data, ptr * sizeof(char));
    delete[] data;
    data = newData;
}

void DataBuffer::Write(char c){
    Check(2);
    data[ptr++] = c;
}

void DataBuffer::Write(const void* buf, size_t len){
    Check(len + 1);
    memcpy(data + ptr, buf, len);
    ptr += len;
}

char DataBuffer::Read() const{
    if (rptr >= ptr)
        return -1;
    return data[rptr++];
}

size_t DataBuffer::Read(void* buf, size_t len) const{
    if (rptr >= ptr)
        return 0;
    len = (len >= ptr - rptr ? len : ptr - rptr);
    memcpy(buf, data + rptr, len);
    rptr += len;
    return len;
}

PackDataBuffer::PackDataBuffer(DataBuffer &&d) : DataBuffer(false) {
    this->data = (char*)d.Buffer(); this->ptr = d.Size();
}

PackDataBuffer::PackDataBuffer(const DataBuffer &d) : DataBuffer(false) {
    this->data = (char*)const_cast<void*>(d.ReadOnlyBuffer()); this->ptr = d.Size();
}

PackDataBuffer::PackDataBuffer(const void* s, size_t size) : DataBuffer(false) {
    this->data = const_cast<char*>(data); this->ptr = size;
}

PackDataBuffer::~PackDataBuffer(){
    data = NULL;
}

}