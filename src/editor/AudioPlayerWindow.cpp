#include <editor/AudioPlayerWindow.h>

#include <lib/openal/al.h>

#include <main.h>
#include <res.h>
#include <util/os/Font.h>
#include <util/os/Shell.h>
#include <util/os/Resource.h>
#include <util/String.h>
#include <io/File.h>
#include <util/gl/GLUtils.h>
#include <util/gl/GLSimplified.h>
#include <util/math3d/Math.h>
#include <util/math3d/LinearAlgebra.h>
#include <util/AudioUtils.h>
#include <util/String.h>
#include <util/StringBuilder.h>
#include <editor/main/ViewObject.h>
#include <editor/gui/Menu.h>
#include <editor/gui/GUIUtils.h>
#include <editor/gui/Menu.h>
#include <editor/object/AudioSourceObject.h>
#include <editor/object/GUIManagerObject.h>

#include <lib/json/nlohmann/json.hpp>

namespace simple3deditor {

WINDOW_INFO_DECL(simple3deditor::AudioPlayerWindow, L"音频播放器");

AudioPlayerWindow::AudioPlayerWindow() : CAudioSourceLoader(source) {
    DebugLog("AudioPlayerWindow Launched");

    guiMgr = new GUIManagerObject();

    basicMenu = new Menu();
    basicMenu->AddItem(new MenuItem(L"加载", [=]{ this->OnMenuAccel(IDM_LOAD, false); }));
    basicMenu->AddItem(new MenuItem(L"加载当前对象", [=]{
        if (Main::data->curObject && Main::data->curObject->GetType() != ViewObjectType::OBJECT_AUDIO_SOURCE){
            DebugError("AudioPlayerWindow::LoadObject Object Type Error");
            return;
        }
        this->AudioSourceLoad((AudioSourceObject*)Main::data->curObject);
    }));
    basicMenu->AddItem(new MenuItem(
        Value<const wchar_t*>([=]{ return this->IsLoop() ? L"循环:开" : L"循环:关"; }),
        [=]{
            bool loop = this->IsLoop();
            loop = !loop;
            this->SetLoop(loop);
            DebugLog("AudioPlayerWindow::LoopItem State %s", loop ? "Looping" : "Default");
        }
    ));
    basicMenu->AddItem(new MenuItem(
        Value<const wchar_t*>([=]{ return this->displayWave ? L"显示模式:波形" : L"显示模式:频谱"; }),
        [=]{
            this->displayWave = !this->displayWave;
            DebugLog("AudioPlayerWindow::DisplayModeItem State %s", this->displayWave ? "Wave" : "Frequency");
        }
    ));
    basicMenu->AddItem(new MenuItem(
        Value<const wchar_t*>([=]{ return this->HasDopplerEffect() ? L"多普勒效应:开" : L"多普勒效应:关"; }),
        [=]{
            bool state = this->HasDopplerEffect();
            state = !state;
            this->SetDopplerEffect(state);
            DebugLog("AudioPlayerWindow::DopplerEffectItem State %s", state ? "On" : "Off");
        }
    ));

    class PlayButton final : public IconButton {
    public:
        PlayButton() : IconButton(Vector2(-0.5f, -0.9f), Vector2(1.0f, 0.6f), 0.2f) {
            hoverColor = Vector3(1.0f, 0.5f, 0.0f);
            defaultColor = Vector3(0.0f, 0.4f, 1.0f);
        }

        virtual void OnRender() override {
            IconButton::OnRender();

            glColor3f(1.0f, 1.0f, 1.0f);
            glBegin(GL_TRIANGLES);
            glVertex2f(-0.15f, -0.8f);
            glVertex2f(-0.15f, -0.4f);
            glVertex2f(0.25f, -0.6f);
            glEnd();
        }
    } *playBtn = new PlayButton();
    playBtn->onClick += [=]{
        if (this->IsLaunched()){
            this->Stop();
        }else{
            this->Launch();
        }
    };
    guiMgr->AddChild(playBtn);

    class ProgressBar : public HorizontalProgressBar {
    public:
        AudioPlayerWindow* window;

        ProgressBar(AudioPlayerWindow* window) : HorizontalProgressBar(), window(window) {
            lowBound = -0.6f;
            highBound = 0.6f;
            posY = -0.1f;
            btnX = 0.05f;
            btnY = 0.1f;
            lineWidth = 10.0f;
        }

        virtual void OnRender() override {
            pos = window->GetOffsetRate();
            HorizontalProgressBar::OnRender();
        }
    } *progBar = new ProgressBar(this);
    progBar->onPosChange += [=](float pos){
        this->SetOffsetRate(pos);
    };
    guiMgr->AddChild(progBar);

    class GainBar : public VerticalProgressBar {
    public:
        GainBar() : VerticalProgressBar() {
            lowBound = -0.8f;
            highBound = -0.2f;
            posX = -0.85f;
            btnX = 0.05f;
            btnY = 0.02f;
            lineWidth = 5.0f;
            pos = 1.0f;
        }
    } *gainBar = new GainBar();
    gainBar->onPosChange += [=](float pos){
        this->SetGain(pos);
        // 这个是大致按照分贝数调节音量的
        // this->SetGain(GetRate(Exp(-5.0f * pos), 0.0067379469f /* e^-5 */, 1.0f));
    };
    guiMgr->AddChild(gainBar);

    class LoopButton : public IconButton {
    public:
        AudioPlayerWindow* window;
        bool state = false;

        LoopButton(AudioPlayerWindow* window) : IconButton(Vector2(0.7f, -0.2f), Vector2(0.2f, 0.2f), 0.04f), window(window) {
            hoverColor = Vector3(0.0f, 0.0f, 0.2f);
            defaultColor = Vector3(0.0f, 0.0f, 0.0f);
            state = window->IsLoop();
        }

        virtual void OnRender() override {
            if (state != window->IsLoop()){
                state = window->IsLoop();
                defaultColor = state ? Vector3(0.0f, 0.0f, 1.0f) : Vector3(0.0f, 0.0f, 0.0f);
                hoverColor = state ? Vector3(0.0f, 0.0f, 0.8f) : Vector3(0.0f, 0.0f, 0.2f);
            }
            IconButton::OnRender();
        }
    } *loopBtn = new LoopButton(this);
    loopBtn->onClick += [=]{
        bool loop = this->IsLoop();
        loop = !loop;
        this->SetLoop(loop);
        DebugLog("AudioPlayerWindow::LoopOption State %s", loop ? "Looping" : "Default");
    };
    guiMgr->AddChild(loopBtn);
}

AudioPlayerWindow::~AudioPlayerWindow(){
    DebugLog("AudioPlayerWindow Destroyed");
    if (guiMgr) delete guiMgr;
}

void AudioPlayerWindow::DrawLineGraph(float* height, size_t size){
    glDisable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < size; i++){
        float rate = i / (float)size;
        glVertex2f(rate * 2.0f - 1.0f, height[i]);
    }
    glEnd();
}

void AudioPlayerWindow::DrawAmplitudeGraph(float* height, size_t size){
    glDisable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < size; i++){
        float rate = i / (float)size;
        if (height[i] > 0.0f){
            glColor3f(rate, 1.0f - rate, 0.0f);
            glVertex2f(rate * 2.0f - 1.0f, 0.0f);
            glVertex2f(rate * 2.0f - 1.0f, height[i]);
        }
    }
    glEnd();
}

void AudioPlayerWindow::RenderGraph(){
    ALint offset = source->GetOffset();
    ALint sampleSize = source->GetSampleSize();
    ALint channels = source->GetChannelCount();
    char* data = source->GetData();

    Complex samples[SAMPLE_SIZE];
    float height[GRAPH_SIZE];

    if (offset + SAMPLE_SIZE >= source->GetSize()){
        return;
    }

    if (sampleSize == 4 && channels == 2){
        for (int i = 0; i < SAMPLE_SIZE; i++){
            samples[i] = Complex(((short*)data)[(i + offset) << 1]);
        }
    }else if (sampleSize == 2 && channels == 1){
        for (int i = 0; i < SAMPLE_SIZE; i++){
            samples[i] = Complex(((short*)data)[i + offset]);
        }
    }else if (sampleSize == 2 && channels == 2){
        for (int i = 0; i < SAMPLE_SIZE; i++){
            samples[i] = Complex(((short)(((char*)data)[(i + offset) << 1] - 0x80) << 8));
        }
    }else if (sampleSize == 1 && channels == 1){
        for (int i = 0; i < SAMPLE_SIZE; i++){
            samples[i] = Complex(((short)(((char*)data)[i + offset] - 0x80) << 8));
        }
    }

    if (displayWave){
        for (int i = 0; i < GRAPH_SIZE; i++){
            height[i] = samples[i << (SAMPLE_SIZE_BIT - 10)].real * 0.0000152587890625f + 0.5f;
        }
        DrawLineGraph(height, GRAPH_SIZE);
    }else{
        AudioUtils::FFT(samples, SAMPLE_SIZE_BIT, false);
        for (int i = 0; i < GRAPH_SIZE; i++){
            height[i] = Log(samples[i << (SAMPLE_SIZE_BIT - 10)].SqrMagnitude()) * 0.1f;
        }
        DrawAmplitudeGraph(height, GRAPH_SIZE);
    }
}

void AudioPlayerWindow::DrawTime(){
    int total = source->GetLength();
    int offset = source->GetOffset() / source->GetFrequency();
    char time[20];

    __builtin_snprintf(time, 20, "%02d:%02d/%02d:%02d", offset / 60, offset % 60, total / 60, total % 60);
    
    glColor3f(1.0f, 1.0f, 0.0f);
    glRasterPos2f(0.6f, -0.3f);
    glDrawString(time);
}

void AudioPlayerWindow::OnRender(){
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GLUtils::ResetProjection();
    GLUtils::ResetModelView();

    if (!source){
        glColor3f(1.0f, 0.5f, 0.0f);
        glRasterPos2f(-1.0f, 1.0f - 30.0f * cliInvSize.y);
        glDrawCNString(L"尚未存在音源");
        glRasterPos2f(-1.0f, 1.0f - 60.0f * cliInvSize.y);
        glDrawCNString(L"可以将文件拖拽至此处");
        return;
    }

    RenderGraph();
    DrawTime();

    guiMgr->OnChainRender();
}

void AudioPlayerWindow::OnChar(char c){
    guiMgr->OnChar(c);
}

void AudioPlayerWindow::OnUnichar(wchar_t c){
    guiMgr->OnUnichar(c);
}

void AudioPlayerWindow::OnResize(int x, int y){
    AWindow::OnResize(x, y);
}

void AudioPlayerWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnMouseMove2D(cursorPos);
}

void AudioPlayerWindow::OnLeftDown(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnLeftDown2D(cursorPos);
}

void AudioPlayerWindow::OnLeftUp(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnLeftUp2D(cursorPos);
}

void AudioPlayerWindow::OnRightDown(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnRightDown2D(cursorPos);
    Main::SetMenu(basicMenu);
}

void AudioPlayerWindow::OnRightUp(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnRightUp2D(cursorPos);
}

void AudioPlayerWindow::OnMouseWheel(int delta){}

void AudioPlayerWindow::OnMenuAccel(int id, bool accel){
    DebugLog("AudioPlayerWindow::OnMenuAccel %d %s", id, accel ? "true" : "false");
    switch (id){
    case IDM_LOAD:
        OnInsLoad();
        break;
    case IDM_MENU_BASIC:
        Main::SetMenu(basicMenu);
        break;
    }
}

void AudioPlayerWindow::OnDropFileA(const char* path, uint len){}

void AudioPlayerWindow::OnDropFileW(const wchar_t* path, uint len){
    String file(path, len);
    DebugLog("AudioPlayerWindow::OnDropFileW %s", file.GetString());
    PreloadFile(file);
}

void AudioPlayerWindow::Serialize(nlohmann::json& o){
    o["id"] = WINDOW_ID;
}

void AudioPlayerWindow::Deserialize(nlohmann::json& o){}

void AudioPlayerWindow::OnInsLoad(){
    // 暂不使用 L"PCM音频文件(*.wav)\0*.wav\0所有音频类型(.*)\0*.*\0"，此状态下发现Shell时可能的环境错误
    // 若要使用请拖入文件
    static const WString filter = Resource::GetWString(IDS_WAVFILE_FILTER);
    String file = ShellFileSelectWindow(filter, FILESELECT_REQ_PATH | FILESELECT_REQ_FILE);
    if (file.GetLength() == 0){
        DebugLog("Stop Loading");
        return;
    }
    PreloadFile(file);
}

void AudioPlayerWindow::PreloadFile(String& file){
    if (!file.EndsWith(L".wav")){
        static const WString message = Resource::GetWString(IDS_WAVFILE_FORM_WARNING);
        static const WString caption = Resource::GetWString(IDS_WAVFILE_FORM_WARNING_CAPTION);
        switch (ShellMsgBox(caption, message)){
        case MSGBOX_NO:
            DebugLog("AudioPlayerWindow::PreloadFile Stop Load File");
            return;
        case MSGBOX_YES:
            DebugLog("AudioPlayerWindow::PreloadFile Preparing FFmpeg");
            DebugLog("AudioPlayerWindow::PreloadFile Source Audio File %s", file.GetString());
            if (!ShellFFmpeg(file, "temp.wav")){
                DebugError("AudioPlayerWindow::PreloadFile ShellFFmpegW Failed");
                return;
            }
            LoadFile("temp.wav");
            File::Delete("temp.wav");
            return;
        case MSGBOX_CANCEL:
            break;
        }
    }
    LoadFile(file);
}

void AudioPlayerWindow::LoadFile(WString file){
    if (file.GetLength() == 0){
        return;
    }

    DebugLog("AudioPlayerWindow::LoadFile From %S", file.GetString());

    File src(file);
    if (!src.Open()){
        DebugError("AudioPlayerWindow::LoadFile File Open Failed");
        return;
    }

    // 解析PCM数据
    int specNum;
    int readLen;
    int dataLen;
    AudioWaveFormat wav;
    bool fmtRead = false;

    src.Read(&specNum, 4);
    if (specNum != *(int*)"RIFF"){
        src.Close();
        DebugError("AudioPlayerWindow::LoadFile File Magic Number 'RIFF' Not Found");
        return;
    }
    src.Read(&specNum, 4);
    src.Read(&specNum, 4);
    if (specNum != *(int*)"WAVE"){
        src.Close();
        DebugError("AudioPlayerWindow::LoadFile File Magic Number 'WAVE' Not Found");
        return;
    }
    readLen = src.Read(&specNum, 4);
    while (true){
        if (readLen != 4){
            DebugError("AudioPlayerWindow::LoadFile File Data Block Not Found");
            src.Close();
            return;
        }
        if (specNum == *(int*)"fmt "){
            readLen = src.Read(&specNum, 4);
            if (specNum != 0x10){
                src.Close();
                DebugError("AudioPlayerWindow::LoadFile File Format Length Is Not 0x10 [%d]", specNum);
                return;
            }
            src.Read(&wav, sizeof(wav));
            wav.wBitsPerSample = wav.nBlockAlign << 3;
            fmtRead = true;
            readLen = src.Read(&specNum, 4);
            continue;
        }else if (specNum == *(int*)"data"){
            src.Read(&dataLen, 4);
            break;
        }
        src.Read(&specNum, 4);
        src.SetPointer(src.GetPointer() + specNum);
        readLen = src.Read(&specNum, 4);
    }
    if (!fmtRead){
        src.Close();
        DebugError("AudioPlayerWindow::LoadFile File Format Not Found");
        return;
    }
    ALint format = GetWaveFormat(&wav);
    if (format == -1){
        src.Close();
        DebugError("AudioPlayerWindow::LoadFile File Format Unrecognized");
        return;
    }

    char* fileData = new char[dataLen];
    src.Read(fileData, dataLen);

    // 转为单声道(后续可加入用户设置)
    if (format == AL_FORMAT_STEREO8){
        char* oldData = fileData;
        char* newData;
        dataLen >>= 1;
        newData = new char[dataLen];
        for (int i = 0; i < dataLen; i++)
            newData[i] = oldData[i << 1];
        delete[] oldData;
        fileData = newData;
        format = AL_FORMAT_MONO8;
        wav.nChannels = 1;
        wav.nBlockAlign = 1;
    }else if (format == AL_FORMAT_STEREO16){
        char* oldData = fileData;
        char* newData;
        dataLen >>= 1;
        newData = new char[dataLen];
        for (int i = 0; i < (dataLen >> 1); i++)
            ((short*)newData)[i] = ((short*)oldData)[i << 1];
        delete[] oldData;
        fileData = newData;
        format = AL_FORMAT_MONO16;
        wav.nChannels = 1;
        wav.nBlockAlign = 2;
    }

    // 频率 8000 11025 16000 22050 44100 48000 96000 192000
    AudioSourceObject* audioSrc = new AudioSourceObject(format, fileData, dataLen, wav.nSamplesPerSec);
    Main::AddObject(audioSrc);
    Main::SelectObject(audioSrc);
    AudioSourceLoad(audioSrc);
    
    src.Close();

    DebugLog("AudioPlayerWindow::LoadFile Success");
}

bool AudioPlayerWindow::IsLoaded(){
    return source;
}

void AudioPlayerWindow::Launch(){
    if (!source){
        DebugError("AudioPlayerWindow::Launch Is Not Loaded");
        return;
    }

    source->Play();

    DebugLog("AudioPlayerWindow::Launch Success");
}

void AudioPlayerWindow::Stop(){
    if (!source)
        return;
    source->Stop();
}

bool AudioPlayerWindow::IsLaunched(){
    if (!source)
        return false;
    return source->IsPlaying();
}

bool AudioPlayerWindow::IsLoop(){
    if (!source)
        return false;
    return source->IsLoop();
}

void AudioPlayerWindow::SetLoop(bool loop){
    if (!source)
        return;
    return source->SetLoop(loop);
}

ALint AudioPlayerWindow::GetOffset(){
    if (!source)
        return 0;
    return source->GetOffset();
}

float AudioPlayerWindow::GetOffsetRate(){
    if (!source)
        return 0.0f;
    return (float)source->GetOffset() / source->GetSize();
}

void AudioPlayerWindow::SetOffsetRate(float rate){
    ALint size;

    if (!source)
        return;
    size = source->GetSize();
    source->SetOffset(Clamp((int)Round(rate * size), 0, size));
}

void AudioPlayerWindow::SetGain(float gain){
    if (!source)
        return;
    source->SetGain(gain);
}

bool AudioPlayerWindow::HasDopplerEffect(){
    if (!source)
        return false;
    return source->HasDopplerEffect();
}

void AudioPlayerWindow::SetDopplerEffect(bool on){
    if (!source)
        return;
    source->SetDopplerEffect(on);
}

ALint AudioPlayerWindow::GetWaveFormat(AudioWaveFormat* wav){
    ALenum format = AL_NONE;
    switch (wav->nChannels){
    case 1:
        switch (wav->nBlockAlign){
        case 1:
            format = AL_FORMAT_MONO8;
            break;
        case 2:
            format = AL_FORMAT_MONO16;
            break;
        }
        break;
    case 2:
        switch (wav->nBlockAlign){
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

}