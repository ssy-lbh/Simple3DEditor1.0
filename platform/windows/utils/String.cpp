#include <utils/String.h>

#include <windows.h>

#include <utils/os/Log.h>

String::String(bool alloc){
    if (alloc){
        str = new char[1];
        str[0] = '\0';
        len = 0;
    }
}

String::String(){
    str = new char[1];
    str[0] = '\0';
    len = 0;
}

String::String(int resid){
    const char* pstr;
    len = LoadStringA(GetModuleHandleA(NULL), resid, (LPSTR)&pstr, 0);
    if (!pstr){
        DebugError("Critical: String::String(%d) LoadStringA Return NULL", resid);
        str = new char[1];
        str[0] = '\0';
        len = 0;
        return;
    }
    str = new char[len + 1];
    str[len] = '\0';
    memcpy(str, pstr, len * sizeof(char));
}

String::String(char c){
    str = new char[2];
    str[0] = c;
    str[1] = '\0';
    len = 1;
}

String::String(const char* s){
    len = strlen(s);
    str = new char[len + 1];
    str[len] = '\0';
    strncpy(str, s, len);
}

String::String(const char* s, size_t size){
    len = size;
    str = new char[len + 1];
    str[len] = '\0';
    memcpy(str, s, len * sizeof(char));
}

String::String(const wchar_t* s){
    len = WideCharToMultiByte(CP_ACP, 0, s, -1, NULL, 0, NULL, NULL);
    str = new char[len + 1];
    WideCharToMultiByte(CP_ACP, 0, s, -1, str, len, NULL, NULL);
    str[len] = '\0';
}

String::String(const wchar_t* s, size_t size){
    len = WideCharToMultiByte(CP_ACP, 0, s, size, NULL, 0, NULL, NULL);
    str = new char[len + 1];
    WideCharToMultiByte(CP_ACP, 0, s, -1, str, len, NULL, NULL);
    str[len] = '\0';
}

String::String(String &&s){
    len = s.len;
    str = new char[len + 1];
    str[len] = '\0';
    memcpy(str, s.str, len * sizeof(char));
}

String::String(WString &&s){
    len = WideCharToMultiByte(CP_ACP, 0, s.GetString(), s.GetLength(), NULL, 0, NULL, NULL);
    str = new char[len + 1];
    WideCharToMultiByte(CP_ACP, 0, s.GetString(), s.GetLength(), str, len, NULL, NULL);
    str[len] = '\0';
}

String::String(const String &s){
    len = s.len;
    str = new char[len + 1];
    str[len] = '\0';
    memcpy(str, s.str, len * sizeof(char));
}

String::String(const WString &s){
    len = WideCharToMultiByte(CP_ACP, 0, s.GetString(), s.GetLength(), NULL, 0, NULL, NULL);
    str = new char[len + 1];
    WideCharToMultiByte(CP_ACP, 0, s.GetString(), s.GetLength(), str, len, NULL, NULL);
    str[len] = '\0';
}

String &String::operator=(const char* s){
    delete[] str;
    len = strlen(s);
    str = new char[len + 1];
    str[len] = '\0';
    strncpy(str, s, len);
    return *this;
}

String &String::operator=(const wchar_t* s){
    delete[] str;
    len = WideCharToMultiByte(CP_ACP, 0, s, -1, NULL, 0, NULL, NULL);
    str = new char[len + 1];
    WideCharToMultiByte(CP_ACP, 0, s, -1, str, len, NULL, NULL);
    str[len] = '\0';
    return *this;
}

String &String::operator=(String &&s){
    delete[] str;
    str = new char[s.len + 1];
    memcpy(str, s.str, len * sizeof(char));
    str[s.len] = '\0';
    len = s.len;
    return *this;
}

String &String::operator=(WString &&s){
    delete[] str;
    len = WideCharToMultiByte(CP_ACP, 0, s.GetString(), s.GetLength(), NULL, 0, NULL, NULL);
    str = new char[len + 1];
    WideCharToMultiByte(CP_ACP, 0, s.GetString(), s.GetLength(), str, len, NULL, NULL);
    str[len] = '\0';
    return *this;
}

String &String::operator=(const String &s){
    delete[] str;
    str = new char[s.len + 1];
    memcpy(str, s.str, len * sizeof(char));
    str[s.len] = '\0';
    len = s.len;
    return *this;
}

String &String::operator=(const WString &s){
    delete[] str;
    len = WideCharToMultiByte(CP_ACP, 0, s.GetString(), s.GetLength(), NULL, 0, NULL, NULL);
    str = new char[len + 1];
    WideCharToMultiByte(CP_ACP, 0, s.GetString(), s.GetLength(), str, len, NULL, NULL);
    str[len] = '\0';
    return *this;
}

String::~String(){
    if (!str){
        DebugError("Critical: String::~String() When str Is NULL");
        return;
    }
    delete[] str;
}

char String::operator[](size_t index) const{
    if (index >= len){
        DebugError("Critical: String::operator[](%llu) When Index Overflow", index);
        return '\0';
    }
    return str[index];
}

String String::operator+(String s) const{
    String res(false);
    res.len = len + s.len;
    res.str = new char[res.len + 1];
    res.str[res.len] = '\0';
    memcpy(res.str, str, len * sizeof(char));
    memcpy(res.str + len, s.str, s.len * sizeof(char));
    return res;
}

bool String::operator==(const String s) const{
    return strcmp(str, s.str) == 0;
}

bool String::operator!=(const String s) const{
    return strcmp(str, s.str) != 0;
}

bool String::operator<(const String s) const{
    return strcmp(str, s.str) < 0;
}

bool String::operator>(const String s) const{
    return strcmp(str, s.str) > 0;
}

bool String::operator<=(const String s) const{
    return strcmp(str, s.str) <= 0;
}

bool String::operator>=(const String s) const{
    return strcmp(str, s.str) >= 0;
}

const char* String::GetString() const{
    return str;
}

size_t String::GetLength() const{
    return len;
}

char String::CharAt(size_t index) const{
    if (index >= len){
        DebugError("Critical: String::CharAt(%llu) When Index Overflow", index);
        return '\0';
    }
    return str[index];
}

String String::Reverse() const{
    String res(false);
    res.str = new char[len + 1];
    res.str[len] = '\0';
    res.len = len;
    strrev(res.str);
    return res;
}

String String::SubString(size_t start) const{
    if (start >= len)
        return String();
    String res(false);
    res.len = len - start;
    res.str = new char[res.len];
    res.str[res.len] = '\0';
    memcpy(res.str, str + start, res.len * sizeof(char));
    return res;
}

String String::SubString(size_t start, size_t end) const{
    end = (len > end ? end : len);
    if (start >= end)
        return String();
    String res(false);
    res.len = end - start;
    res.str = new char[res.len];
    res.str[res.len] = '\0';
    memcpy(res.str, str + start, res.len * sizeof(char));
    return res;
}

size_t String::FindChar(char c) const{
    char* pos = strchr(str, c);
    return pos ? pos - str : -1;
}

size_t String::FindChar(char c, size_t beg) const{
    char* pos = strchr(str + beg, c);
    return pos ? pos - str : -1;
}

size_t String::FindRevChar(char c) const{
    char* pos = strrchr(str, c);
    return pos ? pos - str : -1;
}

size_t String::FindRevChar(char c, size_t beg) const{
    for (size_t i = beg; i != -1; i--){
        if (str[i] == c)
            return i;
    }
    return -1;
}

size_t String::FindString(const char* s) const{
    char* pos = strstr(str, s);
    return pos ? pos - str : -1;
}

size_t String::FindString(const char* s, size_t beg) const{
    char* pos = strstr(str + beg, s);
    return pos ? pos - str : -1;
}

size_t String::FindString(const String s) const{
    char* pos = strstr(str, s.str);
    return pos ? pos - str : -1;
}

size_t String::FindString(const String s, size_t beg) const{
    char* pos = strstr(str + beg, s.str);
    return pos ? pos - str : -1;
}

bool String::StartsWith(const String s) const{
    if (s.len > len)
        return false;
    return strncmp(str, s.str, s.len) == 0;
}

bool String::EndsWith(const String s) const{
    if (s.len > len)
        return false;
    size_t off = len - s.len;
    return strncmp(str + off, s.str, s.len) == 0;
}

bool String::IgnoreCaseEqual(const String s) const{
    return stricmp(str, s.str) == 0;
}

bool String::IgnoreCaseNotEqual(const String s) const{
    return stricmp(str, s.str) != 0;
}

bool String::IgnoreCaseLess(const String s) const{
    return stricmp(str, s.str) < 0;
}

bool String::IgnoreCaseGreater(const String s) const{
    return stricmp(str, s.str) > 0;
}

bool String::IgnoreCaseLessEqual(const String s) const{
    return stricmp(str, s.str) <= 0;
}

bool String::IgnoreCaseGreaterEqual(const String s) const{
    return stricmp(str, s.str) >= 0;
}

String String::ToLowerCase() const{
    String res(false);
    res.str = new char[len + 1];
    res.str[len] = '\0';
    res.len = len;
    for (size_t i = 0; i < len; i++)
        res.str[i] = ((str[i] >= 'A' && str[i] <= 'Z') ? str[i] + ('a' - 'A') : str[i]);
    return res;
}

String String::ToUpperCase() const{
    String res(false);
    res.str = new char[len + 1];
    res.str[len] = '\0';
    res.len = len;
    for (size_t i = 0; i < len; i++)
        res.str[i] = ((str[i] >= 'a' && str[i] <= 'z') ? str[i] + ('A' - 'a') : str[i]);
    return res;
}

size_t String::Count(char c) const{
    size_t cnt = 0;
    for (size_t i = 0; str[i]; i++)
        if (str[i] == c)
            cnt++;
    return cnt;
}

size_t String::Count(const char* s) const{
    size_t cnt = 0;
    size_t slen = strlen(s);
    char* pstr = str;
    while (pstr = strstr(pstr, s)){
        pstr += slen;
        cnt++;
    }
    return cnt;
}

size_t String::Count(const String s) const{
    size_t cnt = 0;
    char* pstr = str;
    while (pstr = strstr(pstr, s.str)){
        pstr += s.GetLength();
        cnt++;
    }
    return cnt;
}

size_t String::Spilt(char c, String* arr, size_t len) const{
    size_t p = 0;
    if (len == 0)
        return Count(c) + 1;
    for (size_t i = 0; i < len; i++){
        size_t pos = FindChar(c);
        if (pos == -1){
            arr[i] = String(str + p, len - p);
            return i + 1;
        }
        arr[i] = String(str + p, pos - p);
        p = pos + 1;
    }
    return Count(c) + 1;
}

size_t String::Split(const char* s, String* arr, size_t len) const{
    size_t p = 0;
    size_t slen;
    if (len == 0)
        return Count(s) + 1;
    slen = strlen(s);
    for (size_t i = 0; i < len; i++){
        size_t pos = FindString(s);
        if (pos == -1){
            arr[i] = String(str + p, len - p);
            return i + 1;
        }
        arr[i] = String(str + p, pos - p);
        p = pos + slen;
    }
    return Count(s) + 1;
}

size_t String::Split(const String s, String* arr, size_t len) const{
    size_t p = 0;
    if (len == 0)
        return Count(s) + 1;
    for (size_t i = 0; i < len; i++){
        size_t pos = FindString(s);
        if (pos == -1){
            arr[i] = String(str + p, len - p);
            return i + 1;
        }
        arr[i] = String(str + p, pos - p);
        p = pos + s.GetLength();
    }
    return Count(s) + 1;
}

WString::WString(bool alloc){
    if (alloc){
        str = new wchar_t[1];
        str[0] = L'\0';
        len = 0;
    }
}

WString::WString(){
    str = new wchar_t[1];
    str[0] = L'\0';
    len = 0;
}

WString::WString(int resid){
    const wchar_t* pstr;
    len = LoadStringW(GetModuleHandleW(NULL), resid, (LPWSTR)&pstr, 0);
    if (!pstr){
        DebugError("Critical: WString::WString(%d) LoadStringW Return NULL", resid);
        str = new wchar_t[1];
        str[0] = L'\0';
        len = 0;
        return;
    }
    str = new wchar_t[len + 1];
    str[len] = L'\0';
    memcpy(str, pstr, len * sizeof(wchar_t));
}

WString::WString(wchar_t c){
    str = new wchar_t[2];
    str[0] = c;
    str[1] = L'\0';
    len = 1;
}

WString::WString(const char* s){
    len = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s, -1, NULL, 0);
    str = new wchar_t[len + 1];
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s, -1, str, len);
    str[len] = L'\0';
}

WString::WString(const char* s, size_t size){
    len = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s, size, NULL, 0);
    str = new wchar_t[len + 1];
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s, -1, str, len);
    str[len] = L'\0';
}

WString::WString(const wchar_t* s){
    len = wcslen(s);
    str = new wchar_t[len + 1];
    str[len] = L'\0';
    wcsncpy(str, s, len);
}

WString::WString(const wchar_t* s, size_t size){
    len = size;
    str = new wchar_t[len + 1];
    str[len] = L'\0';
    memcpy(str, s, len * sizeof(wchar_t));
}

WString::WString(String &&s){
    len = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s.GetString(), s.GetLength(), NULL, 0);
    str = new wchar_t[len + 1];
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s.GetString(), s.GetLength(), str, len);
    str[len] = L'\0';
}

WString::WString(WString &&s){
    str = new wchar_t[s.len + 1];
    len = s.len;
    memcpy(str, s.str, s.len * sizeof(wchar_t));
    str[s.len] = L'\0';
}

WString::WString(const String &s){
    len = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s.GetString(), s.GetLength(), NULL, 0);
    str = new wchar_t[len + 1];
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s.GetString(), s.GetLength(), str, len);
    str[len] = L'\0';
}

WString::WString(const WString &s){
    str = new wchar_t[s.len + 1];
    str[s.len] = L'\0';
    memcpy(str, s.str, s.len * sizeof(wchar_t));
    len = s.len;
}

WString &WString::operator=(const char* s){
    delete[] str;
    len = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s, -1, NULL, 0);
    str = new wchar_t[len + 1];
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s, -1, str, len);
    str[len] = L'\0';
    return *this;
}

WString &WString::operator=(const wchar_t* s){
    delete[] str;
    len = wcslen(s);
    str = new wchar_t[len + 1];
    str[len] = L'\0';
    wcsncpy(str, s, len);
    return *this;
}

WString &WString::operator=(String &&s){
    delete[] str;
    len = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s.GetString(), s.GetLength(), NULL, 0);
    str = new wchar_t[len + 1];
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s.GetString(), s.GetLength(), str, len);
    str[len] = L'\0';
    return *this;
}

WString &WString::operator=(WString &&s){
    delete[] str;
    str = new wchar_t[s.len + 1];
    memcpy(str, s.str, s.len * sizeof(wchar_t));
    str[s.len] = L'\0';
    len = s.len;
    return *this;
}

WString &WString::operator=(const String &s){
    delete[] str;
    len = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s.GetString(), s.GetLength(), NULL, 0);
    str = new wchar_t[len + 1];
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s.GetString(), s.GetLength(), str, len);
    str[len] = L'\0';
    return *this;
}

WString &WString::operator=(const WString &s){
    delete[] str;
    str = new wchar_t[s.len + 1];
    memcpy(str, s.str, s.len * sizeof(wchar_t));
    str[s.len] = L'\0';
    len = s.len;
    return *this;
}

WString::~WString(){
    if (!str){
        DebugError("Critical: WString::~WString() When str Is NULL");
        return;
    }
    delete[] str;
}

wchar_t WString::operator[](size_t index) const{
    if (index >= len){
        DebugError("Critical: WString::operator[](%llu) When Index Overflow", index);
        return L'\0';
    }
    return str[index];
}

WString WString::operator+(WString s) const{
    WString res;
    delete[] res.str;
    res.len = len + s.len;
    res.str = new wchar_t[res.len + 1];
    res.str[res.len] = L'\0';
    memcpy(res.str, str, len * sizeof(wchar_t));
    memcpy(res.str + len, s.str, s.len * sizeof(wchar_t));
    return res;
}

bool WString::operator==(const WString s) const{
    return wcscmp(str, s.str) == 0;
}

bool WString::operator!=(const WString s) const{
    return wcscmp(str, s.str) != 0;
}

bool WString::operator<(const WString s) const{
    return wcscmp(str, s.str) < 0;
}

bool WString::operator>(const WString s) const{
    return wcscmp(str, s.str) > 0;
}

bool WString::operator<=(const WString s) const{
    return wcscmp(str, s.str) <= 0;
}

bool WString::operator>=(const WString s) const{
    return wcscmp(str, s.str) >= 0;
}

const wchar_t* WString::GetString() const{
    return str;
}

size_t WString::GetLength() const{
    return len;
}

wchar_t WString::CharAt(size_t index) const{
    if (index >= len){
        DebugError("Critical: WString::CharAt(%llu) When Index Overflow", index);
        return L'\0';
    }
    return str[index];
}

WString WString::Reverse() const{
    WString res(false);
    res.str = new wchar_t[len + 1];
    res.str[len] = L'\0';
    res.len = len;
    wcsrev(res.str);
    return res;
}

WString WString::SubString(size_t start) const{
    if (start >= len)
        return WString();
    WString res(false);
    res.len = len - start;
    res.str = new wchar_t[res.len];
    res.str[res.len] = L'\0';
    memcpy(res.str, str + start, res.len * sizeof(wchar_t));
    return res;
}

WString WString::SubString(size_t start, size_t end) const{
    end = (len > end ? end : len);
    if (start >= end)
        return WString();
    WString res(false);
    res.len = end - start;
    res.str = new wchar_t[res.len];
    res.str[res.len] = L'\0';
    memcpy(res.str, str + start, res.len * sizeof(wchar_t));
    return res;
}

size_t WString::FindChar(wchar_t c) const{
    wchar_t* pos = wcschr(str, c);
    return pos ? pos - str : -1;
}

size_t WString::FindChar(wchar_t c, size_t beg) const{
    wchar_t* pos = wcschr(str + beg, c);
    return pos ? pos - str : -1;
}

size_t WString::FindRevChar(wchar_t c) const{
    wchar_t* pos = wcsrchr(str, c);
    return pos ? pos - str : -1;
}

size_t WString::FindRevChar(wchar_t c, size_t beg) const{
    for (size_t i = beg; i != -1; i--){
        if (str[i] == c)
            return i;
    }
    return -1;
}

size_t WString::FindString(const wchar_t* s) const{
    wchar_t* pos = wcsstr(str, s);
    return pos ? pos - str : -1;
}

size_t WString::FindString(const wchar_t* s, size_t beg) const{
    wchar_t* pos = wcsstr(str + beg, s);
    return pos ? pos - str : -1;
}

size_t WString::FindString(const WString s) const{
    wchar_t* pos = wcsstr(str, s.str);
    return pos ? pos - str : -1;
}

size_t WString::FindString(const WString s, size_t beg) const{
    wchar_t* pos = wcsstr(str + beg, s.str);
    return pos ? pos - str : -1;
}

bool WString::IgnoreCaseEqual(const WString s) const{
    return wcsicmp(str, s.str) == 0;
}

bool WString::IgnoreCaseNotEqual(const WString s) const{
    return wcsicmp(str, s.str) != 0;
}

bool WString::IgnoreCaseLess(const WString s) const{
    return wcsicmp(str, s.str) < 0;
}

bool WString::IgnoreCaseGreater(const WString s) const{
    return wcsicmp(str, s.str) > 0;
}

bool WString::IgnoreCaseLessEqual(const WString s) const{
    return wcsicmp(str, s.str) <= 0;
}

bool WString::IgnoreCaseGreaterEqual(const WString s) const{
    return wcsicmp(str, s.str) >= 0;
}

bool WString::StartsWith(const WString s) const{
    if (s.len > len)
        return false;
    return wcsncmp(str, s.str, s.len) == 0;
}

bool WString::EndsWith(const WString s) const{
    if (s.len > len)
        return false;
    size_t off = len - s.len;
    return wcsncmp(str + off, s.str, s.len) == 0;
}

WString WString::ToLowerCase() const{
    WString res(false);
    res.str = new wchar_t[len + 1];
    res.str[len] = L'\0';
    res.len = len;
    for (size_t i = 0; i < len; i++)
        res.str[i] = ((str[i] >= L'A' && str[i] <= L'Z') ? str[i] + (L'a' - L'A') : str[i]);
    return res;
}

WString WString::ToUpperCase() const{
    WString res(false);
    res.str = new wchar_t[len + 1];
    res.str[len] = L'\0';
    res.len = len;
    for (size_t i = 0; i < len; i++)
        res.str[i] = ((str[i] >= L'a' && str[i] <= L'z') ? str[i] + (L'A' - L'a') : str[i]);
    return res;
}

size_t WString::Count(wchar_t c) const{
    size_t cnt = 0;
    for (size_t i = 0; str[i]; i++)
        if (str[i] == c)
            cnt++;
    return cnt;
}

size_t WString::Count(const wchar_t* s) const{
    size_t cnt = 0;
    size_t slen = wcslen(s);
    wchar_t* pstr = str;
    while (pstr = wcsstr(pstr, s)){
        pstr += slen;
        cnt++;
    }
    return cnt;
}

size_t WString::Count(const WString s) const{
    size_t cnt = 0;
    wchar_t* pstr = str;
    while (pstr = wcsstr(pstr, s.str)){
        pstr += s.GetLength();
        cnt++;
    }
    return cnt;
}

size_t WString::Spilt(wchar_t c, WString* arr, size_t len) const{
    size_t p = 0;
    if (len == 0)
        return Count(c) + 1;
    for (size_t i = 0; i < len; i++){
        size_t pos = FindChar(c);
        if (pos == -1){
            arr[i] = WString(str + p, len - p);
            return i + 1;
        }
        arr[i] = WString(str + p, pos - p);
        p = pos + 1;
    }
    return Count(c) + 1;
}

size_t WString::Split(const wchar_t* s, WString* arr, size_t len) const{
    size_t p = 0;
    size_t slen;
    if (len == 0)
        return Count(s) + 1;
    slen = wcslen(s);
    for (size_t i = 0; i < len; i++){
        size_t pos = FindString(s);
        if (pos == -1){
            arr[i] = WString(str + p, len - p);
            return i + 1;
        }
        arr[i] = WString(str + p, pos - p);
        p = pos + slen;
    }
    return Count(s) + 1;
}

size_t WString::Split(const WString s, WString* arr, size_t len) const{
    size_t p = 0;
    if (len == 0)
        return Count(s) + 1;
    for (size_t i = 0; i < len; i++){
        size_t pos = FindString(s);
        if (pos == -1){
            arr[i] = WString(str + p, len - p);
            return i + 1;
        }
        arr[i] = WString(str + p, pos - p);
        p = pos + s.GetLength();
    }
    return Count(s) + 1;
}