#include <editor/AudioCaptureWindow.h>

#include <lib/opengl/gl/gl.h>
#include <lib/openal/al.h>
#include <lib/openal/alc.h>

#include <main.h>
#include <res.h>
#include <util/AudioUtils.h>
#include <util/math3d/Math.h>
#include <util/math3d/LinearAlgebra.h>
#include <util/os/Font.h>
#include <util/os/Shell.h>
#include <util/gl/GLUtils.h>
#include <editor/gui/Menu.h>
#include <editor/gui/GUIUtils.h>
#include <editor/object/GUIManagerObject.h>

// 应在一切windows相关类型定义之后
#include <lib/soundtouch/SoundTouch.h>

namespace simple3deditor {

AudioCaptureWindow::AudioCaptureWindow(){
    DebugLog("AudioCaptureWindow Launched");
    
    //SoundTouch 貌似在我的电脑上性能有限，不能很好做到实时变音
    soundTouch = new soundtouch::SoundTouch();
    soundTouch->setChannels(1);
    soundTouch->setSampleRate(FREQUENCY);
    soundTouch->setPitchSemiTones(0);
    soundTouch->setTempo(1.0f);
    soundTouch->clear();

    guiMgr = new GUIManagerObject();

    basicMenu = new Menu();
    basicMenu->AddItem(new MenuItem(
        Value<const wchar_t*>([=]{ return this->capture ? L"录音:开" : L"录音:关"; }),
        [=]{
            this->capture ? this->Stop() : this->Launch();
            DebugLog("AudioCaptureWindow::CaptureItem State %s", this->capture ? "Capturing" : "Stopped");
        }
    ));
    basicMenu->AddItem(new MenuItem(
        Value<const wchar_t*>([=]{ return this->displayWave ? L"显示模式:波形" : L"显示模式:频谱"; }),
        [=]{
            this->displayWave = !this->displayWave;
            DebugLog("AudioCaptureWindow::DisplayModeItem State %s", this->displayWave ? "Wave" : "Frequency");
        }
    ));
    basicMenu->AddItem(new MenuItem(
        Value<const wchar_t*>([=]{ return this->adjustWave ? L"调整波形:开" : L"调整波形:关"; }),
        [=]{
            this->adjustWave = !this->adjustWave;
            DebugLog("AudioCaptureWindow::AdjushWaveItem State %s", this->adjustWave ? "On" : "Off");
        }
    ));
    basicMenu->AddItem(new MenuItem(L"刷新输入设备", [=]{
        if (this->capDev){
            if (this->capture){
                this->Stop();
                this->CloseCaptureDevice();
                this->Launch();
            }else{
                this->CloseCaptureDevice();
            }
        }
    }));

    alGenSources(1, &alSrc);
    alGenBuffers(QUEUE_SIZE, alBuf);
    alSourcePlay(alSrc);

    capBuf = new short[SAMPLE_SIZE];
    memset(capBuf, 0, (SAMPLE_SIZE << 1));
    
    recBuf = new short[SAMPLE_SIZE];
    memset(recBuf, 0, (SAMPLE_SIZE << 1));

    freqBuf = new Complex[SAMPLE_SIZE];

    class FrequencyBar : public VerticalProgressBar {
    public:
        FrequencyBar() : VerticalProgressBar() {
            lowBound = -0.8f;
            highBound = 0.8f;
            posX = 0.9f;
            btnX = 0.1f;
            btnY = 0.05f;
            lineWidth = 10.0f;
            pos = 0.5f;
        }
    } *freqBar = new FrequencyBar();
    freqBar->onPosChange += [=](float pos){
        soundTouch->setPitchSemiTones(Lerp(-12.0f, 12.0f, pos));
    };
    guiMgr->AddChild(freqBar);

    Launch();
}

AudioCaptureWindow::~AudioCaptureWindow(){
    DebugLog("AudioCaptureWindow Destroyed");
    if (guiMgr) delete guiMgr;
    if (capBuf) delete[] (short*)capBuf;
    if (recBuf) delete[] (short*)recBuf;
    if (freqBuf) delete[] freqBuf;

    CloseCaptureDevice();

    if (soundTouch){
        soundTouch->clear();
        delete soundTouch;
    }
}

void AudioCaptureWindow::UpdateCursor(int x, int y){
    AWindow::UpdateCursor(x, y);
}

void AudioCaptureWindow::UpdateWindowSize(int x, int y){
    AWindow::UpdateWindowSize(x, y);
}

void AudioCaptureWindow::ProcessInput(){
    ALint cnt;
    ALint offset = (capOffset & SAMPLE_MASK);

    alcGetIntegerv(capDev, ALC_CAPTURE_SAMPLES, 1, &cnt);
    cnt = Min((uint)cnt, SAMPLE_SIZE - offset);

    alcCaptureSamples(capDev, (soundtouch::SAMPLETYPE*)capBuf + offset, cnt);

    if (soundTouch->numUnprocessedSamples() <= (1 << (SAMPLE_SIZE_BIT + QUEUE_SIZE_BIT)) && tail - head < (QUEUE_SIZE) - 1){
        soundTouch->putSamples((soundtouch::SAMPLETYPE*)capBuf + offset, cnt);
    }

    capOffset += cnt;
    
    if (displayWave && adjustWave){
        float sum = 0.0f;
        for (int i = 0; i < (SAMPLE_SIZE); i++){
            short val = ((short*)capBuf)[(i + capOffset) & SAMPLE_MASK];
            freqBuf[i] = val;
            sum += Abs((float)val);
        }
        if (sum * ratio < (1ll << (SAMPLE_SIZE_BIT + 15)) * 0.01f){
            if (ratio < 100)
                ratio *= 10;
        }else if (sum * ratio > (1ll << (SAMPLE_SIZE_BIT + 15))){
            if (ratio > 1)
                ratio /= 10;
        }
    }else{
        for (int i = 0; i < (SAMPLE_SIZE); i++){
            freqBuf[i] = ((short*)capBuf)[(i + capOffset) & SAMPLE_MASK];
        }
    }

    AudioUtils::FFT(freqBuf, SAMPLE_SIZE_BIT, false);

    if (displayWave){
        glDisable(GL_LINE_SMOOTH);
        glLineWidth(1.0f);
        glBegin(GL_LINE_STRIP);
        glColor3f(1.0f, 1.0f, 1.0f);
        if (adjustWave){
            for (int i = 0; i < 1024; i++){
                float rate = i / 1024.0f;
                float amp = Clamp(((short*)capBuf)[((i << (SAMPLE_SIZE_BIT - 10)) + capOffset) & SAMPLE_MASK] * 0.000030517578125f * ratio, -1.0f, 1.0f);
                glVertex2f(rate * 2.0f - 1.0f, (amp + 1.0f) * 0.5f);
            }
        }else{
            for (int i = 0; i < 1024; i++){
                float rate = i / 1024.0f;
                float amp = Clamp(((short*)capBuf)[((i << (SAMPLE_SIZE_BIT - 10)) + capOffset) & SAMPLE_MASK] * 0.000030517578125f, -1.0f, 1.0f);
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
            float amp = Clamp(Log(freqBuf[i << (SAMPLE_SIZE_BIT - 10)].SqrMagnitude()) * 0.1f, 2.0f / cliSize.y, 1.0f);
            glColor3f(rate, 1.0f - rate, 0.0f);
            glVertex2f(rate * 2.0f - 1.0f, (-amp + 1.0f) * 0.5f);
            glVertex2f(rate * 2.0f - 1.0f, (amp + 1.0f) * 0.5f);
        }
        glEnd();
    }
}

void AudioCaptureWindow::ProcessOutput(){
    ALint cnt;
    ALint offset = (recOffset & SAMPLE_MASK);

    cnt = Min(soundTouch->numSamples(), SAMPLE_SIZE - offset);

    soundTouch->receiveSamples((soundtouch::SAMPLETYPE*)recBuf + offset, cnt);
    if (cnt == (SAMPLE_SIZE) - offset)
        UpdateBuffer(recBuf, 1 << (SAMPLE_SIZE_BIT + 1));
    
    recOffset += cnt;

    for (int i = 0; i < (SAMPLE_SIZE); i++)
        freqBuf[i] = ((short*)recBuf)[(i + recOffset) & SAMPLE_MASK];
    AudioUtils::FFT(freqBuf, SAMPLE_SIZE_BIT, false);

    if (displayWave){
        glDisable(GL_LINE_SMOOTH);
        glLineWidth(1.0f);
        glBegin(GL_LINE_STRIP);
        glColor3f(1.0f, 1.0f, 1.0f);
        if (adjustWave){
            for (int i = 0; i < 1024; i++){
                float rate = i / 1024.0f;
                float amp = Clamp(((short*)recBuf)[((i << (SAMPLE_SIZE_BIT - 10)) + recOffset) & SAMPLE_MASK] * 0.000030517578125f * ratio, -1.0f, 1.0f);
                glVertex2f(rate * 2.0f - 1.0f, (amp - 1.0f) * 0.5f);
            }
        }else{
            for (int i = 0; i < 1024; i++){
                float rate = i / 1024.0f;
                float amp = Clamp(((short*)recBuf)[((i << (SAMPLE_SIZE_BIT - 10)) + recOffset) & SAMPLE_MASK] * 0.000030517578125f, -1.0f, 1.0f);
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
            float amp = Clamp(Log(freqBuf[i << (SAMPLE_SIZE_BIT - 10)].SqrMagnitude()) * 0.1f, 2.0f / cliSize.y, 1.0f);
            glColor3f(rate, 1.0f - rate, 0.0f);
            glVertex2f(rate * 2.0f - 1.0f, (-amp - 1.0f) * 0.5f);
            glVertex2f(rate * 2.0f - 1.0f, (amp - 1.0f) * 0.5f);
        }
        glEnd();
    }
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

    guiMgr->OnChainRender();
}

void AudioCaptureWindow::OnChar(char c){
    guiMgr->OnChar(c);
}

void AudioCaptureWindow::OnUnichar(wchar_t c){
    guiMgr->OnUnichar(c);
}

void AudioCaptureWindow::OnResize(int x, int y){
    UpdateWindowSize(x, y);
}

void AudioCaptureWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnMouseMove2D(cursorPos);
}

void AudioCaptureWindow::OnLeftDown(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnLeftDown2D(cursorPos);
}

void AudioCaptureWindow::OnLeftUp(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnLeftUp2D(cursorPos);
}

void AudioCaptureWindow::OnRightDown(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnRightDown2D(cursorPos);
    Main::SetMenu(basicMenu);
}

void AudioCaptureWindow::OnRightUp(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnRightUp2D(cursorPos);
}

void AudioCaptureWindow::OnMouseWheel(int delta){}

void AudioCaptureWindow::OnMenuAccel(int id, bool accel){
    switch (id){
    case IDM_MENU_BASIC:
        Main::SetMenu(basicMenu);
        break;
    }
}

void AudioCaptureWindow::OnDropFileA(const char* path, uint len){}

void AudioCaptureWindow::OnDropFileW(const wchar_t* path, uint len){}

void AudioCaptureWindow::Serialize(IOutputStream& os){
    os.WriteWithLen(WINDOW_ID);
}

void AudioCaptureWindow::Deserialize(IInputStream& os){}

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

    //DebugLog("AudioCaptureWindow %p Queue Buffer %d %d", this, head, alBuf[head & QUEUE_MASK]);
    alBufferData(alBuf[head & QUEUE_MASK], AL_FORMAT_MONO16, buf, size, FREQUENCY);
    alSourceQueueBuffers(alSrc, 1, &alBuf[(head++) & QUEUE_MASK]);

    alGetSourcei(alSrc, AL_BUFFERS_PROCESSED, &cnt);
    while (cnt--){
        //DebugLog("AudioCaptureWindow %p Unqueue Buffer %d %d", this, tail, alBuf[tail & QUEUE_MASK]);
        alSourceUnqueueBuffers(alSrc, 1, &alBuf[(tail++) & QUEUE_MASK]);
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

    capDev = alcCaptureOpenDevice(name, FREQUENCY, AL_FORMAT_MONO16, 1 << (SAMPLE_SIZE_BIT + QUEUE_SIZE_BIT));

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

}