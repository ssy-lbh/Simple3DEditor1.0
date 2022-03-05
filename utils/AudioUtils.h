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

    static bool CheckALError(const char* tag, const char* file, int line);
    static void FFT(Complex* input, int sizebit, bool inv);
    static void InitOpenAL();
    static void UninitOpenAL();
    static void PrintOpenALInfo();
    static void LoadOpenALPreset();
};

void alListenerPosv3(Vector3 value);
void alListenerVelocityv3(Vector3 value);
void alListenerPosAutoVelv3(Vector3 value);
void alListenerDirv3(Vector3 dir, Vector3 up);

#endif