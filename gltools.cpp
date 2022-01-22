#include "gltools.h"

#include "vecmath.h"

#ifdef STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

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

#ifdef STB_IMAGE_IMPLEMENTATION
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
#endif

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