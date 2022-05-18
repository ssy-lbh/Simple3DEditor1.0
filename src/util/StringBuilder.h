#ifndef __UTIL_STRINGBUILDER__
#define __UTIL_STRINGBUILDER__

#include <define.h>

#include <base/Appendable.h>

class StringBuilderA final : public Object, public IAppendable {
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

    StringBuilderA& Append(int i);
    StringBuilderA& Append(size_t s);
    StringBuilderA& Append(bool b);
    StringBuilderA& Append(float f);
    StringBuilderA& Append(double d);

    virtual StringBuilderA& Append(char c) override;
    virtual StringBuilderA& Append(wchar_t c) override;
    virtual StringBuilderA& Append(const char* s, size_t len) override;
    virtual StringBuilderA& Append(const wchar_t* s, size_t len) override;

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

class StringBuilderW final : public Object, public IAppendable {
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

    StringBuilderW& Append(int i);
    StringBuilderW& Append(size_t s);
    StringBuilderW& Append(bool b);
    StringBuilderW& Append(float f);
    StringBuilderW& Append(double d);

    virtual StringBuilderW& Append(char c) override;
    virtual StringBuilderW& Append(wchar_t c) override;
    virtual StringBuilderW& Append(const char* s, size_t len) override;
    virtual StringBuilderW& Append(const wchar_t* s, size_t len) override;

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