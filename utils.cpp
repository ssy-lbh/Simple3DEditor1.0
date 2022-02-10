#include "utils.h"

#include <windows.h>

#include "log.h"

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

String::String(String &&s){
    str = new char[s.len + 1];
    len = s.len;
    strncpy(str, s.str, s.len);
    str[s.len] = '\0';
}

String::String(const String &s){
    str = new char[s.len + 1];
    str[s.len] = '\0';
    strncpy(str, s.str, s.len);
    len = s.len;
}

String &String::operator=(const char* s){
    delete str;
    len = strlen(s);
    str = new char[len + 1];
    str[len] = '\0';
    strncpy(str, s, len);
    return *this;
}

String &String::operator=(String &&s){
    delete str;
    str = new char[s.len + 1];
    strncpy(str, s.str, s.len);
    str[s.len] = '\0';
    len = s.len;
    return *this;
}

String &String::operator=(const String &s){
    delete str;
    str = new char[s.len + 1];
    strncpy(str, s.str, s.len);
    str[s.len] = '\0';
    len = s.len;
    return *this;
}

String::~String(){
    delete str;
}

char &String::operator[](size_t index){
    if (index >= len){
        DebugError("Critical: String::operator[](%llu) When Index Overflow", index);
    }
    return str[index];
}

String String::operator+(String s) const{
    String res;
    delete res.str;
    res.len = len + s.len;
    res.str = new char[res.len + 1];
    res.str[res.len] = '\0';
    strncpy(res.str, str, len);
    strncpy(res.str + len, s.str, s.len);
    return res;
}

String &String::operator+=(String s){
    size_t newLen = len + s.len;
    char* newStr = new char[newLen + 1];
    newStr[newLen] = '\0';
    strncpy(newStr, str, len);
    strncpy(newStr + len, s.str, s.len);
    delete str;
    str = newStr;
    len = newLen;
    return *this;
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
    }
    return str[index];
}

void String::Reverse(){
    strrev(str);
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

bool String::IgnoreCaseEqual(String s) const{
    return stricmp(str, s.str) == 0;
}

bool String::IgnoreCaseNotEqual(String s) const{
    return stricmp(str, s.str) != 0;
}

bool String::IgnoreCaseLess(String s) const{
    return stricmp(str, s.str) < 0;
}

bool String::IgnoreCaseGreater(String s) const{
    return stricmp(str, s.str) > 0;
}

bool String::IgnoreCaseLessEqual(String s) const{
    return stricmp(str, s.str) <= 0;
}

bool String::IgnoreCaseGreaterEqual(String s) const{
    return stricmp(str, s.str) >= 0;
}

void String::ToLowerCase(){
    for (size_t i = 0; str[i]; i++)
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] += ('a' - 'A');
}

void String::ToUpperCase(){
    for (size_t i = 0; str[i]; i++)
        if (str[i] >= 'a' && str[i] <= 'z')
            str[i] += ('A' - 'a');
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
            arr[i].len = len - p;
            arr[i].str = new char[len - p + 1];
            arr[i].str[len - p] = '\0';
            strncpy(arr[i].str, str + p, len - p);
            return i + 1;
        }
        arr[i].len = pos - p;
        arr[i].str = new char[pos - p + 1];
        arr[i].str[pos - p] = '\0';
        strncpy(arr[i].str, str + p, pos - p);
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
            arr[i].len = len - p;
            arr[i].str = new char[len - p + 1];
            arr[i].str[len - p] = '\0';
            strncpy(arr[i].str, str + p, len - p);
            return i + 1;
        }
        arr[i].len = pos - p;
        arr[i].str = new char[pos - p + 1];
        arr[i].str[pos - p] = '\0';
        strncpy(arr[i].str, str + p, pos - p);
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
            arr[i].len = len - p;
            arr[i].str = new char[len - p + 1];
            arr[i].str[len - p] = '\0';
            strncpy(arr[i].str, str + p, len - p);
            return i + 1;
        }
        arr[i].len = pos - p;
        arr[i].str = new char[pos - p + 1];
        arr[i].str[pos - p] = '\0';
        strncpy(arr[i].str, str + p, pos - p);
        p = pos + s.GetLength();
    }
    return Count(s) + 1;
}

WString::WString(){
    str = new wchar_t[1];
    str[0] = '\0';
    len = 0;
}

WString::WString(wchar_t c){
    str = new wchar_t[2];
    str[0] = c;
    str[1] = '\0';
    len = 1;
}

WString::WString(const wchar_t* s){
    len = wcslen(s);
    str = new wchar_t[len + 1];
    str[len] = '\0';
    wcsncpy(str, s, len);
}

WString::WString(WString &&s){
    str = new wchar_t[s.len + 1];
    len = s.len;
    wcsncpy(str, s.str, s.len);
    str[s.len] = '\0';
}

WString::WString(const WString &s){
    str = new wchar_t[s.len + 1];
    str[s.len] = '\0';
    wcsncpy(str, s.str, s.len);
    len = s.len;
}

WString &WString::operator=(const wchar_t* s){
    delete str;
    len = wcslen(s);
    str = new wchar_t[len + 1];
    str[len] = '\0';
    wcsncpy(str, s, len);
    return *this;
}

WString &WString::operator=(WString &&s){
    delete str;
    str = new wchar_t[s.len + 1];
    wcsncpy(str, s.str, s.len);
    str[s.len] = '\0';
    len = s.len;
    return *this;
}

WString &WString::operator=(const WString &s){
    delete str;
    str = new wchar_t[s.len + 1];
    wcsncpy(str, s.str, s.len);
    str[s.len] = '\0';
    len = s.len;
    return *this;
}

WString::~WString(){
    delete str;
}

wchar_t &WString::operator[](size_t index){
    if (index >= len){
        DebugError("Critical: WString::operator[](%llu) When Index Overflow", index);
    }
    return str[index];
}

WString WString::operator+(WString s) const{
    WString res;
    delete res.str;
    res.len = len + s.len;
    res.str = new wchar_t[res.len + 1];
    res.str[res.len] = '\0';
    wcsncpy(res.str, str, len);
    wcsncpy(res.str + len, s.str, s.len);
    return res;
}

WString &WString::operator+=(WString s){
    size_t newLen = len + s.len;
    wchar_t* newStr = new wchar_t[newLen + 1];
    newStr[newLen] = '\0';
    wcsncpy(newStr, str, len);
    wcsncpy(newStr + len, s.str, s.len);
    delete str;
    str = newStr;
    len = newLen;
    return *this;
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
    }
    return str[index];
}

void WString::Reverse(){
    wcsrev(str);
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

bool WString::IgnoreCaseEqual(WString s) const{
    return wcsicmp(str, s.str) == 0;
}

bool WString::IgnoreCaseNotEqual(WString s) const{
    return wcsicmp(str, s.str) != 0;
}

bool WString::IgnoreCaseLess(WString s) const{
    return wcsicmp(str, s.str) < 0;
}

bool WString::IgnoreCaseGreater(WString s) const{
    return wcsicmp(str, s.str) > 0;
}

bool WString::IgnoreCaseLessEqual(WString s) const{
    return wcsicmp(str, s.str) <= 0;
}

bool WString::IgnoreCaseGreaterEqual(WString s) const{
    return wcsicmp(str, s.str) >= 0;
}

void WString::ToLowerCase(){
    for (size_t i = 0; str[i]; i++)
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] += ('a' - 'A');
}

void WString::ToUpperCase(){
    for (size_t i = 0; str[i]; i++)
        if (str[i] >= 'a' && str[i] <= 'z')
            str[i] += ('A' - 'a');
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
            arr[i].len = len - p;
            arr[i].str = new wchar_t[len - p + 1];
            arr[i].str[len - p] = '\0';
            wcsncpy(arr[i].str, str + p, len - p);
            return i + 1;
        }
        arr[i].len = pos - p;
        arr[i].str = new wchar_t[pos - p + 1];
        arr[i].str[pos - p] = '\0';
        wcsncpy(arr[i].str, str + p, pos - p);
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
            arr[i].len = len - p;
            arr[i].str = new wchar_t[len - p + 1];
            arr[i].str[len - p] = '\0';
            wcsncpy(arr[i].str, str + p, len - p);
            return i + 1;
        }
        arr[i].len = pos - p;
        arr[i].str = new wchar_t[pos - p + 1];
        arr[i].str[pos - p] = '\0';
        wcsncpy(arr[i].str, str + p, pos - p);
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
            arr[i].len = len - p;
            arr[i].str = new wchar_t[len - p + 1];
            arr[i].str[len - p] = '\0';
            wcsncpy(arr[i].str, str + p, len - p);
            return i + 1;
        }
        arr[i].len = pos - p;
        arr[i].str = new wchar_t[pos - p + 1];
        arr[i].str[pos - p] = '\0';
        wcsncpy(arr[i].str, str + p, pos - p);
        p = pos + s.GetLength();
    }
    return Count(s) + 1;
}