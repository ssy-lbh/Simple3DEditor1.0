#ifndef __UTILS_OS_FONT__
#define __UTILS_OS_FONT__

#include <define.h>

#define MAX_ASCII_CHARS 0x80
#define MAX_CACHE_CHARS 0x10000

void glFontSize(uint size);
void glDrawString(const char* text);
void glDrawCNString(const char* text);
void glDrawCNString(const wchar_t* text);
float glGetStringWidth(const char* text);
float glGetCNStringWidth(const wchar_t* text);
float glGetStringHeight(const char* text);
float glGetCNStringHeight(const wchar_t* text);
Vector2 glGetStringSize(const char* text);
Vector2 glGetCNStringSize(const wchar_t* text);

// outText 指针由new[]得到，需要delete[]回收内存，返回字符串由'\0'结尾
bool WideCharToBytes(const wchar_t* text, size_t len, char*& outText, size_t& outLen);
bool BytesToWideChar(const char* text, size_t len, wchar_t*& outText, size_t& outLen);
bool WideCharToBytesUTF8(const wchar_t* text, size_t len, char*& outText, size_t& outLen);
bool BytesToWideCharUTF8(const char* text, size_t len, wchar_t*& outText, size_t& outLen);

typedef struct FT_LibraryRec_* FT_Library;
typedef struct FT_FaceRec_* FT_Face;
typedef struct _FTGLfont FTGLfont;

#define MAX_FONT_CHARS 0x10000

// 目前已经可以基本使用
class Font final : public Object {
private:
    struct CharTexture {
        uint tex = 0;
        // 以下变量大小相对于Y间距
        float width;
        float height;
        float deltaX;
        float deltaY;
        float advX;
    };

    static FT_Library ftLib;

    FT_Face face = NULL;
    CharTexture tex[MAX_FONT_CHARS];

public:
    Font(String& path);
    Font(String& path, uint width, uint height);
    ~Font();

    static void Init();
    static void Uninit();

    void SetSize(Vector2 size);
    void SetSize(uint x, uint y);
    void SetTransform(Matrix2x3 m);
    uint LoadChar(uint c);
    // h决定字体高度与大小
    void DrawString(const char* text, float x, float y, float h, float lineX, float depth = 0.0f);
    void DrawString(const wchar_t* text, float x, float y, float h, float lineX, float depth = 0.0f);
    void DrawString(const WString& text, float x, float y, float h, float lineX, float depth = 0.0f);
};

// FTGL封装版本，内置自定义功能较少
// 稳定性不足，以后修复后能提供更多功能
class GLFont final : public Object {
private:
    FTGLfont* font = NULL;

    enum FontType {
        FONT_BITMAP,
        FONT_BUFFER,
        FONT_CUSTOM,
        FONT_EXTRUDE,
        FONT_OUTLINE,
        FONT_PIXMAP,
        FONT_POLYGON,
        FONT_TEXTURE
    };

    const FontType type;

    void CreateFTGLFont(String& path);

public:
    GLFont(String path, FontType type = FONT_PIXMAP);
    GLFont(String path, uint size, uint res = 72);
    GLFont(String path, uint size, FontType type, uint res = 72);
    ~GLFont();

    FontType GetType();
    void SetSize(uint size, uint res = 72);
    void DrawString(const char* text);
    void DrawString(const wchar_t* text);
    void DrawString(const String& text);
};

#endif