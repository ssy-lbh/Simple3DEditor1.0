#include <editor/PaintWindow.h>

#include <lib/glew/gl/glew.h>

#include <main.h>
#include <res.h>
#include <editor/gui/Menu.h>
#include <editor/dialog/ColorBoard.h>
#include <utils/math3d/Math.h>
#include <utils/math3d/Mesh.h>
#include <utils/gl/GLTexture2D.h>
#include <utils/gl/GLRenderTexture2D.h>
#include <utils/gl/GLComputeProgram.h>

PaintWindow::ClearBrush::ClearBrush(PaintWindow* window) : window(window) {
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

PaintWindow::ClearBrush::~ClearBrush(){
    if (prog) delete prog;
}

void PaintWindow::ClearBrush::OnLeftDown(){
    GLuint kernel;

    if (err)
        return;

    prog->BindTexture(0, window->paintTex, GL_READ_WRITE, GL_RGBA32F);

    kernel = prog->GetProgram();
    glProgramUniform3f(kernel, colorLoc, color.x, color.y, color.z);

    prog->Dispatch((window->GetWidth() + 7) >> 3, (window->GetHeight() + 7) >> 3, 1);
}

void PaintWindow::ClearBrush::OnCommand(int id){
    switch (id){
    case IDM_SELECT_COLOR:
        color = ColorBoard::GetColor();
        break;
    }
}

PaintWindow::DefaultBrush::DefaultBrush(PaintWindow* window) : window(window) {
    prog = new GLComputeProgram(IDS_BRUSH_OVERLAY);
    if (prog->CompileShader()){
        DebugError("Compute Shader %d Compile Error:", IDS_BRUSH_OVERLAY);
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

PaintWindow::DefaultBrush::DefaultBrush(PaintWindow* window, int shaderId) : window(window) {
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

PaintWindow::DefaultBrush::~DefaultBrush(){
    if (prog) delete prog;
}

void PaintWindow::DefaultBrush::OnLeftDown(){
    draw = true;
}

void PaintWindow::DefaultBrush::OnLeftUp(){
    draw = false;
}

void PaintWindow::DefaultBrush::OnMove(){
    if (draw){
        Draw();
    }
}

void PaintWindow::DefaultBrush::OnCommand(int id){
    switch (id){
    case IDM_SELECT_COLOR:
        color = ColorBoard::GetColor();
        break;
    }
}

void PaintWindow::DefaultBrush::Draw(){
    GLint offset[2];
    GLint position[2];
    GLfloat radius = 10.0f;
    GLuint kernel;

    if (err)
        return;

    prog->BindTexture(0, window->paintTex, GL_READ_WRITE, GL_RGBA32F);

    position[0] = window->GetWidth() * (window->cursorPos.x + 1.0f) * 0.5f;
    position[1] = window->GetHeight() * (window->cursorPos.y + 1.0f) * 0.5f;
    offset[0] = position[0] - 12;
    offset[1] = position[1] - 12;

    kernel = prog->GetProgram();

    glProgramUniform1i(kernel, paintLoc, 0);
    glProgramUniform2i(kernel, offsetLoc, offset[0], offset[1]);
    glProgramUniform2i(kernel, positionLoc, position[0], position[1]);
    glProgramUniform1f(kernel, radiusLoc, radius);
    glProgramUniform3f(kernel, colorLoc, color.x, color.y, color.z);
;
    prog->Dispatch(3, 3, 1);
}

PaintWindow::PaintWindow(){
    DebugLog("PaintWindow Launched");
    basicMenu = new Menu();

    Menu* brushMenu = new Menu();
    brushMenu->AddItem(new MenuItem(L"默认", MENUITEM_LAMBDA_TRANS(PaintWindow)[](PaintWindow* window){
        window->OnMenuAccel(IDM_BRUSH_DEFAULT, false);
    }, this));
    brushMenu->AddItem(new MenuItem(L"加色", MENUITEM_LAMBDA_TRANS(PaintWindow)[](PaintWindow* window){
        window->OnMenuAccel(IDM_BRUSH_RGB_ADD, false);
    }, this));
    brushMenu->AddItem(new MenuItem(L"减色", MENUITEM_LAMBDA_TRANS(PaintWindow)[](PaintWindow* window){
        window->OnMenuAccel(IDM_BRUSH_CMK_ADD, false);
    }, this));
    brushMenu->AddItem(new MenuItem(L"水彩(未完成)", MENUITEM_LAMBDA_TRANS(PaintWindow)[](PaintWindow* window){
        window->OnMenuAccel(IDM_BRUSH_WATERCOLOR, false);
    }, this));
    brushMenu->AddItem(new MenuItem(L"乘色", MENUITEM_LAMBDA_TRANS(PaintWindow)[](PaintWindow* window){
        window->OnMenuAccel(IDM_BRUSH_RGB_MUL, false);
    }, this));
    brushMenu->AddItem(new MenuItem(L"平均色", MENUITEM_LAMBDA_TRANS(PaintWindow)[](PaintWindow* window){
        window->OnMenuAccel(IDM_BRUSH_RGB_AVERAGE, false);
    }, this));
    brushMenu->AddItem(new MenuItem());
    brushMenu->AddItem(new MenuItem(L"清空", MENUITEM_LAMBDA_TRANS(PaintWindow)[](PaintWindow* window){
        window->OnMenuAccel(IDM_BRUSH_CLEAR, false);
    }, this));
    basicMenu->AddItem(new MenuItem(L"笔刷", brushMenu));

    basicMenu->AddItem(new MenuItem());
    basicMenu->AddItem(new MenuItem(L"使用画布纹理", MENUITEM_LAMBDA_TRANS(PaintWindow)[](PaintWindow* window){
        window->OnMenuAccel(IDM_TEXTURE_USE_PAINT, false);
    }, this));
    basicMenu->AddItem(new MenuItem(L"调色板", MENUITEM_LAMBDA_TRANS(PaintWindow)[](PaintWindow* window){
        window->OnMenuAccel(IDM_SELECT_COLOR, false);
    }, this));
}

PaintWindow::~PaintWindow(){
    DebugLog("PaintWindow Destroyed");
    if (basicMenu) delete basicMenu;
    if (brush) delete brush;
    if (paintTex) delete paintTex;
}

bool PaintWindow::IsFocus(){
    return focus;
}

void PaintWindow::OnCreate(){
    CreateImage(500, 500);
    SetBrush(new DefaultBrush(this, IDS_BRUSH_OVERLAY));
}

void PaintWindow::OnRender(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 测试代码

    // 可能是因为该段渲染流程使用的内部其他类型的管线
    // Vertex会进入内部VertexBuffer
    // Color会写入uniforn变量中
    // glEnd()以后执行DrawCall时，颜色只会以最后设置的颜色为准
    // 所以目前顶点属性的插值尚未解决

    // GLFrameBuffer* frame = new GLFrameBuffer();
    // frame->BindTexture(paintTex, 500, 500);
    // frame->Enable();
    // glColor3f(1.0f, 1.0f, 1.0f);
    // glBegin(GL_TRIANGLES);
    // glVertex2f(0.0f, 1.0f);
    // glVertex2f(0.866f, -0.5f);
    // glVertex2f(-0.866f, -0.5f);
    // glEnd();
    // frame->Disable();
    // delete frame;

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
    UpdateWindowSize(x, y);
}

void PaintWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
}

void PaintWindow::OnLeftDown(int x, int y){
    UpdateCursor(x, y);
    // 操作
    if (curOp){
        curOp->OnConfirm();
        delete curOp;
        curOp = NULL;
        return;
    }
    // 工具
    if (brush){
        brush->OnLeftDown();
    }
}

void PaintWindow::OnLeftUp(int x, int y){
    UpdateCursor(x, y);
    // 刷子可能应先收到左键弹起，再得知鼠标移动
    // 尚未决定是否修改
    if (brush){
        brush->OnLeftUp();
    }
}

void PaintWindow::OnRightDown(int x, int y){
    UpdateCursor(x, y);
    // 做成菜单后在此添加
    // 操作
    if (curOp){
        curOp->OnUndo();
        delete curOp;
        curOp = NULL;
        return;
    }
    // 工具
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

void PaintWindow::OnFocus(){
    focus = true;
}

void PaintWindow::OnKillFocus(){
    focus = false;
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

void PaintWindow::UpdateCursor(int x, int y){
    cursorPos.x = 2.0f * x / cliSize.x - 1.0f;
    cursorPos.y = 1.0f - 2.0f * y / cliSize.y;
    if (curOp)
        curOp->OnMove();
    if (brush)
        brush->OnMove();
}

void PaintWindow::UpdateWindowSize(int x, int y){
    cliSize.x = x;
    cliSize.y = y;
    aspect = cliSize.x / cliSize.y;
}

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