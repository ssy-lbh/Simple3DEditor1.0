#include <utils/AudioUtils.h>

#include <lib/openal/al.h>
#include <lib/openal/alc.h>

#include <utils/math3d/Math.h>
#include <utils/math3d/LinearAlgebra.h>
#include <utils/os/Log.h>

bool AudioUtils::init = false;

ALCdevice* AudioUtils::alDev = NULL;
ALCcontext* AudioUtils::alCtx = NULL;

AudioUtils::Complex::Complex(_Complex float comp) : comp(comp) {}
AudioUtils::Complex::~Complex(){}

float AudioUtils::Complex::MagnitudeSqr(){
    return real * real + imag * imag;
}

float AudioUtils::Complex::Magnitude(){
    return __builtin_sqrt(real * real + imag * imag);
}

bool AudioUtils::CheckALError(const char* tag, const char* file, int line){
    int loopCnt = 0;
	for (ALenum error = alGetError(); loopCnt < 32 && error != AL_NO_ERROR; error = alGetError(), ++loopCnt){
		const char* pMsg;
		switch (error){
		case AL_INVALID_NAME: pMsg = "Invalid name"; break;
		case AL_INVALID_ENUM: pMsg = "Invalid enum"; break;
		case AL_INVALID_VALUE: pMsg = "Invalid value"; break;
		case AL_INVALID_OPERATION: pMsg = "Invalid operation"; break;
		case AL_OUT_OF_MEMORY: pMsg = "Out of memory"; break;
		default: pMsg = "Unknown Error";
		}
		DebugError("[OpenAL Error] %s %s(0x%x) at %s:%d\n", tag, pMsg, error, file, line);
	}
	return loopCnt != 0;
}

void AudioUtils::FFT(_Complex float* input, int sizebit, bool inv){
    int size = 1 << sizebit;
    int* rev = new int[size];
    rev[0] = 0;
    for (int i = 1; i < size; i++){
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) ? (1 << (sizebit - 1)) : 0);
    }
    for (int i = 0; i < size; i++){
        if (i < rev[i]){
            _Complex float tmp;
            tmp = input[i];
            input[i] = input[rev[i]];
            input[rev[i]] = tmp;
        }
    }
    delete[] rev;
    for (int i = 1; i <= sizebit; i++){
        int half = (1 << (i - 1)), num = (1 << i);
        for (int j = 0; j < (1 << (sizebit - i)); j++){
            _Complex float o, m;
            m = 1.0f + 0.0if;
            o = __builtin_sin(PI / num) * (inv ? -1.0if : 1.0if) + __builtin_cos(PI / num);
            for (int k = (j << i); k < (j << i) + half; k++){
                _Complex float A = input[k], B = input[k + half];
                input[k] = A + B * o;//单位根份数
                input[k + half] = A - B * o;
                m *= o;
            }
        }
    }
    if (!inv){
        for (int i = 0; i < size; i++){
            input[i] /= size;
        }
    }
}

void AudioUtils::InitOpenAL(){
    if (init){
        return;
    }

    init = true;

    alDev = alcOpenDevice(NULL);
    alCtx = alcCreateContext(alDev, NULL);

    alcMakeContextCurrent(alCtx);

    PrintOpenALInfo();
    LoadOpenALPreset();
}

void AudioUtils::UninitOpenAL(){
    if (init){
        init = false;
        
        alcCloseDevice(alDev);
        alcDestroyContext(alCtx);
    }
}

void AudioUtils::PrintOpenALInfo(){
    DebugLog("OpenAL Enabled");
    DebugLog("OpenAL Version %s", alGetString(AL_VERSION));
    DebugLog("OpenAL Renderer %s", alGetString(AL_RENDERER));
    DebugLog("OpenAL Vendor %s", alGetString(AL_VENDOR));
    //DebugLog("OpenAL Extensions %s", alGetString(AL_EXTENSIONS));

    if (!alDev){
        DebugError("OpenAL Device Not Found");
        return;
    }

    DebugLog("OpenAL Device %p %s", alDev, alcGetString(alDev, ALC_DEVICE_SPECIFIER));
}

void AudioUtils::LoadOpenALPreset(){
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alListener3f(AL_ORIENTATION, 0.0f, 1.0f, 0.0f);

    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
}

void alListenerv3(uenum param, Vector3 value){
    alListener3f(param, value.x, value.y, value.z);
}