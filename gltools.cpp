#include "gltools.h"

#include "vecmath.h"
#include "glfunc.h"
#include "res.h"
#include "main.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

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

GLRect GLRect::ChildRect(float left, float right, float bottom, float top){
    GLRect rect;
    Vector2 size;

    size = GetSize();

    rect.left = this->left + Round(size.x * Clamp(left, 0.0f, 1.0f));
    rect.right = this->left + Round(size.x * Clamp(right, 0.0f, 1.0f));
    rect.bottom = this->bottom + Round(size.y * Clamp(bottom, 0.0f, 1.0f));
    rect.top = this->bottom + Round(size.y * Clamp(top, 0.0f, 1.0f));

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

void GLUtils::EnableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC){
    PIXELFORMATDESCRIPTOR pfd;

    *hDC = GetDC(hWnd);

    RtlZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int iFormat = ChoosePixelFormat(*hDC, &pfd);
    SetPixelFormat(*hDC, iFormat, &pfd);

    *hRC = wglCreateContext(*hDC);
    wglMakeCurrent(*hDC, *hRC);
}

void GLUtils::EnableOpenGLEXT(){
    glInitEXTFunctions();
}

void GLUtils::DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC){
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
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

RECT GLUtils::MakeRect(LONG left, LONG right, LONG bottom, LONG top){
    RECT ret;
    ret.left = left; ret.right = right;
    ret.bottom = bottom; ret.top = top;
    return ret;
}

bool GLUtils::InRect(int x, int y, RECT rect){
    return x >= rect.left && x <= rect.right && y >= rect.bottom && y <= rect.top;
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

GLTexture2D::GLTexture2D(const char* path){
    int x, y, channel;
    stbi_uc* image;

    image = stbi_load(path, &x, &y, &channel, 4);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //支持4字节对齐

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);      //S方向上贴图
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);      //T方向上贴图
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);       //放大纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);       //缩小纹理过滤方式
	
    glTexImage2D(GL_TEXTURE_2D, 0, channel, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glBindTexture(GL_TEXTURE_2D, 0);
    
    stbi_image_free(image);
}

GLTexture2D::GLTexture2D(const wchar_t* path){
    HANDLE hFile;
    BY_HANDLE_FILE_INFORMATION Info;
    stbi_uc* data;
    size_t dataSize;

    int x, y, channel;
    stbi_uc* image;

    hFile = CreateFileW(
        path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hFile == INVALID_HANDLE_VALUE){
        tex = 0;
        return;
    }

    GetFileInformationByHandle(hFile, &Info);
    dataSize = ((size_t)Info.nFileSizeHigh) | Info.nFileSizeLow;
    data = new stbi_uc[dataSize];
    ReadFile(hFile, data, dataSize, NULL, NULL);
    image = stbi_load_from_memory(data, dataSize, &x, &y, &channel, 4);
    delete[] data;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //支持4字节对齐

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);      //S方向上贴图
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);      //T方向上贴图
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);       //放大纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);       //缩小纹理过滤方式
	
    glTexImage2D(GL_TEXTURE_2D, 0, channel, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glBindTexture(GL_TEXTURE_2D, 0);
    
    stbi_image_free(image);
}

GLTexture2D::GLTexture2D(int resid){
    HBITMAP hBitmap;
    BITMAP bitmap;
    unsigned char* pBits;
    DWORD dwSize;

    hBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(resid));
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);
    dwSize = bitmap.bmHeight * bitmap.bmWidthBytes;
    pBits = new unsigned char[dwSize];
    GetBitmapBits(hBitmap, dwSize, pBits);

    int channel = bitmap.bmBitsPixel >> 3;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);// 纹理数据上传的对齐像素数
    //glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmap.bmWidthBytes / channel);// 指定像素数据中原图的宽度

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);// S方向上贴图
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);// T方向上贴图
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// 放大纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// 缩小纹理过滤方式
	
    glTexImage2D(GL_TEXTURE_2D, 0, channel, bitmap.bmWidth, bitmap.bmHeight, 0,
                (channel == 4 ? GL_BGRA_EXT : (channel == 3 ? GL_BGR_EXT : GL_R))
                , GL_UNSIGNED_BYTE, pBits);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    delete[] pBits;
}

GLTexture2D::GLTexture2D(GLuint texture, bool delTex) : tex(texture), delTex(delTex) {}

GLTexture2D::~GLTexture2D(){
    if (glIsTexture(delTex))
        glDeleteTextures(1, &tex);
}

bool GLTexture2D::Enable(){
    glEnable(GL_TEXTURE_2D);
    if (glIsTexture(tex)){
        glBindTexture(GL_TEXTURE_2D, tex);
        return true;
    }
    DebugError("GLTexture2D::Enable %d Is Not A Texture", tex);
    glBindTexture(GL_TEXTURE_2D, 0);
    return false;
}

void GLTexture2D::Disable(){
    glDisable(GL_TEXTURE_2D);
}

GLComputeProgram::GLComputeProgram(int resid){
    HRSRC kernelSrc;
    HGLOBAL resIdx;
    LPVOID resPtr;
    DWORD resSize;
    char* srcData;

    kernelSrc = FindResource(Main::hInst, MAKEINTRESOURCE(resid), MAKEINTRESOURCE(SHADER));
    resIdx = LoadResource(Main::hInst, kernelSrc);
    resPtr = LockResource(resIdx);
    resSize = SizeofResource(Main::hInst, kernelSrc);
    srcData = new char[resSize];
    RtlCopyMemory(srcData, resPtr, resSize);
    FreeResource(resIdx);

    prog = glCreateProgram();
    shader = glCreateShader(GL_COMPUTE_SHADER);

    glShaderSource(shader, 1, &srcData, NULL);
    glCompileShader(shader);

    glAttachShader(prog, shader);
    glLinkProgram(prog);
}

GLComputeProgram::GLComputeProgram(const char* source){
    prog = glCreateProgram();
    shader = glCreateShader(GL_COMPUTE_SHADER);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glAttachShader(prog, shader);
    glLinkProgram(prog);
}

GLComputeProgram::~GLComputeProgram(){
    glDeleteProgram(prog);
    glDeleteShader(shader);
}

GLuint GLComputeProgram::GetProgram(){
    return prog;
}

bool GLComputeProgram::CheckProgramError(){
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &progLog);
    return progLog > 0;// OpenGL 程序的日志长度为0时正常
}

bool GLComputeProgram::CheckShaderError(){
    glGetShaderiv(prog, GL_INFO_LOG_LENGTH, &shaderLog);
    return shaderLog > 0;// OpenGL 着色器的日志长度为-1时正常，不过为什么跟程序不一样？保险起见这样写
}

void GLComputeProgram::PrintProgramLog(){
    char* log;
    GLint logLen;

    if (progLog == -1)
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &progLog);
    if (progLog > 0){
        log = new char[progLog];
        glGetProgramInfoLog(prog, progLog, &progLog, log);
        DebugError("%s", log);
        delete[] log;
    }
}

void GLComputeProgram::PrintShaderLog(){
    char* log;
    GLint logLen;

    if (shaderLog == -1)
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &shaderLog);
    if (shaderLog > 0){
        log = new char[shaderLog];
        glGetShaderInfoLog(shader, shaderLog, &shaderLog, log);
        DebugError("%s", log);
        delete[] log;
    }
}

void GLComputeProgram::Dispatch(int x, int y, int z){
    glUseProgram(prog);
    glDispatchCompute(x, y, z);
}

GLuint GLComputeProgram::GetLoc(const char* name){
    return glGetUniformLocation(prog, name);
}

void GLComputeProgram::BindTexture(GLuint unit, GLuint texture, GLenum access, GLenum format){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 省略了glActiveTexture(GL_TEXTURE0);
    glBindImageTexture(unit, texture, 0, GL_FALSE, 0, access, format);
    glDisable(GL_TEXTURE_2D);
}

GLFrameBuffer::GLFrameBuffer(){
    glGenFramebuffers(1, &frame);
}

GLFrameBuffer::GLFrameBuffer(GLuint texture){
    glGenFramebuffers(1, &frame);
    BindTexture(texture);
}

GLFrameBuffer::~GLFrameBuffer(){
    glDeleteFramebuffers(1, &frame);
}

void GLFrameBuffer::BindTexture(GLuint texture){
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WIDTH, &x);
    glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_HEIGHT, &y);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLFrameBuffer::BindTexture(GLuint texture, int x, int y){
    this->x = x; this->y = y;
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void GLFrameBuffer::BindTexture(GLuint texture, GLenum attachment){
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);
    glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WIDTH, &x);
    glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_HEIGHT, &y);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLFrameBuffer::BindTexture(GLuint texture, GLenum attachment, int x, int y){
    this->x = x; this->y = y;
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void GLFrameBuffer::Enable(){
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame);
    ViewportManager::inst->PushViewport({0, y, x, 0});
}

void GLFrameBuffer::Disable(){
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    ViewportManager::inst->PopViewport();
}

bool GLLights::use[8] = {false, false, false, false,
                         false, false, false, false};
GLenum GLLights::idx[8] = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3,
                           GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};
                        
GLenum GLLights::CreateLight(){
    for (int i = 0; i < 8; i++){
        if (!use[i]){
            use[i] = true;
            glEnable(idx[i]);
            return idx[i];
        }
    }
    return GL_NONE;
}

bool GLLights::DestroyLight(GLenum light){
    for (int i = 0; i < 8; i++){
        if (light == idx[i]){
            if (!use[i]){
                DebugError("Light %d Has Already Been Destroyed", i);
                return false;
            }
            use[i] = false;
            glDisable(idx[i]);
            return true;
        }
    }
    return false;
}