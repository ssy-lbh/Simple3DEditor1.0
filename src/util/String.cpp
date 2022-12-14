#include <util/String.h>

#include <cstring>
#include <windows.h>

#include <util/os/Log.h>
#include <util/os/System.h>
#include <util/os/Font.h>

namespace simple3deditor {

String::String(){
    str = new char[1];
    str[0] = '\0';
    len = 0;
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
    WideCharToBytes(s, wcslen(s), str, len);
}

String::String(const wchar_t* s, size_t size){
    WideCharToBytes(s, len, str, len);
}

String::String(String &&s){
    len = s.len;
    str = new char[len + 1];
    str[len] = '\0';
    memcpy(str, s.str, len * sizeof(char));
}

String::String(WString &&s){
    WideCharToBytes(s.GetString(), s.GetLength(), str, len);
}

String::String(const String &s){
    len = s.len;
    str = new char[len + 1];
    str[len] = '\0';
    memcpy(str, s.str, len * sizeof(char));
}

String::String(const WString &s){
    WideCharToBytes(s.GetString(), s.GetLength(), str, len);
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
    WideCharToBytes(s, wcslen(s), str, len);
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
    WideCharToBytes(s.GetString(), s.GetLength(), str, len);
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
    WideCharToBytes(s.GetString(), s.GetLength(), str, len);
    return *this;
}

String::~String(){
    if (str) delete[] str;
}

char String::operator[](size_t index) const{
    if (index >= len){
        DebugError("Critical: String::operator[](%llu) When Index Overflow", index);
        return '\0';
    }
    return str[index];
}

String String::operator+(const String& s) const{
    String res(false);
    res.len = len + s.len;
    res.str = new char[res.len + 1];
    res.str[res.len] = '\0';
    memcpy(res.str, str, len * sizeof(char));
    memcpy(res.str + len, s.str, s.len * sizeof(char));
    return res;
}

String String::operator+(const char* s) const{
    String res(false);
    size_t size = strlen(s);
    res.len = len + size;
    res.str = new char[res.len + 1];
    res.str[res.len] = '\0';
    memcpy(res.str, str, len * sizeof(char));
    memcpy(res.str + len, s, size * sizeof(char));
    return res;
}

bool String::operator==(const String& s) const{
    return strcmp(str, s.str) == 0;
}

bool String::operator!=(const String& s) const{
    return strcmp(str, s.str) != 0;
}

bool String::operator<(const String& s) const{
    return strcmp(str, s.str) < 0;
}

bool String::operator>(const String& s) const{
    return strcmp(str, s.str) > 0;
}

bool String::operator<=(const String& s) const{
    return strcmp(str, s.str) <= 0;
}

bool String::operator>=(const String& s) const{
    return strcmp(str, s.str) >= 0;
}

bool String::operator==(const char* s) const{
    return strcmp(str, s) == 0;
}

bool String::operator!=(const char* s) const{
    return strcmp(str, s) != 0;
}

bool String::operator<(const char* s) const{
    return strcmp(str, s) < 0;
}

bool String::operator>(const char* s) const{
    return strcmp(str, s) > 0;
}

bool String::operator<=(const char* s) const{
    return strcmp(str, s) <= 0;
}

bool String::operator>=(const char* s) const{
    return strcmp(str, s) >= 0;
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

bool String::HasChar(char c) const{
    return strchr(str, c);
}

bool String::HasChars(const char* s) const{
    for (; *s; s++){
        if (strchr(str, *s))
            return true;
    }
    return false;
}

bool String::HasAllChars(const char* s) const{
    for (; *s; s++){
        if (!strchr(str, *s))
            return false;
    }
    return true;
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

size_t String::FindString(const String& s) const{
    char* pos = strstr(str, s.str);
    return pos ? pos - str : -1;
}

size_t String::FindString(const String& s, size_t beg) const{
    char* pos = strstr(str + beg, s.str);
    return pos ? pos - str : -1;
}

bool String::StartsWith(const String& s) const{
    if (s.len > len)
        return false;
    return strncmp(str, s.str, s.len) == 0;
}

bool String::EndsWith(const String& s) const{
    if (s.len > len)
        return false;
    size_t off = len - s.len;
    return strncmp(str + off, s.str, s.len) == 0;
}

bool String::IgnoreCaseEqual(const String& s) const{
    return stricmp(str, s.str) == 0;
}

bool String::IgnoreCaseNotEqual(const String& s) const{
    return stricmp(str, s.str) != 0;
}

bool String::IgnoreCaseLess(const String& s) const{
    return stricmp(str, s.str) < 0;
}

bool String::IgnoreCaseGreater(const String& s) const{
    return stricmp(str, s.str) > 0;
}

bool String::IgnoreCaseLessEqual(const String& s) const{
    return stricmp(str, s.str) <= 0;
}

bool String::IgnoreCaseGreaterEqual(const String& s) const{
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

size_t String::Count(const String& s) const{
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

size_t String::Split(const String& s, String* arr, size_t len) const{
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

WString::WString(){
    str = new wchar_t[1];
    str[0] = L'\0';
    len = 0;
}

WString::WString(wchar_t c){
    str = new wchar_t[2];
    str[0] = c;
    str[1] = L'\0';
    len = 1;
}

WString::WString(const char* s){
    BytesToWideChar(s, strlen(s), str, len);
}

WString::WString(const char* s, size_t size){
    BytesToWideChar(s, size, str, len);
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
    BytesToWideChar(s.GetString(), s.GetLength(), str, len);
}

WString::WString(WString &&s){
    str = new wchar_t[s.len + 1];
    len = s.len;
    memcpy(str, s.str, s.len * sizeof(wchar_t));
    str[s.len] = L'\0';
}

WString::WString(const String &s){
    BytesToWideChar(s.GetString(), s.GetLength(), str, len);
}

WString::WString(const WString &s){
    str = new wchar_t[s.len + 1];
    str[s.len] = L'\0';
    memcpy(str, s.str, s.len * sizeof(wchar_t));
    len = s.len;
}

WString &WString::operator=(const char* s){
    delete[] str;
    BytesToWideChar(s, strlen(s), str, len);
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
    BytesToWideChar(s.GetString(), s.GetLength(), str, len);
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
    BytesToWideChar(s.GetString(), s.GetLength(), str, len);
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
    if (str) delete[] str;
}

wchar_t WString::operator[](size_t index) const{
    if (index >= len){
        DebugError("Critical: WString::operator[](%llu) When Index Overflow", index);
        return L'\0';
    }
    return str[index];
}

WString WString::operator+(const WString& s) const{
    WString res(false);
    res.len = len + s.len;
    res.str = new wchar_t[res.len + 1];
    res.str[res.len] = L'\0';
    memcpy(res.str, str, len * sizeof(wchar_t));
    memcpy(res.str + len, s.str, s.len * sizeof(wchar_t));
    return res;
}

WString WString::operator+(const wchar_t* s) const{
    WString res(false);
    size_t size = wcslen(s);
    res.len = len + size;
    res.str = new wchar_t[res.len + 1];
    res.str[res.len] = L'\0';
    memcpy(res.str, str, len * sizeof(wchar_t));
    memcpy(res.str + len, s, size * sizeof(wchar_t));
    return res;
}

bool WString::operator==(const WString& s) const{
    return wcscmp(str, s.str) == 0;
}

bool WString::operator!=(const WString& s) const{
    return wcscmp(str, s.str) != 0;
}

bool WString::operator<(const WString& s) const{
    return wcscmp(str, s.str) < 0;
}

bool WString::operator>(const WString& s) const{
    return wcscmp(str, s.str) > 0;
}

bool WString::operator<=(const WString& s) const{
    return wcscmp(str, s.str) <= 0;
}

bool WString::operator>=(const WString& s) const{
    return wcscmp(str, s.str) >= 0;
}

bool WString::operator==(const wchar_t* s) const{
    return wcscmp(str, s) == 0;
}

bool WString::operator!=(const wchar_t* s) const{
    return wcscmp(str, s) != 0;
}

bool WString::operator<(const wchar_t* s) const{
    return wcscmp(str, s) < 0;
}

bool WString::operator>(const wchar_t* s) const{
    return wcscmp(str, s) > 0;
}

bool WString::operator<=(const wchar_t* s) const{
    return wcscmp(str, s) <= 0;
}

bool WString::operator>=(const wchar_t* s) const{
    return wcscmp(str, s) >= 0;
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

bool WString::HasChar(wchar_t c) const{
    return wcschr(str, c);
}

bool WString::HasChars(const wchar_t* s) const{
    for (; *s; s++){
        if (wcschr(str, *s))
            return true;
    }
    return false;
}

bool WString::HasAllChars(const wchar_t* s) const{
    for (; *s; s++){
        if (!wcschr(str, *s))
            return false;
    }
    return true;
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

size_t WString::FindString(const WString& s) const{
    wchar_t* pos = wcsstr(str, s.str);
    return pos ? pos - str : -1;
}

size_t WString::FindString(const WString& s, size_t beg) const{
    wchar_t* pos = wcsstr(str + beg, s.str);
    return pos ? pos - str : -1;
}

bool WString::IgnoreCaseEqual(const WString& s) const{
    return wcsicmp(str, s.str) == 0;
}

bool WString::IgnoreCaseNotEqual(const WString& s) const{
    return wcsicmp(str, s.str) != 0;
}

bool WString::IgnoreCaseLess(const WString& s) const{
    return wcsicmp(str, s.str) < 0;
}

bool WString::IgnoreCaseGreater(const WString& s) const{
    return wcsicmp(str, s.str) > 0;
}

bool WString::IgnoreCaseLessEqual(const WString& s) const{
    return wcsicmp(str, s.str) <= 0;
}

bool WString::IgnoreCaseGreaterEqual(const WString& s) const{
    return wcsicmp(str, s.str) >= 0;
}

bool WString::StartsWith(const WString& s) const{
    if (s.len > len)
        return false;
    return wcsncmp(str, s.str, s.len) == 0;
}

bool WString::EndsWith(const WString& s) const{
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

size_t WString::Count(const WString& s) const{
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

size_t WString::Split(const WString& s, WString* arr, size_t len) const{
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

PackString::PackString(String &&s) : String(false) {
    this->str = const_cast<char*>(s.GetString()); this->len = s.GetLength();
}

PackString::PackString(const String &s) : String(false) {
    this->str = const_cast<char*>(s.GetString()); this->len = s.GetLength();
}

PackString::PackString(const char* s, size_t len) : String(false) {
    this->str = const_cast<char*>(s); this->len = len;
}

PackString::~PackString(){
    str = NULL;
}

PackWString::PackWString(WString &&s) : WString(false) {
    this->str = const_cast<wchar_t*>(s.GetString()); this->len = s.GetLength();
}

PackWString::PackWString(const WString &s) : WString(false) {
    this->str = const_cast<wchar_t*>(s.GetString()); this->len = s.GetLength();
}

PackWString::PackWString(const wchar_t* s, size_t len) : WString(false) {
    this->str = const_cast<wchar_t*>(s); this->len = len;
}

PackWString::~PackWString(){
    str = NULL;
}

}