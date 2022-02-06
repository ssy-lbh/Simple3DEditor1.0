#ifndef __GLTOOLS__
#define __GLTOOLS__

#include <gl/gl.h>

#include "vecmath.h"

class GLUtils;
class GLTexture2D;

class GLUtils {
public:
    // 画圆弧角
    static void DrawCorner(float x, float y, float start, float end, float radius, float step);
    // 画圆弧角，同时四个方向UV值从0.0到1.0，覆盖整个纹理，可使用纹理坐标变换改变位置
    static void DrawCornerWithUV(float x, float y, float start, float end, float radius, float step);
    static void EnableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC);
    static void EnableOpenGLEXT();
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
    bool delTex = true;
    
public:
    GLTexture2D(const char* path);
    GLTexture2D(const wchar_t* path);
    GLTexture2D(int resid);
    GLTexture2D(GLuint texture, bool delTex = false);
    ~GLTexture2D();
    void Enable();
    static void Disable();
};

class GLComputeProgram {
private:
    GLuint prog;
    GLuint shader;

    GLint progLog = -1;
    GLint shaderLog = -1;

public:
    GLComputeProgram(int resid);
    GLComputeProgram(const char* source);
    ~GLComputeProgram();

    GLuint GetProgram();
    bool CheckProgramError();
    bool CheckShaderError();
    void PrintProgramLog();
    void PrintShaderLog();
    void Dispatch(int x, int y, int z);
    GLuint GetLoc(const char* name);
    void BindTexture(GLuint unit, GLuint texture, GLenum access, GLenum format);
};

class GLFrameBuffer {
private:
    GLuint frame = 0;
    int x = 0, y = 0;

public:
    GLFrameBuffer();
    GLFrameBuffer(GLuint texture);
    ~GLFrameBuffer();

    void BindTexture(GLuint texture);
    void BindTexture(GLuint texture, int x, int y);
    void Enable();
    void Disable();
};

#endif