#ifndef __BASE_APPENDABLE__
#define __BASE_APPENDABLE__

#include <define.h>

class IAppendableA {
public:
    virtual IAppendableA& Append(const char* s);
    virtual IAppendableA& Append(const char* s, size_t len) = 0;
    virtual IAppendableA& Append(char c);
    virtual IAppendableA& Append(const String& s);
};

class IAppendableW {
public:
    virtual IAppendableW& Append(const wchar_t* s);
    virtual IAppendableW& Append(const wchar_t* s, size_t len) = 0;
    virtual IAppendableW& Append(wchar_t c);
    virtual IAppendableW& Append(const WString& s);
};

class IAppendable : public IAppendableA, public IAppendableW {
public:
    using IAppendableA::Append;
    using IAppendableW::Append;
};

#endif