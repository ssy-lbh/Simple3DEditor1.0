#include "gltools.h"

#include "vecmath.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

void GLUtils::DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC){
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
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
                (channel == 4 ? GL_RGBA : (channel == 3 ? GL_RGB : GL_R))
                , GL_UNSIGNED_BYTE, pBits);
    
    delete[] pBits;
}

GLTexture2D::~GLTexture2D(){
    glDeleteTextures(1, &tex);
}

void GLTexture2D::Enable(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
}

void GLTexture2D::Disable(){
    glDisable(GL_TEXTURE_2D);
}