#include <utils/gl/GLUtils.h>

#include <lib/opengl/gl/gl.h>
#include <utils/os/Log.h>
#include <utils/math3d/Math.h>
#include <utils/math3d/LinearAlgebra.h>

GLRect::GLRect(){}
GLRect::GLRect(Vector2 p1, Vector2 p2) : left(p1.x), right(p2.x), top(p1.y), bottom(p2.y) { Sort(left, right); Sort(bottom, top); }
GLRect::GLRect(float left, float right, float bottom, float top) : left(left), right(right), bottom(bottom), top(top) {}
GLRect::~GLRect(){}

float GLRect::GetAspect(){
    return (right - left) / (top - bottom);
}

float GLRect::GetWidth(){
    return right - left;
}

float GLRect::GetHeight(){
    return top - bottom;
}

Vector2 GLRect::GetSize(){
    return Vector2(right - left, top - bottom);
}

float GLRect::GetXRatio(float x){
    return GetRate(x, left, right);
}

float GLRect::GetYRatio(float y){
    return GetRate(y, bottom, top);
}

Vector2 GLRect::GetRatio(float x, float y){
    return Vector2(GetRate(x, left, right), GetRate(y, bottom, top));
}

Vector2 GLRect::GetRatio(Vector2 pos){
    return GetRatio(pos.x, pos.y);
}

float GLRect::GetXRatioPos(float ratio){
    return Lerp(left, right, ratio);
}

float GLRect::GetYRatioPos(float ratio){
    return Lerp(bottom, top, ratio);
}

Vector2 GLRect::GetRatioPos(float ratioX, float ratioY){
    return Vector2(Lerp(left, right, ratioX), Lerp(bottom, top, ratioY));
}

Vector2 GLRect::GetRatioPos(Vector2 ratio){
    return GetRatioPos(ratio.x, ratio.y);
}

float GLRect::MapXPos(GLRect rect, float x){
    return GetXRatioPos(rect.GetXRatio(x));
}

float GLRect::MapYPos(GLRect rect, float y){
    return GetYRatioPos(rect.GetYRatio(y));
}

Vector2 GLRect::MapPos(GLRect rect, Vector2 pos){
    return GetRatioPos(rect.GetRatio(pos));
}

Vector2 GLRect::MapPos(GLRect rect, float x, float y){
    return GetRatioPos(rect.GetRatio(x, y));
}

GLRect GLRect::ChildRect(GLRect ratio){
    return ChildRect(ratio.left, ratio.right, ratio.bottom, ratio.top);
}

GLRect GLRect::ChildRect(float left, float right, float bottom, float top){
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
    start = ToRadian(start);
    end = ToRadian(end);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (float i = start; i < end; i += step){
        glVertex2f(x + radius * Cos(i), y + radius * Sin(i));
    }
    glVertex2f(x + radius * Cos(end), y + radius * Sin(end));
    glEnd();
}

void GLUtils::DrawCorner(Vector2 center, float start, float end, float radius, float step){
    DrawCorner(center.x, center.y, start, end, radius, step);
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

void GLUtils::DrawCornerWithUV(float x, float y, float start, float end, float radius, float step, GLRect uvBound){
    DrawCornerWithUV(Vector2(x, y), start, end, radius, step, uvBound);
}

void GLUtils::DrawCornerWithUV(Vector2 center, float start, float end, float radius, float step, GLRect uvBound){
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
        Vector2 pos = p1 * j * j + p2 * 2.0f * i * j + p3 * i * i;
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
        Vector2 pos = p1 * j * j * j + p2 * 3.0f * i * j * j + p3 * 3.0f * i * i * j + p4 * i * i * i;
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
        Vector3 pos = p1 * j * j + p2 * 2.0f * i * j + p3 * i * i;
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
        Vector3 pos = p1 * j * j * j + p2 * 3.0f * i * j * j + p3 * 3.0f * i * i * j + p4 * i * i * i;
        glVertex3f(pos.x, pos.y, pos.z);
    }
    glVertex3f(p4.x, p4.y, p4.z);
    glEnd();
}

void GLUtils::DrawRoundRect(float x, float y, float width, float height, float radius, float step){
    float xmin = x + radius, xmax = x + width - radius;
    float ymax = y + height - radius, ymin = y + radius;
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
    float width = rect.GetWidth(), height = rect.GetHeight();
    float xmin = rect.left + radius, xmax = rect.left + width - radius;
    float ymax = rect.bottom + height - radius, ymin = rect.bottom + radius;
    float xr = rect.left + width, yr = rect.bottom + height;

    float xminUV = rect.GetXRatio(xmin);
    float xmaxUV = rect.GetXRatio(xmax);
    float yminUV = 1.0f - rect.GetYRatio(ymin);
    float ymaxUV = 1.0f - rect.GetYRatio(ymax);

    GLUtils::DrawCornerWithUV(xmin, ymax, 90.0f, 180.0f, radius, step,
        GLRect(0.0f, xminUV, ymaxUV, 0.0f));
    GLUtils::DrawCornerWithUV(xmax, ymax, 0.0f, 90.0f, radius, step,
        GLRect(xmaxUV, 1.0f, ymaxUV, 0.0f));
    GLUtils::DrawCornerWithUV(xmin, ymin, 180.0f, 270.0f, radius, step,
        GLRect(0.0f, xminUV, 1.0f, yminUV));
    GLUtils::DrawCornerWithUV(xmax, ymin, 270.0f, 360.0f, radius, step,
        GLRect(xmaxUV, 1.0f, 1.0f, yminUV));

    GLUtils::DrawRectWithUV(
        GLRect(xmin, xmax, rect.bottom, rect.top),
        GLRect(xminUV, xmaxUV, 1.0f, 0.0f)
    );
    GLUtils::DrawRectWithUV(
        GLRect(rect.left, xmin, ymin, ymax),
        GLRect(0.0f, xminUV, yminUV, ymaxUV)
    );
    GLUtils::DrawRectWithUV(
        GLRect(xmax, rect.right, ymin, ymax),
        GLRect(xmaxUV, 1.0f, yminUV, ymaxUV)
    );
}

void GLUtils::DrawRoundRectWithUV(GLRect rect, float radius, float step, GLRect uvBound){
    float width = rect.GetWidth(), height = rect.GetHeight();
    float xmin = rect.left + radius, xmax = rect.left + width - radius;
    float ymax = rect.bottom + height - radius, ymin = rect.bottom + radius;
    float xr = rect.left + width, yr = rect.bottom + height;

    float xminUV = uvBound.GetXRatioPos(rect.GetXRatio(xmin));
    float xmaxUV = uvBound.GetXRatioPos(rect.GetXRatio(xmax));
    float yminUV = uvBound.GetYRatioPos(rect.GetYRatio(ymin));
    float ymaxUV = uvBound.GetYRatioPos(rect.GetYRatio(ymax));

    GLUtils::DrawCornerWithUV(xmin, ymax, 90.0f, 180.0f, radius, step,
        GLRect(uvBound.left, xminUV, ymaxUV, uvBound.top));
    GLUtils::DrawCornerWithUV(xmax, ymax, 0.0f, 90.0f, radius, step,
        GLRect(xmaxUV, uvBound.right, ymaxUV, uvBound.top));
    GLUtils::DrawCornerWithUV(xmin, ymin, 180.0f, 270.0f, radius, step,
        GLRect(uvBound.left, xminUV, uvBound.bottom, yminUV));
    GLUtils::DrawCornerWithUV(xmax, ymin, 270.0f, 360.0f, radius, step,
        GLRect(xmaxUV, uvBound.right, uvBound.bottom, yminUV));

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

float GLUtils::Clamp(float x, float a, float b){
    if (x <= a){
        return a;
    }
    if (x >= b){
        return b;
    }
    return x;
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

void glColorv3(Vector3 v){
    glColor3f(v.x, v.y, v.z);
}

void glColorv4(Vector4 v){
    glColor4f(v.x, v.y, v.z, v.w);
}

void glRotateq(Quaternion q){
    Vector3 axis = q.GetAxis().Normal();
    glRotatef(ToAngle(2.0f * Acos(q.w)), axis.x, axis.y, axis.z);
}

void glScalev3(Vector3 v){
    glScalef(v.x, v.y, v.z);
}

void glLoadMatrix(Matrix4x4 mat){
    glLoadMatrixf((GLfloat*)&mat);
}

void glMultMatrix(Matrix4x4 mat){
    glMultMatrixf((GLfloat*)&mat);
}