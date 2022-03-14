#ifndef __UTILS_OS_FONT__
#define __UTILS_OS_FONT__

#include <define.h>

#define MAX_ASCII_CHARS 0x80

void glFontSize(uint size);
void glInitASCIIFont();
void glDrawString(const char* text);
void glDrawCNString(const char* text);
void glDrawCNString(const wchar_t* text);
float glGetStringWidth(const char* text);
float glGetCNStringWidth(const wchar_t* text);
float glGetStringHeight(const char* text);
float glGetCNStringHeight(const wchar_t* text);
Vector2 glGetStringSize(const char* text);
Vector2 glGetCNStringSize(const wchar_t* text);

typedef struct FT_LibraryRec_* FT_Library;
typedef struct FT_FaceRec_* FT_Face;

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
    Font(String path);
    Font(String path, uint width, uint height);
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

#endif