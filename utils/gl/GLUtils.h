#ifndef __UTILS_GL_GLUTILS__
#define __UTILS_GL_GLUTILS__

#include <define.h>

#include <utils/math3d/LinearAlgebra.h>

#define glCheckError(tag) GLUtils::CheckGLError(tag, __FILE__, __LINE__)

class GLUtils {
public:
    static bool CheckGLError(const char* tag, const char* file, int line);
    // 画圆弧角
    static void DrawCorner(float x, float y, float start, float end, float radius, float step = 0.05f);
    static void DrawCorner(float x, float y, float start, float end, Vector2 radius, float step = 0.05f);
    static void DrawCorner(Vector2 center, float start, float end, float radius, float step = 0.05f);
    static void DrawCorner(Vector2 center, float start, float end, Vector2 radius, float step = 0.05f);
    // 画圆弧角，同时四个方向UV值从0.0到1.0，覆盖整个纹理，可使用纹理坐标变换改变位置
    static void DrawCornerWithUV(float x, float y, float start, float end, float radius, float step = 0.05f);
    static void DrawCornerWithUV(Vector2 center, float start, float end, float radius, float step = 0.05f);
    static void DrawCornerWithUV(float x, float y, float start, float end, float radius, Rect uvBound, float step = 0.05f);
    static void DrawCornerWithUV(Vector2 center, float start, float end, float radius, Rect uvBound, float step = 0.05f);
    static void Draw3DArrow(Vector3 ori, Vector3 dir, float radius, float rate, float lineWidth = 1.0f, float step = 0.05f);
    static void Clear2DViewport();
    static void Clear3DViewport();
    static bool InRect(float x, float y, Rect rect);
    static bool InRect(Vector2 v, Rect rect);
    static void DrawBezier(Vector2 p1, Vector2 p2, Vector2 p3, float step = 0.05f);
    static void DrawBezier(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float step = 0.05f);
    static void DrawBezier(Vector3 p1, Vector3 p2, Vector3 p3, float step = 0.05f);
    static void DrawBezier(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4, float step = 0.05f);
    static void DrawRoundRect(float x, float y, float width, float height, float radius, float step = 0.05f);
    static void DrawRoundRect(float x, float y, float width, float height, Vector2 radius, float step = 0.05f);
    static void DrawRoundRect(Rect rect, float radius, float step = 0.05f);
    static void DrawRoundRectWithUV(Rect rect, float radius, float step = 0.05f);
    static void DrawRoundRectWithUV(Rect rect, float radius, Rect uvBound, float step = 0.05f);
    static void DrawRect(Rect rect);
    static void DrawRect(Vector2 p1, Vector2 p2);
    static void DrawRect(float x1, float y1, float x2, float y2);
    static void DrawRectWithUV(Rect rect);
    static void DrawRectWithUV(Rect rect, Rect uvBound);
    static void PushMatrix(Matrix4x4 mat);
    static void PopMatrix();
    static Matrix4x4 GetProjectionMatrix();
    static Matrix4x4 GetModelViewMatrix();
    static void ResetProjection();
    static void ResetModelView();
};

#endif