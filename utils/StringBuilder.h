#ifndef __UTILS_STRINGBUILDER__
#define __UTILS_STRINGBUILDER__

#include <define.h>

class StringBuilderA final : public Object {
private:
    char* data;
    size_t size;
    size_t ptr;

    void Check(size_t reserve);

public:
    StringBuilderA();
    StringBuilderA(size_t length);
    ~StringBuilderA();

    String ToString();

    const char* GetString();
    size_t GetLength();

    void Clear();
    StringBuilderA& Append(char c);
    StringBuilderA& Append(wchar_t c);
    StringBuilderA& Append(int i);
    StringBuilderA& Append(size_t s);
    StringBuilderA& Append(bool b);
    StringBuilderA& Append(float f);
    StringBuilderA& Append(double d);
    StringBuilderA& Append(String& s);
    StringBuilderA& Append(WString& s);
    StringBuilderA& Append(const char* s);
    StringBuilderA& Append(const wchar_t* s);
};

class StringBuilderW final : public Object {
private:
    wchar_t* data;
    size_t size;
    size_t ptr;

    void Check(size_t reserve);

public:
    StringBuilderW();
    StringBuilderW(size_t length);
    ~StringBuilderW();

    WString ToString();

    const wchar_t* GetString();
    size_t GetLength();

    void Clear();
    StringBuilderW& Append(char c);
    StringBuilderW& Append(wchar_t c);
    StringBuilderW& Append(int i);
    StringBuilderW& Append(size_t s);
    StringBuilderW& Append(bool b);
    StringBuilderW& Append(float f);
    StringBuilderW& Append(double d);
    StringBuilderW& Append(String& s);
    StringBuilderW& Append(WString& s);
    StringBuilderW& Append(const char* s);
    StringBuilderW& Append(const wchar_t* s);
};

#endif