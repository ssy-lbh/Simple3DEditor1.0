#include <base/Writable.h>

#include <cstring>

#include <util/String.h>

namespace simple3deditor {

void IWritable::Write(char c){
    Write(&c, sizeof(char));
}

void IWritable::Write(wchar_t c){
    Write(&c, sizeof(wchar_t));
}

void IWritable::Write(bool b){
    Write(&b, sizeof(bool));
}

void IWritable::Write(short s){
    Write(&s, sizeof(short));
}

void IWritable::Write(int i){
    Write(&i, sizeof(int));
}

void IWritable::Write(long long l){
    Write(&l, sizeof(long long));
}

void IWritable::Write(float f){
    Write(&f, sizeof(float));
}

void IWritable::Write(double d){
    Write(&d, sizeof(double));
}

void IWritable::Write(const char* s){
    Write(s, strlen(s) * sizeof(char));
}

void IWritable::Write(const wchar_t* s){
    Write(s, wcslen(s) * sizeof(wchar_t));
}

void IWritable::WriteWithLen(const char* s){
    WriteWithLen(s, strlen(s));
}

void IWritable::WriteWithLen(const wchar_t* s){
    WriteWithLen(s, wcslen(s));
}

void IWritable::WriteWithLen(const char* s, int len){
    Write(len);
    Write(s, len * sizeof(char));
}

void IWritable::WriteWithLen(const char* s, size_t len){
    Write((int)len);
    Write(s, len * sizeof(char));
}

void IWritable::WriteWithLen(const wchar_t* s, int len){
    Write(len);
    Write(s, len * sizeof(wchar_t));
}

void IWritable::WriteWithLen(const wchar_t* s, size_t len){
    Write((int)len);
    Write(s, len * sizeof(wchar_t));
}

void IWritable::WriteWithLen(const String& s){
    WriteWithLen(s.GetString(), s.GetLength());
}

void IWritable::WriteWithLen(const WString& s){
    WriteWithLen(s.GetString(), s.GetLength());
}

}