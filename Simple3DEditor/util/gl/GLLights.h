#ifndef __UTIL_GL_GLLIGHTS__
#define __UTIL_GL_GLLIGHTS__

#include <define.h>

namespace simple3deditor {

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

}

#endif