#include "gltools.h"

#include "vecmath.h"
#include "glfunc.h"
#include "res.h"
#include "main.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

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
    return x >= rect.left && x < rect.right && y >= rect.bottom && y < rect.top;
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

    GLUtils::DrawCorner(xmin, ymax, 90.0f, 180.0f, radius, 0.05f);
    GLUtils::DrawCorner(xmax, ymax, 0.0f, 90.0f, radius, 0.05f);
    GLUtils::DrawCorner(xmin, ymin, 180.0f, 270.0f, radius, 0.05f);
    GLUtils::DrawCorner(xmax, ymin, 270.0f, 360.0f, radius, 0.05f);

    glBegin(GL_TRIANGLES);
    glVertex2f(xmin, yr); glVertex2f(xmin, y); glVertex2f(xmax, y);
    glVertex2f(xmin, yr); glVertex2f(xmax, yr); glVertex2f(xmax, y);
    glVertex2f(x, ymax); glVertex2f(xmin, ymax); glVertex2f(xmin, ymin);
    glVertex2f(x, ymax); glVertex2f(x, ymin); glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymax); glVertex2f(xr, ymax); glVertex2f(xr, ymin);
    glVertex2f(xmax, ymax); glVertex2f(xmax, ymin); glVertex2f(xr, ymin);
    glEnd();
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
	
    glTexImage2D(GL_TEXTURE_2D, 0, channel, x, y, 0,
                (channel == 4 ? GL_RGBA : (channel == 3 ? GL_RGB : GL_R))
                , GL_UNSIGNED_BYTE, image);
    
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
	
    glTexImage2D(GL_TEXTURE_2D, 0, channel, x, y, 0,
                (channel == 4 ? GL_RGBA : (channel == 3 ? GL_RGB : GL_R))
                , GL_UNSIGNED_BYTE, image);
    
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

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //支持4字节对齐

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);      //S方向上贴图
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);      //T方向上贴图
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);       //放大纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);       //缩小纹理过滤方式
	
    glTexImage2D(GL_TEXTURE_2D, 0, channel, bitmap.bmWidth, bitmap.bmHeight, 0,
                (channel == 4 ? GL_BGRA_EXT : (channel == 3 ? GL_BGR_EXT : GL_R))
                , GL_UNSIGNED_BYTE, pBits);
    
    delete[] pBits;
}

GLTexture2D::GLTexture2D(GLuint texture, bool delTex) : tex(texture), delTex(delTex) {}

GLTexture2D::~GLTexture2D(){
    if (delTex)
        glDeleteTextures(1, &tex);
}

void GLTexture2D::Enable(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
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
    GLint logLen;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLen);
    return logLen > 0;
}

bool GLComputeProgram::CheckShaderError(){
    GLint logLen;
    glGetShaderiv(prog, GL_INFO_LOG_LENGTH, &logLen);
    return logLen > 0;
}

void GLComputeProgram::PrintProgramLog(){
    char* log;
    GLint logLen;

    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen){
        log = new char[logLen];
        glGetProgramInfoLog(prog, logLen, &logLen, log);
        DebugError("%s", log);
        delete[] log;
    }
}

void GLComputeProgram::PrintShaderLog(){
    char* log;
    GLint logLen;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen){
        log = new char[logLen];
        glGetShaderInfoLog(shader, logLen, &logLen, log);
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