#ifndef __UTILS_AUDIOUTILS__
#define __UTILS_AUDIOUTILS__

#include <define.h>

#include <lib/openal/al.h>
#include <lib/openal/alc.h>

#define alCheckError(tag) AudioUtils::CheckALError(tag, __FILE__, __LINE__)

class AudioUtils {
public:
    static bool init;

    static ALCdevice* alDev;
    static ALCcontext* alCtx;

    union Complex {
        _Complex float comp;
        float real, imag;

        Complex(_Complex float comp);
        ~Complex();

        float MagnitudeSqr();
        float Magnitude();
    };

    static bool CheckALError(const char* tag, const char* file, int line);
    static void FFT(_Complex float* input, int sizebit, bool inv);
    static void InitOpenAL();
    static void UninitOpenAL();
    static void PrintOpenALInfo();
    static void LoadOpenALPreset();
};

#endif