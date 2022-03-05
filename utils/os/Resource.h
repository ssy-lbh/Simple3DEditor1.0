#ifndef __UTILS_OS_RESOURCE__
#define __UTILS_OS_RESOURCE__

#include <define.h>

class Resource final {
public:
    static String GetString(int id);
    static WString GetWString(int id);
    static DataBuffer GetBitmap(int id, int* x, int* y, int* comp);
    static DataBuffer GetTexture(int id, int* x, int* y, int* comp, int reqComp);
    static DataBuffer GetTexture(String path, int* x, int* y, int* comp, int reqComp);
    static DataBuffer GetTexture(WString path, int* x, int* y, int* comp, int reqComp);
    static DataBuffer GetShader(int id);
};

#endif