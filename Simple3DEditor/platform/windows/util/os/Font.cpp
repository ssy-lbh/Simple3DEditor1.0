#include <util/os/Font.h>

#include <lib/glew/gl/glew.h>
#include <lib/glew/gl/wglew.h>

#include <util/String.h>
#include <util/os/Log.h>
#include <util/os/AppFrame.h>
#include <util/os/Thread.h>
#include <util/math3d/Math.h>
#include <util/math3d/LinearAlgebra.h>

#include <windows.h>

namespace simple3deditor {

void glFontSize(uint size) {
    HFONT hFont;
    AppFrame* frame = AppFrame::GetLocalInst();
    HDC hDC = ::wglGetCurrentDC();

    hFont = ::CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
        GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "微软雅黑");

    HFONT hOldFont = (HFONT)::SelectObject(hDC, hFont);
    ::DeleteObject(hOldFont);

    if (frame->fontASCII)
        ::glDeleteLists(frame->fontASCII, MAX_ASCII_CHARS);
    frame->fontASCII = ::glGenLists(MAX_ASCII_CHARS);
    ::wglUseFontBitmapsA(hDC, 0, MAX_ASCII_CHARS, frame->fontASCII);
}

void glDrawString(const char* text){
    AppFrame* frame = AppFrame::GetLocalInst();

    ::glListBase(frame->fontASCII);
    ::glCallLists(::strlen(text), GL_UNSIGNED_BYTE, text);
}

void glDrawCNString(const char* text){
    size_t wlen = 0;
    wchar_t* wstr;

    wlen = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text, -1, NULL, 0);

    wstr = new wchar_t[wlen + 1];
    ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text, -1, wstr, wlen);
    wstr[wlen] = L'\0';

    glDrawCNString(wstr);

    delete[] wstr;
}

void glDrawCNString(const wchar_t* text){
    AppFrame* frame = AppFrame::GetLocalInst();
    HDC hDC;
    GLuint font;

    if (!frame->fontCache){
        frame->fontCache = new uint[MAX_CACHE_CHARS];
        memset(frame->fontCache, 0, MAX_CACHE_CHARS * sizeof(uint));
    }

    hDC = ::wglGetCurrentDC();

    for (; *text != L'\0'; text++){
        if ((uint)*text < MAX_ASCII_CHARS){
            ::glCallList(frame->fontASCII + (uint)*text);
            continue;
        }
        if ((uint)*text < MAX_CACHE_CHARS){
            uint& list = frame->fontCache[(uint)*text];
            if (!list){
                list = ::glGenLists(1);
                ::wglUseFontBitmapsW(hDC, *text, 1, list);
            }
            ::glCallList(list);
            continue;
        }
        font = ::glGenLists(1);
        ::wglUseFontBitmapsW(hDC, *text, 1, font);
        ::glCallList(font);
    }

    ::glDeleteLists(font, 1);
}

float glGetStringWidth(const char* text){
    HDC hDC = ::wglGetCurrentDC();
    size_t len = ::strlen(text);
    SIZE size;
    
    ::GetTextExtentPoint32A(hDC, text, len, &size);

    return (float)size.cx;
}

float glGetCNStringWidth(const wchar_t* text){
    HDC hDC = ::wglGetCurrentDC();
    size_t len = ::wcslen(text);
    SIZE size;
    
    ::GetTextExtentPoint32W(hDC, text, len, &size);

    return (float)size.cx;
}

float glGetStringHeight(const char* text){
    HDC hDC = ::wglGetCurrentDC();
    size_t len = ::strlen(text);
    SIZE size;
    
    ::GetTextExtentPoint32A(hDC, text, len, &size);

    return (float)size.cy;
}

float glGetCNStringHeight(const wchar_t* text){
    HDC hDC = ::wglGetCurrentDC();
    size_t len = ::wcslen(text);
    SIZE size;
    
    ::GetTextExtentPoint32W(hDC, text, len, &size);

    return (float)size.cy;
}

Vector2 glGetStringSize(const char* text){
    HDC hDC = ::wglGetCurrentDC();
    size_t len = ::strlen(text);
    SIZE size;
    
    ::GetTextExtentPoint32A(hDC, text, len, &size);

    return Vector2(size.cx, size.cy);
}

Vector2 glGetCNStringSize(const wchar_t* text){
    HDC hDC = ::wglGetCurrentDC();
    size_t len = ::wcslen(text);
    SIZE size;
    
    ::GetTextExtentPoint32W(hDC, text, len, &size);

    return Vector2(size.cx, size.cy);
}

bool WideCharToBytes(const wchar_t* text, size_t len, char*& outText, size_t& outLen){
    outLen = ::WideCharToMultiByte(CP_ACP, 0, text, len, NULL, 0, NULL, NULL);
    outText = new char[outLen + 1];
    ::WideCharToMultiByte(CP_ACP, 0, text, len, outText, outLen, NULL, NULL);
    outText[outLen] = '\0';
    return true;
}

bool BytesToWideChar(const char* text, size_t len, wchar_t*& outText, size_t& outLen){
    outLen = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text, len, NULL, 0);
    outText = new wchar_t[outLen + 1];
    ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text, len, outText, outLen);
    outText[outLen] = L'\0';
    return true;
}

bool WideCharToBytesUTF8(const wchar_t* text, size_t len, char*& outText, size_t& outLen){
    outLen = ::WideCharToMultiByte(CP_UTF8, 0, text, len, NULL, 0, NULL, NULL);
    outText = new char[outLen + 1];
    ::WideCharToMultiByte(CP_UTF8, 0, text, len, outText, outLen, NULL, NULL);
    outText[outLen] = '\0';
    return true;
}

bool BytesToWideCharUTF8(const char* text, size_t len, wchar_t*& outText, size_t& outLen){
    outLen = ::MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, text, len, NULL, 0);
    outText = new wchar_t[outLen + 1];
    ::MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, text, len, outText, outLen);
    outText[outLen] = L'\0';
    return true;
}

}