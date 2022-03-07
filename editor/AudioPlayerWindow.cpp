#include <editor/AudioPlayerWindow.h>

#include <lib/openal/al.h>

#include <main.h>
#include <res.h>
#include <utils/os/Font.h>
#include <utils/os/Shell.h>
#include <utils/String.h>
#include <utils/File.h>
#include <utils/gl/GLUtils.h>
#include <utils/gl/GLSimplified.h>
#include <utils/math3d/Math.h>
#include <utils/math3d/LinearAlgebra.h>
#include <utils/math3d/ViewObject.h>
#include <utils/AudioUtils.h>
#include <utils/String.h>
#include <utils/StringBuilder.h>

const float AudioPlayerWindow::PlayButton::BOUND_LEFT = -0.5f;
const float AudioPlayerWindow::PlayButton::BOUND_RIGHT = 0.5f;
const float AudioPlayerWindow::PlayButton::BOUND_TOP = -0.3f;
const float AudioPlayerWindow::PlayButton::BOUND_BOTTOM = -0.9f;
const float AudioPlayerWindow::PlayButton::RADIUS = 0.2f;
const Vector3 AudioPlayerWindow::PlayButton::HOVER_COLOR = Vector3(1.0f, 0.5f, 0.0f);
const Vector3 AudioPlayerWindow::PlayButton::LEAVE_COLOR = Vector3(0.0f, 0.4f, 1.0f);

AudioPlayerWindow::PlayButton::PlayButton(AudioPlayerWindow* window) : window(window) {}
AudioPlayerWindow::PlayButton::~PlayButton(){}

bool AudioPlayerWindow::PlayButton::Trigger(Vector2 pos){
    return pos.x >= BOUND_LEFT && pos.x <= BOUND_RIGHT && pos.y >= BOUND_BOTTOM && pos.y <= BOUND_TOP;
}

void AudioPlayerWindow::PlayButton::Hover(Vector2 pos){
    hover = true;
}

void AudioPlayerWindow::PlayButton::Click(Vector2 pos){
    if (window->IsLaunched()){
        window->Stop();
    }else{
        window->Launch();
    }
}

void AudioPlayerWindow::PlayButton::Leave(Vector2 pos){
    hover = false;
}

void AudioPlayerWindow::PlayButton::Render(){
    if (hover){
        glColorv3(HOVER_COLOR);
    }else{
        glColorv3(LEAVE_COLOR);
    }
    GLUtils::DrawRoundRect(-0.5f, -0.9f, 1.0f, 0.6f, 0.2f, 0.05f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.15f, -0.8f);
    glVertex2f(-0.15f, -0.4f);
    glVertex2f(0.25f, -0.6f);
    glEnd();
}

const float AudioPlayerWindow::ProgressBar::LOW_BOUND = -0.6f;
const float AudioPlayerWindow::ProgressBar::HIGH_BOUND = 0.6f;
const float AudioPlayerWindow::ProgressBar::POSITION_Y = -0.1f;
const float AudioPlayerWindow::ProgressBar::BUTTON_WIDTH_X = 0.05f;
const float AudioPlayerWindow::ProgressBar::BUTTON_WIDTH_Y = 0.1f;

AudioPlayerWindow::ProgressBar::ProgressBar(AudioPlayerWindow* window) : window(window) {}
AudioPlayerWindow::ProgressBar::~ProgressBar(){}

bool AudioPlayerWindow::ProgressBar::Trigger(Vector2 pos){
    return pos.x >= this->pos - BUTTON_WIDTH_X &&
            pos.x <= this->pos + BUTTON_WIDTH_X &&
            pos.y >= POSITION_Y - BUTTON_WIDTH_Y &&
            pos.y <= POSITION_Y + BUTTON_WIDTH_Y;
}

void AudioPlayerWindow::ProgressBar::Click(Vector2 pos){
    origin = this->pos;
}

void AudioPlayerWindow::ProgressBar::Drag(Vector2 dir){
    ALint size;

    pos = Clamp(origin + dir.x, LOW_BOUND, HIGH_BOUND);

    if (!window->source)
        return;
    
    size = window->source->GetSize();
    window->source->SetOffset(Clamp((int)(GetRate(pos, LOW_BOUND, HIGH_BOUND) * size), 0, size));
}

void AudioPlayerWindow::ProgressBar::Hover(Vector2 pos){
    hover = true;
}

void AudioPlayerWindow::ProgressBar::Leave(Vector2 pos){
    hover = false;
}

void AudioPlayerWindow::ProgressBar::Render(){
    // 因为Render调用之前source不能为NULL，所以应不需检查
    ALint offset = window->source->GetOffset();
    ALint size = window->source->GetSize();

    glLineWidth(10.0f);
    glColor3f(0.6f, 0.6f, 0.6f);
    glBegin(GL_LINES);
    glVertex2f(LOW_BOUND, POSITION_Y);
    glVertex2f(HIGH_BOUND, POSITION_Y);
    glEnd();
    glLineWidth(1.0f);

    pos = Lerp(LOW_BOUND, HIGH_BOUND, (float)offset / size);
    if (hover){
        glColor3f(0.0f, 0.0f, 0.3f);
    }else{
        glColor3f(0.0f, 0.0f, 0.5f);
    }
    GLUtils::DrawRect(pos - BUTTON_WIDTH_X, POSITION_Y - BUTTON_WIDTH_Y,
                        pos + BUTTON_WIDTH_X, POSITION_Y + BUTTON_WIDTH_Y);
}

const float AudioPlayerWindow::GainBar::LOW_BOUND = -0.8f;
const float AudioPlayerWindow::GainBar::HIGH_BOUND = -0.2f;
const float AudioPlayerWindow::GainBar::POSITION_X = -0.85f;
const float AudioPlayerWindow::GainBar::BUTTON_WIDTH_X = 0.05f;
const float AudioPlayerWindow::GainBar::BUTTON_WIDTH_Y = 0.02f;

AudioPlayerWindow::GainBar::GainBar(AudioPlayerWindow* window) : window(window) {}
AudioPlayerWindow::GainBar::~GainBar(){}

bool AudioPlayerWindow::GainBar::Trigger(Vector2 pos){
    return pos.x >= (POSITION_X - BUTTON_WIDTH_X) &&
            pos.x <= (POSITION_X + BUTTON_WIDTH_X) &&
            pos.y >= this->pos - BUTTON_WIDTH_Y && pos.y <= this->pos + BUTTON_WIDTH_Y;
}

void AudioPlayerWindow::GainBar::Click(Vector2 pos){
    origin = this->pos;
}

void AudioPlayerWindow::GainBar::Drag(Vector2 dir){
    pos = Clamp(origin + dir.y, LOW_BOUND, HIGH_BOUND);
    if (window->source)
        window->source->SetGain(GetRate(Exp(-5.0f * GetRate(pos, HIGH_BOUND, LOW_BOUND)), 0.0067379469f /* e^-5 */, 1.0f));
}

void AudioPlayerWindow::GainBar::Hover(Vector2 pos){
    hover = true;
}

void AudioPlayerWindow::GainBar::Leave(Vector2 pos){
    hover = false;
}

void AudioPlayerWindow::GainBar::Render(){
    glLineWidth(5.0f);
    glColor3f(0.6f, 0.6f, 0.6f);
    glBegin(GL_LINES);
    glVertex2f(POSITION_X, LOW_BOUND);
    glVertex2f(POSITION_X, HIGH_BOUND);
    glEnd();
    glLineWidth(1.0f);

    if (hover){
        glColor3f(0.0f, 0.0f, 0.3f);
    }else{
        glColor3f(0.0f, 0.0f, 0.5f);
    }
    GLUtils::DrawRect(POSITION_X - BUTTON_WIDTH_X, pos - BUTTON_WIDTH_Y,
                        POSITION_X + BUTTON_WIDTH_X, pos + BUTTON_WIDTH_Y);
}

AudioPlayerWindow::LoopOption::LoopOption(AudioPlayerWindow* window) : window(window) {}
AudioPlayerWindow::LoopOption::~LoopOption(){}

bool AudioPlayerWindow::LoopOption::Trigger(Vector2 pos){
    return pos.x >= 0.7f && pos.x <= 0.9f && pos.y >= -0.2f && pos.y <= 0.0f;
}

void AudioPlayerWindow::LoopOption::Click(Vector2 pos){
    loop = window->IsLoop();
    loop = !loop;
    window->SetLoop(loop);
    DebugLog("AudioPlayerWindow::LoopOption State %s", loop ? "Looping" : "Default");
}

void AudioPlayerWindow::LoopOption::Render(){
    loop = window->IsLoop();
    if (loop){
        glColor3f(0.0f, 0.0f, 1.0f);
    }else{
        glColor3f(0.0f, 0.0f, 0.0f);
    }
    GLUtils::DrawRect(0.7f, -0.2f, 0.9f, 0.0f);
}

AudioPlayerWindow::LoopItem::LoopItem(AudioPlayerWindow* window) : window(window) {}
AudioPlayerWindow::LoopItem::~LoopItem(){}

const wchar_t* AudioPlayerWindow::LoopItem::GetName(){
    return window->IsLoop() ? L"循环:开" : L"循环:关";
}

void AudioPlayerWindow::LoopItem::OnClick(){
    bool loop = window->IsLoop();
    loop = !loop;
    window->SetLoop(loop);
    DebugLog("AudioPlayerWindow::LoopItem State %s", loop ? "Looping" : "Default");
}

AudioPlayerWindow::DopplerEffectItem::DopplerEffectItem(AudioPlayerWindow* window) : window(window) {}
AudioPlayerWindow::DopplerEffectItem::~DopplerEffectItem(){}

const wchar_t* AudioPlayerWindow::DopplerEffectItem::GetName(){
    return window->HasDopplerEffect() ? L"多普勒效应:开" : L"多普勒效应:关";
}

void AudioPlayerWindow::DopplerEffectItem::OnClick(){
    bool state = window->HasDopplerEffect();
    state = !state;
    window->SetDopplerEffect(state);
    DebugLog("AudioPlayerWindow::DopplerEffectItem State %s", state ? "On" : "Off");
}

AudioPlayerWindow::AudioPlayerWindow(){
    DebugLog("AudioPlayerWindow Launched");

    uiMgr = new UIManager();

    uiMgr->AddButton(new PlayButton(this));
    uiMgr->AddButton(new ProgressBar(this));
    uiMgr->AddButton(new GainBar(this));
    uiMgr->AddButton(new LoopOption(this));

    basicMenu = new Menu();
    basicMenu->AddItem(new MenuItem(L"加载", MENUITEM_LAMBDA_TRANS(AudioPlayerWindow)[](AudioPlayerWindow* window){
        window->OnMenuAccel(IDM_LOAD, false);
    }, this));
    basicMenu->AddItem(new MenuItem(L"加载当前对象", MENUITEM_LAMBDA_TRANS(AudioPlayerWindow)[](AudioPlayerWindow* window){
        if (Main::data->curObject && Main::data->curObject->GetType() != ViewObjectType::OBJECT_AUDIO_SOURCE){
            DebugError("AudioPlayerWindow::LoadObject Object Type Error");
            return;
        }
        window->LoadObject((AudioSourceObject*)Main::data->curObject);
    }, this));
    basicMenu->AddItem(new LoopItem(this));
    basicMenu->AddItem(new SwitchMenuItem(L"显示模式:波形", L"显示模式:频谱", SWITCHMENUITEM_LAMBDA_TRANS(AudioPlayerWindow)[](bool state, AudioPlayerWindow* window){
        window->displayWave = state;
        DebugLog("AudioPlayerWindow::DisplayModeItem State %s", window->displayWave ? "Wave" : "Frequency");
    }, this, displayWave));
    basicMenu->AddItem(new DopplerEffectItem(this));
}

AudioPlayerWindow::~AudioPlayerWindow(){
    DebugLog("AudioPlayerWindow Destroyed");
    LoadObject(NULL);
    if (uiMgr) delete uiMgr;
}

bool AudioPlayerWindow::IsFocus(){
    return focus;
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

    Complex samples[1 << bit];
    float height[1 << 10];

    if (offset + (1 << bit) >= source->GetSize()){
        return;
    }

    if (sampleSize == 4 && channels == 2){
        for (int i = 0; i < (1 << bit); i++){
            samples[i] = Complex(((short*)data)[(i + offset) << 1]);
        }
    }else if (sampleSize == 2 && channels == 1){
        for (int i = 0; i < (1 << bit); i++){
            samples[i] = Complex(((short*)data)[i + offset]);
        }
    }else if (sampleSize == 2 && channels == 2){
        for (int i = 0; i < (1 << bit); i++){
            samples[i] = Complex(((short)(((char*)data)[(i + offset) << 1] - 0x80) << 8));
        }
    }else if (sampleSize == 1 && channels == 1){
        for (int i = 0; i < (1 << bit); i++){
            samples[i] = Complex(((short)(((char*)data)[i + offset] - 0x80) << 8));
        }
    }

    if (displayWave){
        for (int i = 0; i < (1 << 10); i++){
            height[i] = samples[i << (bit - 10)].real * 0.0000152587890625f + 0.5f;
        }
        DrawLineGraph(height, 1 << 10);
    }else{
        AudioUtils::FFT(samples, bit, false);
        for (int i = 0; i < (1 << 10); i++){
            height[i] = Log(samples[i << (bit - 10)].SqrMagnitude()) * 0.1f;
        }
        DrawAmplitudeGraph(height, 1 << 10);
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
        glRasterPos2f(-1.0f, 1.0f - 30.0f / size.y);
        glDrawCNString(L"尚未存在音源");
        glRasterPos2f(-1.0f, 1.0f - 60.0f / size.y);
        glDrawCNString(L"可以将文件拖拽至此处");
        return;
    }

    RenderGraph();
    DrawTime();

    uiMgr->Render();
}

void AudioPlayerWindow::OnClose(){}

void AudioPlayerWindow::OnTimer(int id){
    if (IsLaunched()){
        Main::RequestRender();
    }
}

void AudioPlayerWindow::OnChar(char c){}

void AudioPlayerWindow::OnUnichar(wchar_t c){}

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
    Main::SetMenu(basicMenu);
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

void AudioPlayerWindow::OnDropFileA(const char* path){}

void AudioPlayerWindow::OnDropFileW(const wchar_t* path){
    DebugLog("AudioPlayerWindow::OnDropFileW %S", path);
    PreloadFile(WString(path));
}

void AudioPlayerWindow::OnInsLoad(){
    // 暂不使用 L"PCM音频文件(*.wav)\0*.wav\0所有音频类型(.*)\0*.*\0"，此状态下发现Shell时可能的环境错误
    // 若要使用请拖入文件
    WString file = ShellFileSelectWindow(WString(IDS_WAVFILE_FILTER), FILESELECT_REQ_PATH | FILESELECT_REQ_FILE);
    if (file.GetLength() == 0){
        DebugLog("Stop Loading");
        return;
    }
    PreloadFile(file);
}

void AudioPlayerWindow::PreloadFile(WString file){
    if (!file.EndsWith(L".wav")){
        WString message(IDS_WAVFILE_FORM_WARNING);
        WString caption(IDS_WAVFILE_FORM_WARNING_CAPTION);
        switch (ShellMsgBox(caption, message)){
        case MSGBOX_NO:
            DebugLog("AudioPlayerWindow::PreloadFile Stop Load File");
            return;
        case MSGBOX_YES:
            DebugLog("AudioPlayerWindow::PreloadFile Preparing FFmpeg");
            if (!ShellFFmpeg(file, L".\\temp.wav")){
                DebugError("AudioPlayerWindow::PreloadFile ShellFFmpegW Failed");
                return;
            }
            LoadFile(L".\\temp.wav");
            File::Delete(L".\\temp.wav");
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
    LoadObject(audioSrc);
    
    src.Close();

    DebugLog("AudioPlayerWindow::LoadFile Success");
}

void AudioPlayerWindow::LoadObject(AudioSourceObject* o){
    if (o){
        if (o->GetWindowRef()){
            DebugError("AudioPlayerWindow::LoadObject Object Has Already Been Loaded By Another Window");
            return;
        }
        if (source)
            source->SetWindowRef(NULL);
        source = o;
        o->SetWindowRef(this);
    }else{
        if (source)
            source->SetWindowRef(NULL);
        source = NULL;
    }
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