#include <utils/os/Resource.h>

#include <windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include <lib/stb_image/stb_image.h>

#include <res.h>
#include <utils/String.h>
#include <utils/DataBuffer.h>
#include <utils/os/Log.h>

String Resource::GetString(int id){
    const char* pstr;
    size_t len;

    len = LoadStringA(GetModuleHandleA(NULL), id, (LPSTR)&pstr, 0);
    if (!pstr){
        DebugError("Critical: Resource::GetString(%d) LoadStringA Return NULL", id);
        return String();
    }
    return String(pstr, len);
}

WString Resource::GetWString(int id){
    const wchar_t* pstr;
    size_t len;

    len = LoadStringW(GetModuleHandleA(NULL), id, (LPWSTR)&pstr, 0);
    if (!pstr){
        DebugError("Critical: Resource::GetWString(%d) LoadStringW Return NULL", id);
        return WString();
    }
    return WString(pstr, len);
}

DataBuffer Resource::GetBitmap(int id, int* x, int* y, int* comp){
    HBITMAP hBitmap;
    BITMAP bitmap;
    unsigned char* pBits;
    DWORD dwSize;

    hBitmap = LoadBitmapA(GetModuleHandleA(NULL), MAKEINTRESOURCEA(id));
    GetObjectA(hBitmap, sizeof(BITMAP), &bitmap);
    dwSize = bitmap.bmHeight * bitmap.bmWidthBytes;
    pBits = new unsigned char[dwSize];
    GetBitmapBits(hBitmap, dwSize, pBits);

    *x = bitmap.bmWidth;
    *y = bitmap.bmHeight;
    *comp = bitmap.bmBitsPixel >> 3;

    DataBuffer res(pBits, dwSize);
    delete[] pBits;

    return res;
}

DataBuffer Resource::GetTexture(int id, int* x, int* y, int* comp, int reqComp){
    HINSTANCE hInst = GetModuleHandleA(NULL);
    HRSRC resSrc;
    HGLOBAL resIdx;
    LPVOID resPtr;
    DWORD resSize;
    char* srcData;
    stbi_uc* image;

    resSrc = FindResourceA(hInst, MAKEINTRESOURCE(id), MAKEINTRESOURCE(TEXTURE));
    resIdx = LoadResource(hInst, resSrc);
    resPtr = LockResource(resIdx);
    resSize = SizeofResource(hInst, resSrc);
    srcData = new char[resSize];
    RtlCopyMemory(srcData, resPtr, resSize);
    FreeResource(resIdx);

    image = stbi_load_from_memory((stbi_uc*)srcData, resSize, x, y, comp, reqComp);
    DataBuffer res(image, *x * *y * *comp);
    stbi_image_free(image);

    return res;
}

DataBuffer Resource::GetTexture(String path, int* x, int* y, int* comp, int reqComp){
    stbi_uc* image;

    image = stbi_load(path.GetString(), x, y, comp, reqComp);
    DataBuffer res(image, *x * *y * *comp);
    stbi_image_free(image);

    return res;
}

DataBuffer Resource::GetShader(int id){
    HINSTANCE hInst = GetModuleHandleA(NULL);
    HRSRC kernelSrc;
    HGLOBAL resIdx;
    LPVOID resPtr;
    DWORD resSize;
    char* srcData;

    kernelSrc = FindResourceA(hInst, MAKEINTRESOURCE(id), MAKEINTRESOURCE(SHADER));
    resIdx = LoadResource(hInst, kernelSrc);
    resPtr = LockResource(resIdx);
    resSize = SizeofResource(hInst, kernelSrc);
    srcData = new char[resSize + 1];
    RtlCopyMemory(srcData, resPtr, resSize);
    FreeResource(resIdx);

    srcData[resSize] = '\0';
    DataBuffer res(srcData, resSize + 1);
    delete[] srcData;

    return res;
}