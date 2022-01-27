#include "audio.h"

#include "opengl/gl/gl.h"

#include "font.h"
#include "gltools.h"

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
		case AL_INVALID_NAME: pMsg = "invalid name"; break;
		case AL_INVALID_ENUM: pMsg = "invalid enum"; break;
		case AL_INVALID_VALUE: pMsg = "invalid value"; break;
		case AL_INVALID_OPERATION: pMsg = "invalid operation"; break;
		case AL_OUT_OF_MEMORY: pMsg = "out of memory"; break;
		default: pMsg = "unknown error";
		}
		__builtin_printf("[OpenAL Error] %s %s(0x%x) at %s:%d\n", tag, pMsg, error, file, line);
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

AudioPlayerWindow::PlayButton::PlayButton(AudioPlayerWindow* window) : window(window) {}
AudioPlayerWindow::PlayButton::~PlayButton(){}

bool AudioPlayerWindow::PlayButton::Trigger(Vector2 pos){
    return pos.x >= -0.5f && pos.x <= 0.5f && pos.y >= -0.8f && pos.y <= -0.2f;
}

void AudioPlayerWindow::PlayButton::Hover(){
    hover = true;
}

void AudioPlayerWindow::PlayButton::Click(){
    if (window->IsLaunched()){
        window->Stop();
    }else{
        window->Launch();
    }
}

void AudioPlayerWindow::PlayButton::Leave(){
    hover = false;
}

void AudioPlayerWindow::PlayButton::Render(){
    if (hover){
        glColor3f(1.0f, 0.5f, 0.0f);
    }else{
        glColor3f(0.0f, 0.4f, 1.0f);
    }
    GLUtils::DrawRoundRect(-0.5f, -0.8f, 1.0f, 0.6f, 0.2f, 0.05f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.15f, -0.7f);
    glVertex2f(-0.15f, -0.3f);
    glVertex2f(0.25f, -0.5f);
    glEnd();
}

AudioPlayerWindow::ProgressBar::ProgressBar(AudioPlayerWindow* window) : window(window) {}
AudioPlayerWindow::ProgressBar::~ProgressBar(){}

bool AudioPlayerWindow::ProgressBar::Trigger(Vector2 pos){
    return pos.x >= this->pos - 0.05f && pos.x <= this->pos + 0.05f && pos.y >= -0.1f && pos.y <= 0.1f;
}

void AudioPlayerWindow::ProgressBar::Click(){
    origin = pos;
}

void AudioPlayerWindow::ProgressBar::Drag(Vector2 dir){
    ALint offset;

    pos = Clamp(origin + dir.x, -0.6f, 0.6f);
    offset = Clamp((int)(((pos + 0.6f) / 1.2f) * window->alAudioSize), 0, window->alAudioSize);
    offset &= 0xFFFFFFFC;// 去除最后两位，避免偏移到样本中间

    alSourcei(window->alSrc, AL_BYTE_OFFSET, offset);
}

void AudioPlayerWindow::ProgressBar::Hover(){
    hover = true;
}

void AudioPlayerWindow::ProgressBar::Leave(){
    hover = false;
}

AudioPlayerWindow::LoopOption::LoopOption(AudioPlayerWindow* window) : window(window) {}
AudioPlayerWindow::LoopOption::~LoopOption(){}

bool AudioPlayerWindow::LoopOption::Trigger(Vector2 pos){
    return pos.x >= 0.7f && pos.x <= 0.9f && pos.y >= -0.1f && pos.y <= 0.1f;
}

void AudioPlayerWindow::LoopOption::Click(){
    loop = !loop;
    DebugLog("AudioPlayerWindow::LoopOption State %s", loop ? "Looping" : "Default");
    alSourcei(window->alSrc, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

void AudioPlayerWindow::LoopOption::Render(){
    if (loop){
        glColor3f(0.0f, 0.0f, 1.0f);
    }else{
        glColor3f(0.0f, 0.0f, 0.0f);
    }
    GLUtils::DrawRect(0.7f, -0.1f, 0.9f, 0.1f);
}

void AudioPlayerWindow::ProgressBar::Render(){
    if (window->launched){
        ALint offset;

        alGetSourcei(window->alSrc, AL_BYTE_OFFSET, &offset);

        glLineWidth(10.0f);
        glColor3f(0.6f, 0.6f, 0.6f);
        glBegin(GL_LINES);
        glVertex2f(-0.6f, 0.0f);
        glVertex2f(0.6f, 0.0f);
        glEnd();
        glLineWidth(1.0f);

        pos = Lerp(-0.6f, 0.6f, (float)offset / window->alAudioSize);
        if (hover){
            glColor3f(0.0f, 0.0f, 0.3f);
        }else{
            glColor3f(0.0f, 0.0f, 0.5f);
        }
        GLUtils::DrawRect(pos - 0.05f, -0.1f, pos + 0.05f, 0.1f);
    }
}

AudioPlayerWindow::AudioPlayerWindow(){
    uiMgr = new UIManager();

    uiMgr->AddButton(new PlayButton(this));
    uiMgr->AddButton(new ProgressBar(this));
    uiMgr->AddButton(new LoopOption(this));

    path[0] = L'\0';
}

AudioPlayerWindow::~AudioPlayerWindow(){
    if (uiMgr) delete uiMgr;

    if (launched){
        alSourceStop(alSrc);

        alDeleteBuffers(1, &alBuf);
        alDeleteSources(1, &alSrc);

        alcDestroyContext(alCtx);
        alcCloseDevice(alDev);
    }
}

bool AudioPlayerWindow::IsFocus(){
    return focus;
}

void AudioPlayerWindow::OnRender(){
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    glClearDepth(1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!wcscmp(path, L"")){
        glColor3f(1.0f, 0.5f, 0.0f);
        glRasterPos2f(-1.0f, 1.0f - 30.0f / size.y);
        glDrawCNString(L"尚未存在音频文件");
        glRasterPos2f(-1.0f, 1.0f - 60.0f / size.y);
        glDrawCNString(L"可以将文件拖拽至此处");
        return;
    }

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    uiMgr->Render();
}

void AudioPlayerWindow::OnCreate(HWND hWnd){
    this->hWnd = hWnd;
}

void AudioPlayerWindow::OnClose(){}

void AudioPlayerWindow::OnResize(int x, int y){
    if (x == 0 || y == 0){
        return;
    }
    size.x = x;
    size.y = y;
}

void AudioPlayerWindow::OnMouseMove(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
}

void AudioPlayerWindow::OnLeftDown(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
    uiMgr->LeftDown();
}

void AudioPlayerWindow::OnLeftUp(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
    uiMgr->LeftUp();
}

void AudioPlayerWindow::OnRightDown(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
}

void AudioPlayerWindow::OnRightUp(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
}

void AudioPlayerWindow::OnMouseHover(int key, int x, int y){}

void AudioPlayerWindow::OnMouseLeave(){}

void AudioPlayerWindow::OnFocus(){
    focus = true;
}

void AudioPlayerWindow::OnKillFocus(){
    focus = false;
}

void AudioPlayerWindow::OnMouseWheel(int delta){}

void AudioPlayerWindow::OnMenuAccel(int id, bool accel){}

void AudioPlayerWindow::OnDropFileA(const char* path){}

void AudioPlayerWindow::OnDropFileW(const wchar_t* path){
    DebugLog("AudioPlayerWindow::OnDropFileW %s", path);
    wcscpy_s(this->path, path);
    if (!loaded){
        Load(path);
    }
}

void AudioPlayerWindow::Load(const wchar_t* file){
    if (!wcscmp(path, L"")){
        return;
    }

    DebugLog("AudioPlayerWindow::Load From %s", path);

    HANDLE hFile = CreateFileW(
        path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE){
        DebugError("AudioPlayerWindow::Load File Open Failed");
        return;
    }

    char* fileData;

    DebugLog("OpenAL Started");

    alDev = alcOpenDevice(NULL);
    alCtx = alcCreateContext(alDev, NULL);

    alcMakeContextCurrent(alCtx);

    alGenSources(1, &alSrc);
    alGenBuffers(1, &alBuf);

    // analyse data segment
    DWORD specNum;
    DWORD readLen;
    DWORD dataLen;
    WAVEFORMATEX wav;
    bool fmtRead = false;
    ReadFile(hFile, &specNum, 4, NULL, NULL);
    if (specNum != *(DWORD*)"RIFF"){
        DebugError("AudioPlayerWindow::Load File Magic Number 'RIFF' Not Found");
        return;
    }
    ReadFile(hFile, &specNum, 4, NULL, NULL);
    ReadFile(hFile, &specNum, 4, NULL, NULL);
    if (specNum != *(DWORD*)"WAVE"){
        DebugError("AudioPlayerWindow::Load File Magic Number 'WAVE' Not Found");
        return;
    }
    ReadFile(hFile, &specNum, 4, &readLen, NULL);
    while (true){
        if (readLen != 4){
            return;
        }
        if (specNum == *(DWORD*)"fmt "){
            ReadFile(hFile, &specNum, 4, &readLen, NULL);
            if (specNum != 0x10){
                DebugError("AudioPlayerWindow::Load File Format Length Is Not 0x10 [%d]", specNum);
                return;
            }
            ReadFile(hFile, &wav, 0x10, NULL, NULL);
            wav.wBitsPerSample = wav.nBlockAlign << 3;
            fmtRead = true;
            ReadFile(hFile, &specNum, 4, &readLen, NULL);
            continue;
        }else if (specNum == *(DWORD*)"data"){
            ReadFile(hFile, &dataLen, 4, NULL, NULL);
            break;
        }
        ReadFile(hFile, &specNum, 4, NULL, NULL);
        SetFilePointer(hFile, specNum, NULL, FILE_CURRENT);
        ReadFile(hFile, &specNum, 4, &readLen, NULL);
    }
    if (!fmtRead){
        DebugError("AudioPlayerWindow::Load File Format Not Found");
        return;
    }
    ALint format = GetWaveFormat(&wav);
    if (format == -1){
        DebugError("AudioPlayerWindow::Load File Format Unrecognized");
        return;
    }

    fileData = new char[dataLen];
    ReadFile(hFile, fileData, dataLen, NULL, NULL);
    alBufferData(alBuf, format, fileData, dataLen, wav.nSamplesPerSec);// 11025 44100
    delete[] fileData;
    CloseHandle(hFile);

    alAudioSize = dataLen;

    DebugLog("AudioPlayerWindow::Load Success");

    loaded = true;
}

bool AudioPlayerWindow::IsLoaded(){
    return loaded;
}

void AudioPlayerWindow::Launch(){
    if (!wcscmp(path, L"")){
        return;
    }

    if (!loaded){
        DebugError("AudioPlayerWindow::Launch Is Not Loaded");
        return;
    }

    ALint cnt;
    alGetSourcei(alSrc, AL_BUFFERS_QUEUED, &cnt);
    if (cnt == 0){
        alSourceQueueBuffers(alSrc, 1, &alBuf);
    }
    
    alSourcePlay(alSrc);

    DebugLog("AudioPlayerWindow::Launch Success");

    launched = true;
}

void AudioPlayerWindow::Stop(){
    alSourceStop(alSrc);
    alSourceUnqueueBuffers(alSrc, 1, &alBuf);
}

bool AudioPlayerWindow::IsLaunched(){
    ALint state;
    alGetSourcei(alSrc, AL_SOURCE_STATE, &state);
    if (state == AL_PLAYING){
        launched = true;
    }else{
        launched = false;
    }
    return launched;
}

ALint AudioPlayerWindow::GetWaveFormat(PWAVEFORMATEX lpwav){
    ALenum format = -1;
    switch (lpwav->nChannels){
    case 1:
        switch (lpwav->nBlockAlign){
        case 1:
            format = AL_FORMAT_MONO8;
            break;
        case 2:
            format = AL_FORMAT_MONO16;
            break;
        }
        break;
    case 2:
        switch (lpwav->nBlockAlign){
        case 2:
            format = AL_FORMAT_STEREO8;
            break;
        case 4:
            format = AL_FORMAT_STEREO16;
            break;
        }
        break;
    }
    return format;
}

AudioCaptureWindow::AudioCaptureWindow(){}
AudioCaptureWindow::~AudioCaptureWindow(){}

bool AudioCaptureWindow::IsFocus(){
    return focus;
}

void AudioCaptureWindow::OnRender(){}

void AudioCaptureWindow::OnCreate(HWND hWnd){
    this->hWnd = hWnd;
}

void AudioCaptureWindow::AudioCaptureWindow::OnClose(){}

void AudioCaptureWindow::OnResize(int x, int y){
    size.x = x;
    size.y = y;
}

void AudioCaptureWindow::OnMouseMove(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
}

void AudioCaptureWindow::OnLeftDown(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
}

void AudioCaptureWindow::OnLeftUp(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
}

void AudioCaptureWindow::OnRightDown(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
}

void AudioCaptureWindow::OnRightUp(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
}

void AudioCaptureWindow::OnMouseHover(int key, int x, int y){}

void AudioCaptureWindow::OnMouseLeave(){}

void AudioCaptureWindow::OnFocus(){
    focus = true;
}

void AudioCaptureWindow::OnKillFocus(){
    focus = false;
}

void AudioCaptureWindow::OnMouseWheel(int delta){}

void AudioCaptureWindow::OnMenuAccel(int id, bool accel){}

void AudioCaptureWindow::OnDropFileA(const char* path){}

void AudioCaptureWindow::OnDropFileW(const wchar_t* path){}
