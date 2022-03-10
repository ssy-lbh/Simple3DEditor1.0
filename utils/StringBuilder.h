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

    void Clear();
    StringBuilder& Append(char c);
    StringBuilder& Append(int i);
    StringBuilder& Append(size_t s);
    StringBuilder& Append(bool b);
    StringBuilder& Append(float f);
    StringBuilder& Append(double d);
    StringBuilder& Append(String& s);
    StringBuilder& Append(WString& s);
    StringBuilder& Append(const char* s);
    StringBuilder& Append(const wchar_t* s);
};

#endif