#include <io/ReadWrite.h>

#include <cstring>

namespace simple3deditor {

bool AReader::Ready() {
    return false;
}

void AWriter::Write(char c) {
    while(std::atomic_flag_test_and_set(&lock))
        continue;
    Write(&c, 1);
    std::atomic_flag_clear(&lock);
}

void AWriter::Write(const char* s) {
    while(std::atomic_flag_test_and_set(&lock))
        continue;
    Write(s, ::strlen(s));
    std::atomic_flag_clear(&lock);
}

AWriter& AWriter::Append(const char* s) {
    if (!s) {
        Write("null", 4);
        return *this;
    }
    Write(s);
    return *this;
}

AWriter& AWriter::Append(const char* s, size_t size) {
    Write(s, size);
    return *this;
}

AWriter& AWriter::Append(char c) {
    Write(c);
    return *this;
}

}