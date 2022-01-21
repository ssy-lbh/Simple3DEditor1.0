#ifndef __GLTOOLS__
#define __GLTOOLS__

#include <gl/gl.h>

class GLUtils;
class GLTexture2D;

class GLUtils {
public:
    static void DrawCorner(float x, float y, float start, float end, float radius, float step);
    static void EnableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC);
    static void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);
    static RECT MakeRect(LONG left, LONG right, LONG bottom, LONG top);
    static bool InRect(int x, int y, RECT rect);
};

class GLTexture2D {
private:
    GLuint tex;
public:
    GLTexture2D(const char* path);
    GLTexture2D(int resid);
    ~GLTexture2D();
    void Enable();
    static void Disable();
};

#endif