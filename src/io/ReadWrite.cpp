#include <io/ReadWrite.h>

#include <cstring>

bool AReader::Ready() {
    return false;
}

void AWriter::Write(char c) {
    while(__atomic_test_and_set(&lock, 1));
    Write(&c, 1);
    lock = false;
}

void AWriter::Write(const char* s) {
    while(__atomic_test_and_set(&lock, 1));
    Write(s, strlen(s));
    lock = false;
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