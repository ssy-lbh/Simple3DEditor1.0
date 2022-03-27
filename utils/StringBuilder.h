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
    const String ToPackString();

    inline const char* GetString(){
        data[ptr] = '\0';
        return data;
    }

    inline size_t GetLength(){
        return ptr;
    }

    void SetLength(size_t size);

    inline void Clear(){
        ptr = 0;
    }

    StringBuilderA& Append(char c);
    StringBuilderA& Append(wchar_t c);
    StringBuilderA& Append(int i);
    StringBuilderA& Append(size_t s);
    StringBuilderA& Append(bool b);
    StringBuilderA& Append(float f);
    StringBuilderA& Append(double d);
    StringBuilderA& Append(const String& s);
    StringBuilderA& Append(const WString& s);
    StringBuilderA& Append(const char* s);
    StringBuilderA& Append(const wchar_t* s);

    StringBuilderA& operator+=(char c);
    StringBuilderA& operator+=(wchar_t c);
    StringBuilderA& operator+=(int i);
    StringBuilderA& operator+=(size_t s);
    StringBuilderA& operator+=(bool b);
    StringBuilderA& operator+=(float f);
    StringBuilderA& operator+=(double d);
    StringBuilderA& operator+=(const String& s);
    StringBuilderA& operator+=(const WString& s);
    StringBuilderA& operator+=(const char* s);
    StringBuilderA& operator+=(const wchar_t* s);
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
    const WString ToPackString();

    inline const wchar_t* GetString(){
        data[ptr] = L'\0';
        return data;
    }

    inline size_t GetLength(){
        return ptr;
    }

    void SetLength(size_t size);

    inline void Clear(){
        ptr = 0;
    }

    StringBuilderW& Append(char c);
    StringBuilderW& Append(wchar_t c);
    StringBuilderW& Append(int i);
    StringBuilderW& Append(size_t s);
    StringBuilderW& Append(bool b);
    StringBuilderW& Append(float f);
    StringBuilderW& Append(double d);
    StringBuilderW& Append(const String& s);
    StringBuilderW& Append(const WString& s);
    StringBuilderW& Append(const char* s);
    StringBuilderW& Append(const wchar_t* s);

    StringBuilderW& operator+=(char c);
    StringBuilderW& operator+=(wchar_t c);
    StringBuilderW& operator+=(int i);
    StringBuilderW& operator+=(size_t s);
    StringBuilderW& operator+=(bool b);
    StringBuilderW& operator+=(float f);
    StringBuilderW& operator+=(double d);
    StringBuilderW& operator+=(const String& s);
    StringBuilderW& operator+=(const WString& s);
    StringBuilderW& operator+=(const char* s);
    StringBuilderW& operator+=(const wchar_t* s);
};

#endif