#ifndef __UTILS_GL_GLLIGHTS__
#define __UTILS_GL_GLLIGHTS__

#include <define.h>

class GLLights final : public Object {
private:
    static bool use[8];
    static uenum idx[8];

public:
    static uenum Create();
    static bool Destroy(uenum light);
    static void Enable(uenum light);
    static void Disable(uenum light);
};

#endif