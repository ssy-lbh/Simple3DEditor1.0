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

// 未投入使用，存在问题
class Font final : public Object {
private:
    struct CharTexture {
        uint tex = 0;
        wchar_t ch;
        uint width;
        uint height;
        uint advX;
        uint advY;
        uint deltaX;
        uint deltaY;
    };

    static FT_Library ftLib;

    FT_Face face = NULL;
    CharTexture tex[MAX_FONT_CHARS];

public:
    Font(String path);
    Font(String path, uint width, uint height);
    ~Font();

    static void Init();

    void SetSize(Vector2 size);
    void SetSize(uint x, uint y);
    void SetTransform(Matrix2x3 m);
    uint GetIndex(uint c);
    const uchar* LoadIndex(uint idx);
    uint LoadChar(wchar_t c);
    void DrawString(const wchar_t* text, uint x, uint y, uint maxX, uint lineHeight, float depth = 0.0f);
    void DrawString(const WString& text, uint x, uint y, uint maxX, uint lineHeight, float depth = 0.0f);
};

#endif