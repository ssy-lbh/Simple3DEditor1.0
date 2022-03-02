#ifndef __UTILS_GL_GLUTILS__
#define __UTILS_GL_GLUTILS__

#include <define.h>

#include <utils/math3d/LinearAlgebra.h>

class GLRect final : public Object {
public:
    float left;
    float right;
    float bottom;
    float top;

    GLRect();
    GLRect(Vector2 p1, Vector2 p2);
    GLRect(float left, float right, float bottom, float top);
    ~GLRect();

    float GetAspect();
    float GetWidth();
    float GetHeight();
    Vector2 GetSize();
    float GetXRatio(float x);
    float GetYRatio(float y);
    Vector2 GetRatio(float x, float y);
    Vector2 GetRatio(Vector2 pos);
    float GetXRatioPos(float ratio);
    float GetYRatioPos(float ratio);
    Vector2 GetRatioPos(float ratioX, float ratioY);
    Vector2 GetRatioPos(Vector2 ratio);
    float MapXPos(GLRect rect, float x);
    float MapYPos(GLRect rect, float y);
    // 把pos在rect中所在的位置比例映射到自身中位置
    Vector2 MapPos(GLRect rect, Vector2 pos);
    Vector2 MapPos(GLRect rect, float x, float y);
    GLRect ChildRect(GLRect ratio);
    GLRect ChildRect(float left, float right, float bottom, float top);
};

#define glCheckError(tag) GLUtils::CheckGLError(tag, __FILE__, __LINE__)

class GLUtils {
public:
    static bool CheckGLError(const char* tag, const char* file, int line);
    // 画圆弧角
    static void DrawCorner(float x, float y, float start, float end, float radius, float step);
    static void DrawCorner(Vector2 center, float start, float end, float radius, float step);
    // 画圆弧角，同时四个方向UV值从0.0到1.0，覆盖整个纹理，可使用纹理坐标变换改变位置
    static void DrawCornerWithUV(float x, float y, float start, float end, float radius, float step);
    static void DrawCornerWithUV(Vector2 center, float start, float end, float radius, float step);
    static void DrawCornerWithUV(float x, float y, float start, float end, float radius, float step, GLRect uvBound);
    static void DrawCornerWithUV(Vector2 center, float start, float end, float radius, float step, GLRect uvBound);
    static void Clear2DViewport();
    static void Clear3DViewport();
    static bool InRect(float x, float y, GLRect rect);
    static bool InRect(Vector2 v, GLRect rect);
    static void DrawBezier(Vector2 p1, Vector2 p2, Vector2 p3, float step);
    static void DrawBezier(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float step);
    static void DrawBezier(Vector3 p1, Vector3 p2, Vector3 p3, float step);
    static void DrawBezier(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4, float step);
    static void DrawRoundRect(float x, float y, float width, float height, float radius, float step);
    static void DrawRoundRect(GLRect rect, float radius, float step);
    static void DrawRoundRectWithUV(GLRect rect, float radius, float step);
    static void DrawRoundRectWithUV(GLRect rect, float radius, float step, GLRect uvBound);
    static float Clamp(float x, float a, float b);
    static void DrawRect(GLRect rect);
    static void DrawRect(Vector2 p1, Vector2 p2);
    static void DrawRect(float x1, float y1, float x2, float y2);
    static void DrawRectWithUV(GLRect rect);
    static void DrawRectWithUV(GLRect rect, GLRect uvBound);
    static void PushMatrix(Matrix4x4 mat);
    static void PopMatrix();
    static Matrix4x4 GetProjectionMatrix();
    static Matrix4x4 GetModelViewMatrix();
    static void ResetProjection();
    static void ResetModelView();
};

void glColorv3(Vector3 v);
void glColorv4(Vector4 v);
void glRotateq(Quaternion q);
void glScalev3(Vector3 v);
void glLoadMatrix(Matrix4x4 mat);
void glMultMatrix(Matrix4x4 mat);
void glGetMatrix(Matrix4x4* mat);

#endif