#include <utils/os/Resource.h>

#include <windows.h>

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

    if (x) *x = bitmap.bmWidth;
    if (y) *y = bitmap.bmHeight;
    if (comp) *comp = bitmap.bmBitsPixel >> 3;

    return DataBuffer(pBits, dwSize);
}

DataBuffer Resource::GetTexture(String path, int* x, int* y, int* comp, int reqComp){
    DebugError("Resource::GetTexture [Unimplemented]");
    return DataBuffer();
}