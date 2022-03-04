#ifndef __UTILS_AUDIOUTILS__
#define __UTILS_AUDIOUTILS__

#include <define.h>

typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;

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

void alListenerPosv3(Vector3 value);
void alListenerVelocityv3(Vector3 value);
void alListenerDirv3(Vector3 value);

#endif