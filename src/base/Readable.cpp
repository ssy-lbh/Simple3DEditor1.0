#include <base/Readable.h>

#include <util/String.h>
#include <util/StringBuilder.h>
#include <util/math3d/Math.h>

char IReadable::skipBuffer[SKIP_BUFFER_SIZE];

int IReadable::Read() {
    char c;
    Read(&c, 1);
    return c;
}

bool IReadable::ReadBool(){
    bool b;
    Read(&b, sizeof(bool));
    return b;
}

short IReadable::ReadShort(){
    short s;
    Read(&s, sizeof(short));
    return s;
}

int IReadable::ReadInt(){
    int i;
    Read(&i, sizeof(int));
    return i;
}

long long IReadable::ReadLong(){
    long long l;
    Read(&l, sizeof(long long));
    return l;
}

float IReadable::ReadFloat(){
    float f;
    Read(&f, sizeof(float));
    return f;
}

double IReadable::ReadDouble(){
    double d;
    Read(&d, sizeof(double));
    return d;
}

size_t IReadable::Skip(size_t n) {
    size_t rem = n;
    while(rem) {
        size_t rd = Read(skipBuffer, Min(SKIP_BUFFER_SIZE, rem));
        if (rd == -1)
            break;
        rem -= rd;
    }
    return n - rem;
}

String IReadable::ReadString(){
    int n = ReadInt();
    char* str = new char[n];
    Read(str, n * sizeof(char));
    return String(str, n);
}

WString IReadable::ReadWString(){
    int n = ReadInt();
    wchar_t* str = new wchar_t[n];
    Read(str, n * sizeof(wchar_t));
    return WString(str, n);
}