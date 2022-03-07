#include <utils/os/Font.h>

#include <windows.h>

#include <lib/opengl/gl/gl.h>
#include <lib/opengl/gl/wgl.h>

#include <utils/os/AppFrame.h>
#include <utils/math3d/LinearAlgebra.h>

#define MAX_CHARS 128

void glFontSize(uint size) {
    HFONT hFont;
    HDC hDC = wglGetCurrentDC();

    hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
        GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "微软雅黑");

    HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);
    DeleteObject(hOldFont);
}

void glInitASCIIFont(){
    AppFrame* frame = AppFrame::GetLocalInst();
    HDC hDC;

    frame->fontASCII = glGenLists(MAX_CHARS);
    hDC = wglGetCurrentDC();
    wglUseFontBitmapsA(hDC, 0, MAX_CHARS, frame->fontASCII);
}

void glDrawString(const char* text){
    AppFrame* frame = AppFrame::GetLocalInst();

    glListBase(frame->fontASCII);
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
}

void glDrawCNString(const char* text){
    size_t wlen = 0;
    wchar_t* wstr;

    wlen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text, -1, NULL, 0);

    wstr = new wchar_t[wlen + 1];
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text, -1, wstr, wlen);
    wstr[wlen] = L'\0';

    glDrawCNString(wstr);

    delete[] wstr;
}

void glDrawCNString(const wchar_t* text){
    AppFrame* frame = AppFrame::GetLocalInst();
    HDC hDC;
    GLuint font;

    hDC = wglGetCurrentDC();
    font = glGenLists(1);

    for (; *text != L'\0'; text++){
        if ((uint)*text < MAX_CHARS){
            glCallList(frame->fontASCII + (uint)*text);
            continue;
        }
        wglUseFontBitmapsW(hDC, *text, 1, font);
        glCallList(font);
    }

    glDeleteLists(font, 1);
}

float glGetStringWidth(const char* text){
    HDC hDC = wglGetCurrentDC();
    size_t len = strlen(text);
    SIZE size;
    
    GetTextExtentPoint32A(hDC, text, len, &size);

    return (float)size.cx;
}

float glGetCNStringWidth(const wchar_t* text){
    HDC hDC = wglGetCurrentDC();
    size_t len = wcslen(text);
    SIZE size;
    
    GetTextExtentPoint32W(hDC, text, len, &size);

    return (float)size.cx;
}

float glGetStringHeight(const char* text){
    HDC hDC = wglGetCurrentDC();
    size_t len = strlen(text);
    SIZE size;
    
    GetTextExtentPoint32A(hDC, text, len, &size);

    return (float)size.cy;
}

float glGetCNStringHeight(const wchar_t* text){
    HDC hDC = wglGetCurrentDC();
    size_t len = wcslen(text);
    SIZE size;
    
    GetTextExtentPoint32W(hDC, text, len, &size);

    return (float)size.cy;
}

Vector2 glGetStringSize(const char* text){
    HDC hDC = wglGetCurrentDC();
    size_t len = strlen(text);
    SIZE size;
    
    GetTextExtentPoint32A(hDC, text, len, &size);

    return Vector2(size.cx, size.cy);
}

Vector2 glGetCNStringSize(const wchar_t* text){
    HDC hDC = wglGetCurrentDC();
    size_t len = wcslen(text);
    SIZE size;
    
    GetTextExtentPoint32W(hDC, text, len, &size);

    return Vector2(size.cx, size.cy);
}