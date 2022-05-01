#include <base/Appendable.h>

#include <cstring>

#include <util/String.h>

IAppendableA& IAppendableA::Append(const char* s){
    Append(s, strlen(s));
    return *this;
}

IAppendableA& IAppendableA::Append(char c){
    Append(&c, 1);
    return *this;
}

IAppendableA& IAppendableA::Append(const String& s){
    Append(s.GetString(), s.GetLength());
    return *this;
}

IAppendableW& IAppendableW::Append(const wchar_t* s){
    Append(s, wcslen(s));
    return *this;
}

IAppendableW& IAppendableW::Append(wchar_t c){
    Append(&c, 1);
    return *this;
}

IAppendableW& IAppendableW::Append(const WString& s){
    Append(s.GetString(), s.GetLength());
    return *this;
}