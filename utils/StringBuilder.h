#ifndef __UTILS_STRINGBUILDER__
#define __UTILS_STRINGBUILDER__

#include <define.h>

class StringBuilder final : public Object {
private:
    char* data;
    size_t size;
    size_t ptr;

    void Check(size_t reserve);

public:
    StringBuilder();
    StringBuilder(size_t length);
    ~StringBuilder();

    String ToString();

    const char* GetString();
    size_t GetLength();

    void Append(char c);
    void Append(int i);
    void Append(bool b);
    void Append(float f);
    void Append(double d);
    void Append(String s);
    void Append(const char* s);
    void Append(const wchar_t* s);
};

#endif