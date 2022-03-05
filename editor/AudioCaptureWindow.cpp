#include <editor/AudioCaptureWindow.h>

#include <lib/opengl/gl/gl.h>
#include <lib/openal/al.h>
#include <lib/openal/alc.h>

#include <main.h>
#include <res.h>
#include <utils/AudioUtils.h>
#include <utils/math3d/Math.h>
#include <utils/math3d/LinearAlgebra.h>
#include <utils/os/Font.h>
#include <utils/os/Shell.h>
#include <utils/gl/GLUtils.h>

// 应在一切windows相关类型定义之后
#include <lib/soundtouch/SoundTouch.h>

AudioCaptureWindow::ProgressBar::ProgressBar(AudioCaptureWindow* window) : window(window) {}
AudioCaptureWindow::ProgressBar::~ProgressBar(){}

bool AudioCaptureWindow::ProgressBar::Trigger(Vector2 pos){
    return pos.x >= 0.8f && pos.x <= 1.0f && pos.y >= this->pos - 0.05f && pos.y <= this->pos + 0.05f;
}

void AudioCaptureWindow::ProgressBar::Click(Vector2 pos){
    origin = this->pos;
}

void AudioCaptureWindow::ProgressBar::Drag(Vector2 dir){
    pos = Clamp(origin + dir.y, -0.8f, 0.8f);
    window->soundTouch->setPitchSemiTones(pos * 1.25f * 12.0f);
}

void AudioCaptureWindow::ProgressBar::Hover(Vector2 pos){
    hover = true;
}

void AudioCaptureWindow::ProgressBar::Leave(Vector2 pos){
    hover = false;
}

void AudioCaptureWindow::ProgressBar::Render(){
    glLineWidth(10.0f);
    glColor3f(0.6f, 0.6f, 0.6f);
    glBegin(GL_LINES);
    glVertex2f(0.9f, -0.8f);
    glVertex2f(0.9f, 0.8f);
    glEnd();
    glLineWidth(1.0f);

    if (hover){
        glColor3f(0.0f, 0.0f, 0.3f);
    }else{
        glColor3f(0.0f, 0.0f, 0.5f);
    }
    GLUtils::DrawRect(0.8f, pos - 0.05f, 1.0f, pos + 0.05f);
}

AudioCaptureWindow::CaptureItem::CaptureItem(AudioCaptureWindow* window) : window(window) {}
AudioCaptureWindow::CaptureItem::~CaptureItem(){}

const wchar_t* AudioCaptureWindow::CaptureItem::GetName(){
    return window->capture ? L"录音:开" : L"录音:关";
}

void AudioCaptureWindow::CaptureItem::OnClick(){
    window->capture ? window->Stop() : window->Launch();
    DebugLog("AudioCaptureWindow::CaptureItem State %s", window->capture ? "Capturing" : "Stopped");
}

AudioCaptureWindow::DisplayModeItem::DisplayModeItem(AudioCaptureWindow* window) : window(window) {}
AudioCaptureWindow::DisplayModeItem::~DisplayModeItem(){}

const wchar_t* AudioCaptureWindow::DisplayModeItem::GetName(){
    return window->displayWave ? L"显示模式:波形" : L"显示模式:频谱";
}

void AudioCaptureWindow::DisplayModeItem::OnClick(){
    window->displayWave = !window->displayWave;
    DebugLog("AudioCaptureWindow::DisplayModeItem State %s", window->displayWave ? "Wave" : "Frequency");
}

AudioCaptureWindow::AdjushWaveItem::AdjushWaveItem(AudioCaptureWindow* window) : window(window) {}
AudioCaptureWindow::AdjushWaveItem::~AdjushWaveItem(){}

const wchar_t* AudioCaptureWindow::AdjushWaveItem::GetName(){
    return window->adjustWave ? L"调整波形:开" : L"调整波形:关";
}

void AudioCaptureWindow::AdjushWaveItem::OnClick(){
    window->adjustWave = !window->adjustWave;
    DebugLog("AudioCaptureWindow::AdjushWaveItem State %s", window->adjustWave ? "On" : "Off");
}

AudioCaptureWindow::AudioCaptureWindow(){
    DebugLog("AudioCaptureWindow Launched");
    
    //SoundTouch 貌似在我的电脑上性能有限，不能很好做到实时变音
    soundTouch = new soundtouch::SoundTouch();
    soundTouch->setChannels(1);
    soundTouch->setSampleRate(freq);
    soundTouch->setPitchSemiTones(0);
    soundTouch->setTempo(1.0f);
    soundTouch->clear();

    uiMgr = new UIManager();

    uiMgr->AddButton(new ProgressBar(this));

    basicMenu = new Menu();
    basicMenu->AddItem(new CaptureItem(this));
    basicMenu->AddItem(new DisplayModeItem(this));
    basicMenu->AddItem(new AdjushWaveItem(this));
    basicMenu->AddItem(new MenuItem(L"刷新输入设备", MENUITEM_LAMBDA_TRANS(AudioCaptureWindow)[](AudioCaptureWindow* window){
        if (window->capDev){
            if (window->capture){
                window->Stop();
                window->CloseCaptureDevice();
                window->Launch();
            }else{
                window->CloseCaptureDevice();
            }
        }
    }, this));

    alGenSources(1, &alSrc);
    alGenBuffers(1 << queueBit, alBuf);
    alSourcePlay(alSrc);

    capBuf = new short[1 << bit];
    memset(capBuf, 0, (1 << (bit + 1)));
    
    recBuf = new short[1 << bit];
    memset(recBuf, 0, (1 << (bit + 1)));

    freqBuf = new Complex[1 << bit];
}

AudioCaptureWindow::~AudioCaptureWindow(){
    DebugLog("AudioCaptureWindow Destroyed");
    if (uiMgr) delete uiMgr;
    if (capBuf) delete[] (short*)capBuf;
    if (recBuf) delete[] (short*)recBuf;
    if (freqBuf) delete[] freqBuf;

    CloseCaptureDevice();

    if (soundTouch){
        soundTouch->clear();
        delete soundTouch;
    }
}

void AudioCaptureWindow::ProcessInput(){
    ALint cnt;
    ALint offset = (capOffset & ((1 << bit) - 1));
    float sum;

    alcGetIntegerv(capDev, ALC_CAPTURE_SAMPLES, 1, &cnt);
    cnt = Min(cnt, (1 << bit) - offset);

    alcCaptureSamples(capDev, (soundtouch::SAMPLETYPE*)capBuf + offset, cnt);

    if (soundTouch->numUnprocessedSamples() <= (1 << (bit + queueBit)) && tail - head < (1 << queueBit) - 1){
        soundTouch->putSamples((soundtouch::SAMPLETYPE*)capBuf + offset, cnt);
    }

    capOffset += cnt;
    
    if (displayWave && adjustWave){
        for (int i = 0; i < (1 << bit); i++){
            short val = ((short*)capBuf)[(i + capOffset) & ((1 << bit) - 1)];
            freqBuf[i] = val;
            sum += Abs((float)val);
        }
        if (sum * ratio < (1ll << (bit + 15)) * 0.01f){
            if (ratio < 100)
                ratio *= 10;
        }else if (sum * ratio > (1ll << (bit + 15))){
            if (ratio > 1)
                ratio /= 10;
        }
    }else{
        for (int i = 0; i < (1 << bit); i++){
            freqBuf[i] = ((short*)capBuf)[(i + capOffset) & ((1 << bit) - 1)];
        }
    }

    AudioUtils::FFT(freqBuf, bit, false);

    if (displayWave){
        glDisable(GL_LINE_SMOOTH);
        glLineWidth(1.0f);
        glBegin(GL_LINE_STRIP);
        glColor3f(1.0f, 1.0f, 1.0f);
        if (adjustWave){
            for (int i = 0; i < 1024; i++){
                float rate = i / 1024.0f;
                float amp = Clamp(((short*)capBuf)[((i << (bit - 10)) + capOffset) & ((1 << bit) - 1)] * 0.000030517578125f * ratio, -1.0f, 1.0f);
                glVertex2f(rate * 2.0f - 1.0f, (amp + 1.0f) * 0.5f);
            }
        }else{
            for (int i = 0; i < 1024; i++){
                float rate = i / 1024.0f;
                float amp = Clamp(((short*)capBuf)[((i << (bit - 10)) + capOffset) & ((1 << bit) - 1)] * 0.000030517578125f, -1.0f, 1.0f);
                glVertex2f(rate * 2.0f - 1.0f, (amp + 1.0f) * 0.5f);
            }
        }
        glEnd();
    }else{
        glDisable(GL_LINE_SMOOTH);
        glLineWidth(1.0f);
        glBegin(GL_LINES);
        for (int i = 0; i < 1024; i++){
            float rate = i / 1024.0f;
            float amp = Clamp(Log(freqBuf[i << (bit - 10)].SqrMagnitude()) * 0.1f, 2.0f / size.y, 1.0f);
            glColor3f(rate, 1.0f - rate, 0.0f);
            glVertex2f(rate * 2.0f - 1.0f, (-amp + 1.0f) * 0.5f);
            glVertex2f(rate * 2.0f - 1.0f, (amp + 1.0f) * 0.5f);
        }
        glEnd();
    }
}

void AudioCaptureWindow::ProcessOutput(){
    ALint cnt;
    ALint offset = (recOffset & ((1 << bit) - 1));

    cnt = Min((ALint)soundTouch->numSamples(), (1 << bit) - offset);

    soundTouch->receiveSamples((soundtouch::SAMPLETYPE*)recBuf + offset, cnt);
    if (cnt == (1 << bit) - offset)
        UpdateBuffer(recBuf, 1 << (bit + 1));
    
    recOffset += cnt;

    for (int i = 0; i < (1 << bit); i++)
        freqBuf[i] = ((short*)recBuf)[(i + recOffset) & ((1 << bit) - 1)];
    AudioUtils::FFT(freqBuf, bit, false);

    if (displayWave){
        glDisable(GL_LINE_SMOOTH);
        glLineWidth(1.0f);
        glBegin(GL_LINE_STRIP);
        glColor3f(1.0f, 1.0f, 1.0f);
        if (adjustWave){
            for (int i = 0; i < 1024; i++){
                float rate = i / 1024.0f;
                float amp = Clamp(((short*)recBuf)[((i << (bit - 10)) + recOffset) & ((1 << bit) - 1)] * 0.000030517578125f * ratio, -1.0f, 1.0f);
                glVertex2f(rate * 2.0f - 1.0f, (amp - 1.0f) * 0.5f);
            }
        }else{
            for (int i = 0; i < 1024; i++){
                float rate = i / 1024.0f;
                float amp = Clamp(((short*)recBuf)[((i << (bit - 10)) + recOffset) & ((1 << bit) - 1)] * 0.000030517578125f, -1.0f, 1.0f);
                glVertex2f(rate * 2.0f - 1.0f, (amp - 1.0f) * 0.5f);
            }
        }
        glEnd();
    }else{
        glDisable(GL_LINE_SMOOTH);
        glLineWidth(1.0f);
        glBegin(GL_LINES);
        for (int i = 0; i < 1024; i++){
            float rate = i / 1024.0f;
            float amp = Clamp(Log(freqBuf[i << (bit - 10)].SqrMagnitude()) * 0.1f, 2.0f / size.y, 1.0f);
            glColor3f(rate, 1.0f - rate, 0.0f);
            glVertex2f(rate * 2.0f - 1.0f, (-amp - 1.0f) * 0.5f);
            glVertex2f(rate * 2.0f - 1.0f, (amp - 1.0f) * 0.5f);
        }
        glEnd();
    }
}

bool AudioCaptureWindow::IsFocus(){
    return focus;
}

void AudioCaptureWindow::OnRender(){
    ALint cnt;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    GLUtils::ResetProjection();
    GLUtils::ResetModelView();

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.0f); glVertex2f(1.0f, 0.0f);
    glEnd();

    if (capture){
        ProcessInput();
        ProcessOutput();
    }

    uiMgr->Render();
}

void AudioCaptureWindow::OnCreate(){
    Launch();
}

void AudioCaptureWindow::OnClose(){}

void AudioCaptureWindow::OnTimer(int id){
    Main::RequestRender();
}

void AudioCaptureWindow::OnChar(char c){}

void AudioCaptureWindow::OnUnichar(wchar_t c){}

void AudioCaptureWindow::OnResize(int x, int y){
    size.x = x;
    size.y = y;
}

void AudioCaptureWindow::OnMouseMove(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
}

void AudioCaptureWindow::OnLeftDown(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
    uiMgr->LeftDown();
}

void AudioCaptureWindow::OnLeftUp(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
    uiMgr->LeftUp();
}

void AudioCaptureWindow::OnRightDown(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
    Main::SetMenu(basicMenu);
}

void AudioCaptureWindow::OnRightUp(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
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

void AudioCaptureWindow::OnMenuAccel(int id, bool accel){
    switch (id){
    case IDM_MENU_BASIC:
        Main::SetMenu(basicMenu);
        break;
    }
}

void AudioCaptureWindow::OnDropFileA(const char* path){}

void AudioCaptureWindow::OnDropFileW(const wchar_t* path){}

void AudioCaptureWindow::Launch(){
    if (capture){
        DebugError("AudioCaptureWindow::Launch Already Launched");
        return;
    }

    if (!SetCaptureDevice(NULL))
        return;

    alcCaptureStart(capDev);
    capture = true;

    DebugLog("AudioCaptureWindow::Launch Success");
}

void AudioCaptureWindow::Stop(){
    alcCaptureStop(capDev);
    capture = false;
}

void AudioCaptureWindow::UpdateBuffer(void* buf, int size){
    int cnt;
    int state;

    //DebugLog("AudioCaptureWindow %p Queue Buffer %d %d", this, head, alBuf[head & queueMask]);
    alBufferData(alBuf[head & queueMask], AL_FORMAT_MONO16, buf, size, freq);
    alSourceQueueBuffers(alSrc, 1, &alBuf[(head++) & queueMask]);

    alGetSourcei(alSrc, AL_BUFFERS_PROCESSED, &cnt);
    while (cnt--){
        //DebugLog("AudioCaptureWindow %p Unqueue Buffer %d %d", this, tail, alBuf[tail & queueMask]);
        alSourceUnqueueBuffers(alSrc, 1, &alBuf[(tail++) & queueMask]);
    }

    alGetSourcei(alSrc, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING){
        alSourcePlay(alSrc);
    }
}

bool AudioCaptureWindow::SetCaptureDevice(const char* name){
    if (capDev){
        return true;
    }

    capDev = alcCaptureOpenDevice(name, freq, AL_FORMAT_MONO16, 1 << (bit + queueBit));

    if (!capDev){
        DebugError("AudioCaptureWindow::SetCaptureDevice Capture Device %s Not Found", name ? name : "(default)");
        return false;
    }

    DebugLog("AudioCaptureWindow::SetCaptureDevice Open Device %p %s", capDev, alcGetString(capDev, ALC_DEVICE_SPECIFIER));

    return true;
}

bool AudioCaptureWindow::CloseCaptureDevice(){
    ALCboolean res;

    if (!capDev){
        return false;
    }

    res = alcCaptureCloseDevice(capDev);
    capDev = NULL;

    return res;
}