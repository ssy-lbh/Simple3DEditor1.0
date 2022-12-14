#include <editor/PaintWindow.h>

#include <glew/gl/glew.h>

#include <main.h>
#include <res.h>
#include <editor/gui/Menu.h>
#include <editor/dialog/ColorBoard.h>
#include <util/math3d/Math.h>
#include <util/math3d/Mesh.h>
#include <util/gl/GLTexture2D.h>
#include <util/gl/GLRenderTexture2D.h>
#include <util/gl/GLFrameBuffer.h>
#include <util/gl/GLComputeProgram.h>

#include <json/nlohmann/json.hpp>

namespace simple3deditor {

class PaintWindow::ClearBrush : public ITool {
private:
    PaintWindow* window;
    uint colorLoc;
    Vector3 color = Vector3::one;

    GLComputeProgram* prog = NULL;
    bool err = false;

public:
    ClearBrush(PaintWindow* window) : window(window) {
        prog = new GLComputeProgram(IDS_BRUSH_CLEAR);
        if (prog->CompileShader()){
            DebugError("Compute Shader %d Compile Error:", IDS_BRUSH_CLEAR);
            prog->PrintShaderLog();
            err = true;
        }
        if (prog->LinkProgram()){
            DebugError("Program Link Error:");
            prog->PrintProgramLog();
            err = true;
        }

        if (err)
            return;

        colorLoc = prog->GetLoc("color");
    }

    virtual ~ClearBrush() override{
        if (prog) delete prog;
    }

    virtual void OnLeftDown() override{
        GLuint kernel;

        if (err)
            return;

        prog->BindTexture(0, window->paintTex, GL_READ_WRITE, GL_RGBA32F);

        kernel = prog->GetProgram();
        glProgramUniform3f(kernel, colorLoc, color.x, color.y, color.z);

        prog->Dispatch((window->GetWidth() + 7) >> 3, (window->GetHeight() + 7) >> 3, 1);
    }

    virtual void OnCommand(int id) override{
        switch (id){
        case IDM_SELECT_COLOR:
            color = ColorBoard::GetColor();
            break;
        }
    }
};

class PaintWindow::DefaultBrush : public ITool {
private:
    PaintWindow* window;
    bool draw = false;

    // Uniformλ??
    uint paintLoc;
    uint offsetLoc;
    uint positionLoc;
    uint radiusLoc;
    uint colorLoc;

    Vector3 color = Vector3::one;

    GLComputeProgram* prog = NULL;
    bool err = false;

public:
    DefaultBrush(PaintWindow* window) : DefaultBrush(window, IDS_BRUSH_OVERLAY) {}

    DefaultBrush(PaintWindow* window, int shaderId) : window(window) {
        prog = new GLComputeProgram(shaderId);
        if (prog->CompileShader()){
            DebugError("Compute Shader %d Compile Error:", shaderId);
            prog->PrintShaderLog();
            err = true;
        }
        if (prog->LinkProgram()){
            DebugError("Program Link Error:");
            prog->PrintProgramLog();
            err = true;
        }

        if (err)
            return;

        paintLoc = prog->GetLoc("paint");
        offsetLoc = prog->GetLoc("offset");
        positionLoc = prog->GetLoc("position");
        radiusLoc = prog->GetLoc("radius");
        colorLoc = prog->GetLoc("color");
    }

    virtual ~DefaultBrush() override{
        if (prog) delete prog;
    }

    virtual void OnLeftDown() override{
        draw = true;
    }
    
    virtual void OnLeftUp() override{
        draw = false;
    }

    virtual void OnMove() override{
        if (draw){
            Draw();
        }
    }

    virtual void OnCommand(int id) override{
        switch (id){
        case IDM_SELECT_COLOR:
            color = ColorBoard::GetColor();
            break;
        }
    }

    void Draw(){
        GLint offset[2];
        GLint position[2];
        GLfloat radius = 10.0f;
        GLuint kernel;

        if (err)
            return;

        prog->BindTexture(0, window->paintTex, GL_READ_WRITE, GL_RGBA32F);

        position[0] = (GLint)(window->GetWidth() * (window->cursorPos.x + 1.0f) * 0.5f);
        position[1] = (GLint)(window->GetHeight() * (window->cursorPos.y + 1.0f) * 0.5f);
        offset[0] = position[0] - 12;
        offset[1] = position[1] - 12;

        kernel = prog->GetProgram();

        glProgramUniform1i(kernel, paintLoc, 0);
        glProgramUniform2i(kernel, offsetLoc, offset[0], offset[1]);
        glProgramUniform2i(kernel, positionLoc, position[0], position[1]);
        glProgramUniform1f(kernel, radiusLoc, radius);
        glProgramUniform3f(kernel, colorLoc, color.x, color.y, color.z);

        prog->Dispatch(3, 3, 1);
    }
};

WINDOW_INFO_DECL(simple3deditor::PaintWindow, L"?滭????");

PaintWindow::PaintWindow(){
    DebugLog("PaintWindow Launched");
    basicMenu = new Menu();

    Menu* brushMenu = new Menu();
    brushMenu->AddItem(new MenuItem(L"Ĭ??", [=]{ this->OnMenuAccel(IDM_BRUSH_DEFAULT, false); }));
    brushMenu->AddItem(new MenuItem(L"??ɫ", [=]{ this->OnMenuAccel(IDM_BRUSH_RGB_ADD, false); }));
    brushMenu->AddItem(new MenuItem(L"??ɫ", [=]{ this->OnMenuAccel(IDM_BRUSH_CMK_ADD, false); }));
    brushMenu->AddItem(new MenuItem(L"ˮ??(δ????)", [=]{ this->OnMenuAccel(IDM_BRUSH_WATERCOLOR, false); }));
    brushMenu->AddItem(new MenuItem(L"??ɫ", [=]{ this->OnMenuAccel(IDM_BRUSH_RGB_MUL, false); }));
    brushMenu->AddItem(new MenuItem(L"ƽ??ɫ", [=]{ this->OnMenuAccel(IDM_BRUSH_RGB_AVERAGE, false); }));
    brushMenu->AddItem(new MenuItem());
    brushMenu->AddItem(new MenuItem(L"????", [=]{ this->OnMenuAccel(IDM_BRUSH_CLEAR, false); }));
    basicMenu->AddItem(new MenuItem(L"??ˢ", brushMenu));

    basicMenu->AddItem(new MenuItem());
    basicMenu->AddItem(new MenuItem(L"ʹ?û???????", [=]{ this->OnMenuAccel(IDM_TEXTURE_USE_PAINT, false); }));
    basicMenu->AddItem(new MenuItem(L"??ɫ??", [=]{ this->OnMenuAccel(IDM_SELECT_COLOR, false); }));

    CreateImage(500, 500);
    SetBrush(new DefaultBrush(this, IDS_BRUSH_OVERLAY));
}

PaintWindow::~PaintWindow(){
    DebugLog("PaintWindow Destroyed");
    if (basicMenu) delete basicMenu;
    if (brush) delete brush;
    if (paintTex) delete paintTex;
}

void PaintWindow::OnRender(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (paintTex->Enable()){
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
        glEnd();
        GLRenderTexture2D::Disable();
    }
}

void PaintWindow::OnResize(int x, int y){
    AWindow::OnResize(x, y);
    UpdateWindowSize(x, y);
}

void PaintWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
}

void PaintWindow::OnLeftDown(int x, int y){
    UpdateCursor(x, y);
    // ????
    if (curOp){
        curOp->OnConfirm();
        delete curOp;
        curOp = NULL;
        return;
    }
    // ????
    if (brush){
        brush->OnLeftDown();
    }
}

void PaintWindow::OnLeftUp(int x, int y){
    UpdateCursor(x, y);
    // ˢ?ӿ???Ӧ???յ????????????ٵ?֪?????ƶ?
    // ??δ?????Ƿ??޸?
    if (brush){
        brush->OnLeftUp();
    }
}

void PaintWindow::OnRightDown(int x, int y){
    UpdateCursor(x, y);
    // ???ɲ˵????ڴ?????
    // ????
    if (curOp){
        curOp->OnUndo();
        delete curOp;
        curOp = NULL;
        return;
    }
    // ????
    if (brush){
        brush->OnRightDown();
    }
    Main::SetMenu(basicMenu);
}

void PaintWindow::OnRightUp(int x, int y){
    UpdateCursor(x, y);
    if (brush){
        brush->OnRightUp();
    }
}

void PaintWindow::OnMenuAccel(int id, bool accel){
    switch (id){
    case IDM_TEXTURE_USE_PAINT:{
        Mesh* mesh = Main::GetMesh();
        if (!mesh)
            return;
        mesh->SetTexture(new GLTexture2D(paintTex));
    }
        break;
    case IDM_BRUSH_CLEAR:
        SetBrush(new ClearBrush(this));
        break;
    case IDM_BRUSH_DEFAULT:
        SetBrush(new DefaultBrush(this, IDS_BRUSH_OVERLAY));
        break;
    case IDM_BRUSH_RGB_ADD:
        SetBrush(new DefaultBrush(this, IDS_BRUSH_RGB_ADD));
        break;
    case IDM_BRUSH_CMK_ADD:
        SetBrush(new DefaultBrush(this, IDS_BRUSH_CMK_ADD));
        break;
    case IDM_BRUSH_WATERCOLOR:
        SetBrush(new DefaultBrush(this, IDS_BRUSH_WATERCOLOR));
        break;
    case IDM_BRUSH_RGB_MUL:
        SetBrush(new DefaultBrush(this, IDS_BRUSH_RGB_MUL));
        break;
    case IDM_BRUSH_RGB_AVERAGE:
        SetBrush(new DefaultBrush(this, IDS_BRUSH_RGB_AVERAGE));
        break;
    case IDM_MENU_BASIC:
        Main::SetMenu(basicMenu);
        break;
    }
    if (brush)
        brush->OnCommand(id);
}

void PaintWindow::Serialize(nlohmann::json& o){
    o["id"] = WINDOW_ID;
}

void PaintWindow::Deserialize(nlohmann::json& o){}

void PaintWindow::UpdateCursor(int x, int y){
    AWindow::UpdateCursor(x, y);
    cursorPos.y = -cursorPos.y;
    if (curOp)
        curOp->OnMove();
    if (brush)
        brush->OnMove();
}

void PaintWindow::UpdateWindowSize(int x, int y){}

void PaintWindow::SetOperation(IOperation* op){
    if (curOp){
        DebugLog("PaintWindow::SetOperation delete curOp");
        curOp->OnConfirm();
        delete curOp;
    }
    curOp = op;
    curOp->OnEnter();
}

void PaintWindow::SetBrush(ITool* tool){
    if (brush){
        brush->OnUnselect();
        delete brush;
    }
    brush = tool;
    tool->OnSelect();
}

void PaintWindow::CreateImage(int x, int y){
    DebugLog("PaintWindow::CreateImage %d %d", x, y);

    if (paintTex)
        delete paintTex;

    paintTex = new GLRenderTexture2D(x, y, GL_RGBA32F);
}

bool PaintWindow::InRect(int x, int y){
    return (GLuint)x < (GLuint)paintTex->GetWidth() &&
            (GLuint)y < (GLuint)paintTex->GetHeight();
}

int PaintWindow::GetWidth(){
    return paintTex->GetWidth();
}

int PaintWindow::GetHeight(){
    return paintTex->GetHeight();
}

}