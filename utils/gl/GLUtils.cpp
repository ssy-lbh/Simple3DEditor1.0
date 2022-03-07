#include <utils/gl/GLUtils.h>

#include <lib/opengl/gl/gl.h>

#include <utils/os/Log.h>
#include <utils/gl/GLSimplified.h>
#include <utils/math3d/Math.h>
#include <utils/math3d/LinearAlgebra.h>

GLRect::GLRect(){}

GLRect::GLRect(Vector2 p1, Vector2 p2) : left(p1.x), right(p2.x), top(p1.y), bottom(p2.y) {
    Sort(left, right); Sort(bottom, top);
}

GLRect::GLRect(float left, float right, float bottom, float top)
    : left(left), right(right), bottom(bottom), top(top) {
    Sort(left, right); Sort(bottom, top);
}

GLRect::~GLRect(){}

const GLRect GLRect::zero = GLRect(0.0f, 0.0f, 0.0f, 0.0f);

float GLRect::GetAspect() const{
    return (right - left) / (top - bottom);
}

float GLRect::GetWidth() const{
    return right - left;
}

float GLRect::GetHeight() const{
    return top - bottom;
}

Vector2 GLRect::GetSize() const{
    return Vector2(right - left, top - bottom);
}

bool GLRect::Inside(Vector2 pos) const{
    return pos.x >= left && pos.x <= right &&
            pos.y >= bottom && pos.y <= top;
}

float GLRect::GetXRatio(float x) const{
    return GetRate(x, left, right);
}

float GLRect::GetYRatio(float y) const{
    return GetRate(y, bottom, top);
}

Vector2 GLRect::GetRatio(float x, float y) const{
    return Vector2(GetRate(x, left, right), GetRate(y, bottom, top));
}

Vector2 GLRect::GetRatio(Vector2 pos) const{
    return GetRatio(pos.x, pos.y);
}

float GLRect::GetXRatioPos(float ratio) const{
    return Lerp(left, right, ratio);
}

float GLRect::GetYRatioPos(float ratio) const{
    return Lerp(bottom, top, ratio);
}

Vector2 GLRect::GetRatioPos(float ratioX, float ratioY) const{
    return Vector2(Lerp(left, right, ratioX), Lerp(bottom, top, ratioY));
}

Vector2 GLRect::GetRatioPos(Vector2 ratio) const{
    return GetRatioPos(ratio.x, ratio.y);
}

float GLRect::MapXPos(GLRect rect, float x) const{
    return GetXRatioPos(rect.GetXRatio(x));
}

float GLRect::MapYPos(GLRect rect, float y) const{
    return GetYRatioPos(rect.GetYRatio(y));
}

Vector2 GLRect::MapPos(GLRect rect, Vector2 pos) const{
    return GetRatioPos(rect.GetRatio(pos));
}

Vector2 GLRect::MapPos(GLRect rect, float x, float y) const{
    return GetRatioPos(rect.GetRatio(x, y));
}

GLRect GLRect::ChildRect(GLRect ratio) const{
    return ChildRect(ratio.left, ratio.right, ratio.bottom, ratio.top);
}

GLRect GLRect::ChildRect(float left, float right, float bottom, float top) const{
    GLRect rect;
    Vector2 size;

    size = GetSize();

    rect.left = this->left + Round(size.x * Clamp((left + 1.0f) * 0.5f, 0.0f, 1.0f));
    rect.right = this->left + Round(size.x * Clamp((right + 1.0f) * 0.5f, 0.0f, 1.0f));
    rect.bottom = this->bottom + Round(size.y * Clamp((bottom + 1.0f) * 0.5f, 0.0f, 1.0f));
    rect.top = this->bottom + Round(size.y * Clamp((top + 1.0f) * 0.5f, 0.0f, 1.0f));

    return rect;
}

bool GLUtils::CheckGLError(const char* tag, const char* file, int line){
    int loopCnt = 0;
	for (GLenum error = glGetError(); loopCnt < 32 && error != GL_NO_ERROR; error = glGetError(), ++loopCnt){
		const char* pMsg;
		switch (error){
		case GL_INVALID_ENUM: pMsg = "Invalid enum"; break;
		case GL_INVALID_VALUE: pMsg = "Invalid value"; break;
		case GL_INVALID_OPERATION: pMsg = "Invalid operation"; break;
        case GL_STACK_OVERFLOW: pMsg = "Stack overflow"; break;
        case GL_STACK_UNDERFLOW: pMsg = "Stack underflow"; break;
		case GL_OUT_OF_MEMORY: pMsg = "Out of memory"; break;
		default: pMsg = "Unknown error";
		}
		DebugError("[OpenGL Error] %s %s(0x%x) at %s:%d\n", tag, pMsg, error, file, line);
	}
	return loopCnt != 0;
}

void GLUtils::DrawCorner(float x, float y, float start, float end, float radius, float step){
    DrawCorner(Vector2(x, y), start, end, Vector2(radius, radius), step);
}

void GLUtils::DrawCorner(float x, float y, float start, float end, Vector2 radius, float step){
    DrawCorner(Vector2(x, y), start, end, radius, step);
}

void GLUtils::DrawCorner(Vector2 center, float start, float end, float radius, float step){
    DrawCorner(center, start, end, Vector2(radius, radius), step);
}

void GLUtils::DrawCorner(Vector2 center, float start, float end, Vector2 radius, float step){
    start = ToRadian(start);
    end = ToRadian(end);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(center.x, center.y);
    for (float i = start; i < end; i += step){
        glVertex2f(center.x + radius.x * Cos(i), center.y + radius.y * Sin(i));
    }
    glVertex2f(center.x + radius.x * Cos(end), center.y + radius.y * Sin(end));
    glEnd();
}

void GLUtils::DrawCornerWithUV(float x, float y, float start, float end, float radius, float step){
    float vsin, vcos;

    start = ToRadian(start);
    end = ToRadian(end);

    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5f, 0.5f);
    glVertex2f(x, y);
    for (float i = start; i < end; i += step){
        vsin = Sin(i), vcos = Cos(i);
        glTexCoord2f((vcos + 1.0f) * 0.5f, (-vsin + 1.0f) * 0.5f);
        glVertex2f(x + radius * vcos, y + radius * vsin);
    }
    vsin = Sin(end), vcos = Cos(end);
    glTexCoord2f((vcos + 1.0f) * 0.5f, (-vsin + 1.0f) * 0.5f);
    glVertex2f(x + radius * vcos, y + radius * vsin);
    glEnd();
}

void GLUtils::DrawCornerWithUV(Vector2 center, float start, float end, float radius, float step){
    DrawCornerWithUV(center.x, center.y, start, end, radius, step);
}

void GLUtils::DrawCornerWithUV(float x, float y, float start, float end, float radius, GLRect uvBound, float step){
    DrawCornerWithUV(Vector2(x, y), start, end, radius, uvBound, step);
}

void GLUtils::DrawCornerWithUV(Vector2 center, float start, float end, float radius, GLRect uvBound, float step){
    float vsin, vcos;

    start = ToRadian(start);
    end = ToRadian(end);

    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5f, 0.5f);
    glVertex2f(center.x, center.y);
    for (float i = start; i < end; i += step){
        vsin = Sin(i), vcos = Cos(i);
        glTexCoord2f(uvBound.GetXRatioPos((vcos + 1.0f) * 0.5f), uvBound.GetYRatioPos((vsin + 1.0f) * 0.5f));
        glVertex2f(center.x + radius * vcos, center.y + radius * vsin);
    }
    vsin = Sin(end), vcos = Cos(end);
    glTexCoord2f(uvBound.GetXRatioPos((vcos + 1.0f) * 0.5f), uvBound.GetYRatioPos((vsin + 1.0f) * 0.5f));
    glVertex2f(center.x + radius * vcos, center.y + radius * vsin);
    glEnd();
}

void GLUtils::Draw3DArrow(Vector3 ori, Vector3 dir, float radius, float rate, float lineWidth, float step){
    Quaternion rot = Quaternion::FromTo(Vector3::forward, dir);
    Vector3 forward;
    Vector3 right;
    Vector3 up;
    Vector3 dst;
    Vector3 center;

    rot = Quaternion::FromTo(Vector3::forward, dir);

    forward = dir.Normal();
    right = (rot * Vector3::right) * radius;
    up = (rot * Vector3::up) * radius;

    dst = ori + dir;
    center = ori + dir * (1.0f - rate);

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(lineWidth);
    glBegin(GL_LINES);
    glVertexv3(ori); glVertexv3(center);
    glEnd();
    glLineWidth(1.0f);
    glDisable(GL_LINE_SMOOTH);

    glBegin(GL_TRIANGLE_FAN);
    glVertexv3(dst);
    glVertexv3(center + right);
    for (float i = 0.0f; i < 2.0f * PI; i += step)
        glVertexv3(center + right * Cos(i) + up * Sin(i));
    glVertexv3(center + right);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertexv3(center);
    glVertexv3(center + right);
    for (float i = 0.0f; i < 2.0f * PI; i += step)
        glVertexv3(center + right * Cos(i) + up * Sin(i));
    glVertexv3(center + right);
    glEnd();
}

void GLUtils::Clear2DViewport(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLUtils::Clear3DViewport(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool GLUtils::InRect(float x, float y, GLRect rect){
    return x >= rect.left && x <= rect.right && y >= rect.bottom && y <= rect.top;
}

bool GLUtils::InRect(Vector2 v, GLRect rect){
    return v.x >= rect.left && v.x <= rect.right && v.y >= rect.bottom && v.y <= rect.top;
}

void GLUtils::DrawBezier(Vector2 p1, Vector2 p2, Vector2 p3, float step){
    glBegin(GL_LINE_STRIP);
    glVertex2f(p1.x, p1.y);
    for (float i = step; i < 1.0f; i += step){
        float j = 1.0f - i;
        Vector2 pos = p1 * (j * j) + p2 * (2.0f * i * j) + p3 * (i * i);
        glVertex2f(pos.x, pos.y);
    }
    glVertex2f(p3.x, p3.y);
    glEnd();
}

void GLUtils::DrawBezier(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float step){
    glBegin(GL_LINE_STRIP);
    glVertex2f(p1.x, p1.y);
    for (float i = step; i < 1.0f; i += step){
        float j = 1.0f - i;
        Vector2 pos = p1 * (j * j * j) + p2 * (3.0f * i * j * j) + p3 * (3.0f * i * i * j) + p4 * (i * i * i);
        glVertex2f(pos.x, pos.y);
    }
    glVertex2f(p4.x, p4.y);
    glEnd();
}

void GLUtils::DrawBezier(Vector3 p1, Vector3 p2, Vector3 p3, float step){
    glBegin(GL_LINE_STRIP);
    glVertex3f(p1.x, p1.y, p1.z);
    for (float i = step; i < 1.0f; i += step){
        float j = 1.0f - i;
        Vector3 pos = p1 * (j * j) + p2 * (2.0f * i * j) + p3 * (i * i);
        glVertex3f(pos.x, pos.y, pos.z);
    }
    glVertex3f(p3.x, p3.y, p3.z);
    glEnd();
}

void GLUtils::DrawBezier(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4, float step){
    glBegin(GL_LINE_STRIP);
    glVertex3f(p1.x, p1.y, p1.z);
    for (float i = step; i < 1.0f; i += step){
        float j = 1.0f - i;
        Vector3 pos = p1 * (j * j * j) + p2 * (3.0f * i * j * j) + p3 * (3.0f * i * i * j) + p4 * (i * i * i);
        glVertex3f(pos.x, pos.y, pos.z);
    }
    glVertex3f(p4.x, p4.y, p4.z);
    glEnd();
}

void GLUtils::DrawRoundRect(float x, float y, float width, float height, float radius, float step){
    DrawRoundRect(x, y, width, height, Vector2(radius, radius), step);
}

void GLUtils::DrawRoundRect(float x, float y, float width, float height, Vector2 radius, float step){
    float xmin = x + radius.x, xmax = x + width - radius.x;
    float ymax = y + height - radius.y, ymin = y + radius.y;
    float xr = x + width, yr = y + height;

    GLUtils::DrawCorner(xmin, ymax, 90.0f, 180.0f, radius, step);
    GLUtils::DrawCorner(xmax, ymax, 0.0f, 90.0f, radius, step);
    GLUtils::DrawCorner(xmin, ymin, 180.0f, 270.0f, radius, step);
    GLUtils::DrawCorner(xmax, ymin, 270.0f, 360.0f, radius, step);

    glBegin(GL_TRIANGLES);
    glVertex2f(xmin, yr); glVertex2f(xmin, y); glVertex2f(xmax, y);
    glVertex2f(xmin, yr); glVertex2f(xmax, yr); glVertex2f(xmax, y);
    glVertex2f(x, ymax); glVertex2f(xmin, ymax); glVertex2f(xmin, ymin);
    glVertex2f(x, ymax); glVertex2f(x, ymin); glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymax); glVertex2f(xr, ymax); glVertex2f(xr, ymin);
    glVertex2f(xmax, ymax); glVertex2f(xmax, ymin); glVertex2f(xr, ymin);
    glEnd();
}

void GLUtils::DrawRoundRect(GLRect rect, float radius, float step){
    DrawRoundRect(rect.left, rect.bottom, rect.GetWidth(), rect.GetHeight(), radius, step);
}

void GLUtils::DrawRoundRectWithUV(GLRect rect, float radius, float step){
    GLRect uvBound;
    uvBound.left = 0.0f; uvBound.right = 1.0f;
    uvBound.bottom = 1.0f; uvBound.top = 0.0f;
    DrawRoundRectWithUV(rect, radius, uvBound, step);
}

void GLUtils::DrawRoundRectWithUV(GLRect rect, float radius, GLRect uvBound, float step){
    float width = rect.GetWidth(), height = rect.GetHeight();
    float xmin = rect.left + radius, xmax = rect.left + width - radius;
    float ymax = rect.bottom + height - radius, ymin = rect.bottom + radius;
    float xr = rect.left + width, yr = rect.bottom + height;

    float xminUV = uvBound.GetXRatioPos(rect.GetXRatio(xmin));
    float xmaxUV = uvBound.GetXRatioPos(rect.GetXRatio(xmax));
    float yminUV = uvBound.GetYRatioPos(rect.GetYRatio(ymin));
    float ymaxUV = uvBound.GetYRatioPos(rect.GetYRatio(ymax));

    GLUtils::DrawCornerWithUV(xmin, ymax, 90.0f, 180.0f, radius,
        GLRect(uvBound.left, xminUV, ymaxUV, uvBound.top), step);
    GLUtils::DrawCornerWithUV(xmax, ymax, 0.0f, 90.0f, radius,
        GLRect(xmaxUV, uvBound.right, ymaxUV, uvBound.top), step);
    GLUtils::DrawCornerWithUV(xmin, ymin, 180.0f, 270.0f, radius,
        GLRect(uvBound.left, xminUV, uvBound.bottom, yminUV), step);
    GLUtils::DrawCornerWithUV(xmax, ymin, 270.0f, 360.0f, radius,
        GLRect(xmaxUV, uvBound.right, uvBound.bottom, yminUV), step);

    GLUtils::DrawRectWithUV(
        GLRect(xmin, xmax, rect.bottom, rect.top),
        GLRect(xminUV, xmaxUV, uvBound.bottom, uvBound.top)
    );
    GLUtils::DrawRectWithUV(
        GLRect(rect.left, xmin, ymin, ymax),
        GLRect(uvBound.left, xminUV, yminUV, ymaxUV)
    );
    GLUtils::DrawRectWithUV(
        GLRect(xmax, rect.right, ymin, ymax),
        GLRect(xmaxUV, uvBound.right, yminUV, ymaxUV)
    );
}

void GLUtils::DrawRect(GLRect rect){
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(rect.left, rect.bottom);
    glVertex2f(rect.left, rect.top);
    glVertex2f(rect.right, rect.top);
    glVertex2f(rect.right, rect.bottom);
    glEnd();
}

void GLUtils::DrawRect(Vector2 p1, Vector2 p2){
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p1.x, p2.y);
    glVertex2f(p2.x, p2.y);
    glVertex2f(p2.x, p1.y);
    glEnd();
}

void GLUtils::DrawRect(float x1, float y1, float x2, float y2){
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1, y1);
    glVertex2f(x1, y2);
    glVertex2f(x2, y2);
    glVertex2f(x2, y1);
    glEnd();
}

void GLUtils::DrawRectWithUV(GLRect rect){
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(rect.left, rect.bottom);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(rect.left, rect.top);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(rect.right, rect.top);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(rect.right, rect.bottom);
    glEnd();
}

void GLUtils::DrawRectWithUV(GLRect rect, GLRect uvBound){
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(uvBound.left, uvBound.bottom); glVertex2f(rect.left, rect.bottom);
    glTexCoord2f(uvBound.left, uvBound.top); glVertex2f(rect.left, rect.top);
    glTexCoord2f(uvBound.right, uvBound.top); glVertex2f(rect.right, rect.top);
    glTexCoord2f(uvBound.right, uvBound.bottom); glVertex2f(rect.right, rect.bottom);
    glEnd();
}

void GLUtils::PushMatrix(Matrix4x4 mat){
    glPushMatrix();
    glMultMatrixf((float*)&mat);
}

void GLUtils::PopMatrix(){
    glPopMatrix();
}

Matrix4x4 GLUtils::GetProjectionMatrix(){
    Matrix4x4 mat;
    glGetFloatv(GL_PROJECTION_MATRIX, (float*)&mat);
    return mat;
}

Matrix4x4 GLUtils::GetModelViewMatrix(){
    Matrix4x4 mat;
    glGetFloatv(GL_MODELVIEW_MATRIX, (float*)&mat);
    return mat;
}

void GLUtils::ResetProjection(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 此种变换下，Z值小的图形深度大，渲染在后
    glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 2.0);
}

void GLUtils::ResetModelView(){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}