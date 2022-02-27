#include <utils/StringBuilder.h>

#include <windows.h>

#include <utils/String.h>
#include <utils/os/Log.h>

StringBuilder::StringBuilder(){
    data = new char[32];
    size = 32;
    ptr = 0;
}

StringBuilder::StringBuilder(size_t length){
    data = new char[length];
    size = length;
    ptr = 0;
}

StringBuilder::~StringBuilder(){
    if (!data){
        DebugError("Critical: StringBuilder::~StringBuilder() When data Is NULL");
        return;
    }
    delete data;
}

void StringBuilder::Check(size_t reserve){
    if (ptr + reserve < size)
        return;
    size <<= 1;
    while (ptr + reserve >= size)
        size <<= 1;
    data = (char*)realloc(data, size * sizeof(char));
}

String StringBuilder::ToString(){
    return String(data, ptr);
}

const char* StringBuilder::GetString(){
    data[ptr] = '\0';
    return data;
}

size_t StringBuilder::GetLength(){
    return ptr;
}

void StringBuilder::Append(char c){
    Check(2);
    data[ptr++] = c;
}

void StringBuilder::Append(int i){
    Check(12);
    ptr += __builtin_snprintf(data + ptr, 11, "%d", i);
}

void StringBuilder::Append(bool b){
    Check(6);
    ptr += __builtin_snprintf(data + ptr, 5, "%s", b ? "true" : "false");
}

void StringBuilder::Append(float f){
    Check(20);
    ptr += __builtin_snprintf(data + ptr, 19, "%g", f);
}

void StringBuilder::Append(double d){
    Check(20);
    ptr += __builtin_snprintf(data + ptr, 19, "%g", d);

}
void StringBuilder::Append(String s){
    Check(s.GetLength() + 2);
    ptr += __builtin_snprintf(data + ptr, s.GetLength() + 1, "%s", s.GetString());
}

void StringBuilder::Append(const char* s){
    size_t len = strlen(s);
    Check(len + 2);
    ptr += __builtin_snprintf(data + ptr, len + 1, "%s", s);
}

void StringBuilder::Append(const wchar_t* s){
    size_t len = wcslen(s);
    Check(len + 2);
    ptr += __builtin_snprintf(data + ptr, len + 1, "%S", s);
}