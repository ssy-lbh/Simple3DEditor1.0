#include <utils/StringBuilder.h>

#include <stdio.h>
#include <cstring>

#include <utils/String.h>
#include <utils/os/Log.h>

StringBuilderA::StringBuilderA(){
    data = new char[32];
    size = 32;
    ptr = 0;
}

StringBuilderA::StringBuilderA(size_t length){
    data = new char[length];
    size = length;
    ptr = 0;
}

StringBuilderA::~StringBuilderA(){
    if (!data){
        DebugError("Critical: StringBuilderA::~StringBuilderA() When data Is NULL");
        return;
    }
    delete data;
}

void StringBuilderA::Check(size_t reserve){
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

String StringBuilderA::ToString(){
    return String(data, ptr);
}

const String StringBuilderA::ToPackString(){
    data[ptr] = '\0';
    return PackString(data, ptr);
}

StringBuilderA& StringBuilderA::Append(char c){
    Check(2);
    data[ptr++] = c;
    return *this;
}

StringBuilderA& StringBuilderA::Append(wchar_t c){
    Check(4);
    ptr += __builtin_snprintf(data + ptr, 3, "%C", c);
    return *this;
}

StringBuilderA& StringBuilderA::Append(int i){
    Check(12);
    ptr += __builtin_snprintf(data + ptr, 11, "%d", i);
    return *this;
}

StringBuilderA& StringBuilderA::Append(size_t s){
    Check(24);
    ptr += __builtin_snprintf(data + ptr, 23, "%llu", s);
    return *this;
}

StringBuilderA& StringBuilderA::Append(bool b){
    Check(6);
    ptr += __builtin_snprintf(data + ptr, 5, "%s", b ? "true" : "false");
    return *this;
}

StringBuilderA& StringBuilderA::Append(float f){
    Check(20);
    ptr += __builtin_snprintf(data + ptr, 19, "%g", f);
    return *this;
}

StringBuilderA& StringBuilderA::Append(double d){
    Check(20);
    ptr += __builtin_snprintf(data + ptr, 19, "%g", d);
    return *this;

}
StringBuilderA& StringBuilderA::Append(String& s){
    Check(s.GetLength() + 2);
    ptr += __builtin_snprintf(data + ptr, s.GetLength() + 1, "%s", s.GetString());
    return *this;
}

StringBuilderA& StringBuilderA::Append(WString& s){
    size_t len = s.GetLength() << 2;
    Check(len + 2);
    ptr += __builtin_snprintf(data + ptr, len + 1, "%s", s.GetString());
    return *this;
}

StringBuilderA& StringBuilderA::Append(const char* s){
    size_t len = strlen(s);
    Check(len + 2);
    ptr += __builtin_snprintf(data + ptr, len + 1, "%s", s);
    return *this;
}

StringBuilderA& StringBuilderA::Append(const wchar_t* s){
    size_t len = wcslen(s) << 2;
    Check(len + 2);
    ptr += __builtin_snprintf(data + ptr, len + 1, "%S", s);
    return *this;
}

StringBuilderW::StringBuilderW(){
    data = new wchar_t[32];
    size = 32;
    ptr = 0;
}

StringBuilderW::StringBuilderW(size_t length){
    data = new wchar_t[length];
    size = length;
    ptr = 0;
}

StringBuilderW::~StringBuilderW(){
    if (!data){
        DebugError("Critical: StringBuilderW::~StringBuilderW() When data Is NULL");
        return;
    }
    delete data;
}

void StringBuilderW::Check(size_t reserve){
    if (ptr + reserve < size)
        return;
    size <<= 1;
    while (ptr + reserve >= size)
        size <<= 1;
    wchar_t* newData = new wchar_t[size];
    memcpy(newData, data, ptr * sizeof(wchar_t));
    delete[] data;
    data = newData;
}

WString StringBuilderW::ToString(){
    return WString(data, ptr);
}

const WString StringBuilderW::ToPackString(){
    data[ptr] = L'\0';
    return PackWString(data, ptr);
}

StringBuilderW& StringBuilderW::Append(char c){
    Check(2);
    ptr += __mingw_snwprintf(data + ptr, 1, L"%c", c);
    return *this;
}

StringBuilderW& StringBuilderW::Append(wchar_t c){
    Check(2);
    data[ptr++] = c;
    return *this;
}

StringBuilderW& StringBuilderW::Append(int i){
    Check(12);
    ptr += __mingw_snwprintf(data + ptr, 11, L"%d", i);
    return *this;
}

StringBuilderW& StringBuilderW::Append(size_t s){
    Check(24);
    ptr += __mingw_snwprintf(data + ptr, 23, L"%llu", s);
    return *this;
}

StringBuilderW& StringBuilderW::Append(bool b){
    Check(6);
    ptr += __mingw_snwprintf(data + ptr, 5, L"%s", b ? "true" : "false");
    return *this;
}

StringBuilderW& StringBuilderW::Append(float f){
    Check(20);
    ptr += __mingw_snwprintf(data + ptr, 19, L"%g", f);
    return *this;
}

StringBuilderW& StringBuilderW::Append(double d){
    Check(20);
    ptr += __mingw_snwprintf(data + ptr, 19, L"%g", d);
    return *this;

}
StringBuilderW& StringBuilderW::Append(String& s){
    Check(s.GetLength() + 2);
    ptr += __mingw_snwprintf(data + ptr, s.GetLength() + 1, L"%s", s.GetString());
    return *this;
}

StringBuilderW& StringBuilderW::Append(WString& s){
    size_t len = s.GetLength();
    Check(len + 2);
    ptr += __mingw_snwprintf(data + ptr, len + 1, L"%s", s.GetString());
    return *this;
}

StringBuilderW& StringBuilderW::Append(const char* s){
    size_t len = strlen(s);
    Check(len + 2);
    ptr += __mingw_snwprintf(data + ptr, len + 1, L"%s", s);
    return *this;
}

StringBuilderW& StringBuilderW::Append(const wchar_t* s){
    size_t len = wcslen(s);
    Check(len + 2);
    ptr += __mingw_snwprintf(data + ptr, len + 1, L"%S", s);
    return *this;
}