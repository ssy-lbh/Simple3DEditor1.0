#include <utils/gl/GLTexture2D.h>

#include <windows.h>

#include <lib/opengl/gl/gl.h>
#include <utils/os/Log.h>

#define STB_IMAGE_IMPLEMENTATION
#include <lib/stb_image/stb_image.h>

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