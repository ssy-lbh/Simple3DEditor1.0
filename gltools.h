#ifndef __GLTOOLS__
#define __GLTOOLS__

#include <gl/gl.h>

#include "vecmath.h"

#define STB_IMAGE_IMPLEMENTATION

class GLUtils;
class GLTexture2D;

class GLUtils {
public:
    static void DrawCorner(float x, float y, float start, float end, float radius, float step);
    static void EnableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC);
    static void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);
    static void Clear2DViewport();
    static void Clear3DViewport();
    static RECT MakeRect(LONG left, LONG right, LONG bottom, LONG top);
    static bool InRect(int x, int y, RECT rect);
    static void DrawBezier(Vector2 p1, Vector2 p2, Vector2 p3, float step);
    static void DrawBezier(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float step);
    static void DrawBezier(Vector3 p1, Vector3 p2, Vector3 p3, float step);
    static void DrawBezier(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4, float step);
    static void DrawRoundRect(float x, float y, float width, float height, float radius, float step);
    static float Clamp(float x, float a, float b);
    static void DrawRect(Vector2 p1, Vector2 p2);
    static void DrawRect(float x1, float y1, float x2, float y2);
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