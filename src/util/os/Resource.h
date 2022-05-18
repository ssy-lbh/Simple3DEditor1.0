#ifndef __UTIL_OS_RESOURCE__
#define __UTIL_OS_RESOURCE__

#include <define.h>

class Resource final {
public:
    static String GetString(int id);
    static WString GetWString(int id);
    static DataBuffer GetBitmap(int id, int* x, int* y, int* comp);
    static DataBuffer GetTexture(int id, int* x, int* y, int* comp, int reqComp);
    static DataBuffer GetTexture(String path, int* x, int* y, int* comp, int reqComp);
    static DataBuffer GetShader(int id);
    static DataBuffer GetBinary(int id);
    static void StoreImage(String path, PackDataBuffer data, int x, int y, int comp);
    static void StoreImage(String path, const void* data, int x, int y, int comp);
    static void StoreBMP(String path, PackDataBuffer data, int x, int y, int comp);
    static void StorePNG(String path, PackDataBuffer data, int x, int y, int comp);
    static void StoreJPG(String path, PackDataBuffer data, int x, int y, int comp, int quality);
    static void StoreTGA(String path, PackDataBuffer data, int x, int y, int comp);
    static void StoreHDR(String path, PackDataBuffer data, int x, int y, int comp);
    static void StoreBMP(String path, const void* data, int x, int y, int comp);
    static void StorePNG(String path, const void* data, int x, int y, int comp);
    static void StoreJPG(String path, const void* data, int x, int y, int comp, int quality);
    static void StoreTGA(String path, const void* data, int x, int y, int comp);
    static void StoreHDR(String path, const void* data, int x, int y, int comp);
};

#endif