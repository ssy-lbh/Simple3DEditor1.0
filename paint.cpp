#include "paint.h"

#include "main.h"
#include "res.h"
#include "geodef.h"

UVEditWindow::MoveOperation::MoveOperation(UVEditWindow* main) : main(main) {}
UVEditWindow::MoveOperation::~MoveOperation(){}

void UVEditWindow::MoveOperation::OnEnter(){
    DebugLog("MoveOperation OnEnter");
    x = y = true;
    start = main->cursorPos;
    if (Main::data->selectedPoints.Size() > 0){
        Main::data->selectedPoints.Foreach<MoveOperation*>([](Vertex* v, MoveOperation* op){
            op->moveInfo.Add({v, v->uv});
        }, this);
    }
}

void UVEditWindow::MoveOperation::OnMove(){
    Vector2 mov;
    if (moveInfo.Size() > 0){
        mov = (main->cursorPos - start) * 0.5f;
        mov = Vector2(x ? mov.x : 0.0f, y ? mov.y : 0.0f);
        moveInfo.Foreach<Vector2*>([](MoveInfo info, Vector2* offset){
            info.vert->uv = info.uv + *offset;
        }, &mov);
        DebugLog("MoveOperation OnMove %f %f", x ? mov.x : 0.0f, y ? mov.y : 0.0f);
    }
}

void UVEditWindow::MoveOperation::OnConfirm(){
    DebugLog("MoveOperation OnConfirm");
}

void UVEditWindow::MoveOperation::OnUndo(){
    DebugLog("MoveOperation OnUndo");
    moveInfo.Foreach([](MoveInfo info){
        info.vert->uv = info.uv;
    });
}

void UVEditWindow::MoveOperation::OnCommand(int id){
    switch (id){
    case IDM_OP_X: x = true; y = false; break;
    case IDM_OP_Y: y = true; x = false; break;
    case IDM_OP_Z: x = y = false; break;
    case IDM_OP_PLANE_X: x = false; y = true; break;
    case IDM_OP_PLANE_Y: y = false; x = true; break;
    case IDM_OP_PLANE_Z: x = y = true; break;
    }
}

UVEditWindow::EmptyTool::EmptyTool(UVEditWindow* window) : window(window) {}
UVEditWindow::EmptyTool::~EmptyTool(){}

void UVEditWindow::EmptyTool::OnLeftDown(){
    Vertex* v = Main::data->mesh->FindUV(Vector2((window->cursorPos.x + 1.0f) * 0.5f, (window->cursorPos.y + 1.0f) * 0.5f), 5.0f / window->cliSize.y);
    if (v == NULL){
        Main::data->selectedPoints.Clear();
        DebugLog("No Point Selected");
    }else{
        Main::data->selectedPoints.Add(v);
        DebugLog("Select Point %f %f", v->uv.x, v->uv.y);
    }
}

UVEditWindow::SelectTool::SelectTool(UVEditWindow* window) : window(window) {}
UVEditWindow::SelectTool::~SelectTool(){}

void UVEditWindow::SelectTool::OnLeftDown(){
    start = window->cursorPos;
    end = window->cursorPos;
    leftDown = true;
}

void UVEditWindow::SelectTool::OnLeftUp(){
    leftDown = false;
    if (start.x == end.x && start.y == end.y){
        Main::data->selectedPoints.Clear();
        return;
    }
    //TODO 等待实现范围框选
    Main::data->mesh->FindUVRect(
        Vector2((start.x + 1.0f) * 0.5f, (start.y + 1.0f) * 0.5f),
        Vector2((end.x + 1.0f) * 0.5f, (end.y + 1.0f) * 0.5f),
        Main::data->selectedPoints
    );
}

void UVEditWindow::SelectTool::OnMove(){
    end = window->cursorPos;
}

void UVEditWindow::SelectTool::OnRender(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (leftDown){
        glColor4f(1.0f, 1.0f, 0.0f, 0.1f);
        GLUtils::DrawRect(start, end);
    }
}

UVEditWindow::UVEditWindow(){
    SetTool(new EmptyTool(this));
}

UVEditWindow::~UVEditWindow(){
    if (curOp) delete curOp;
}

bool UVEditWindow::IsFocus(){
    return focus;
}

void UVEditWindow::OnRender(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_AUTO_NORMAL);
    glEnable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_ALPHA_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    if (Main::data->mesh->EnableTexture()){
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // V坐标反转
        // 变换矩阵会以(0.5,0.5)为中心对纹理坐标产生相应变换
        glOrtho(-1.0, 1.0, 1.0, -1.0, 0.0, 100.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
        glEnd();
        Main::data->mesh->DisableTexture();
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Main::data->mesh->RenderUVMap();

    // 已选择点绘制
    glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(8.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POINTS);
    Main::data->selectedPoints.Foreach([](Vertex* p){
        glVertex2f(p->uv.x, p->uv.y);
    });
    glEnd();
    glDisable(GL_POINT_SMOOTH);

    if (curTool)
        curTool->OnRender();
}

void UVEditWindow::OnResize(int x, int y){
    UpdateWindowSize(x, y);
}

void UVEditWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
}

void UVEditWindow::OnLeftDown(int x, int y){
    UpdateCursor(x, y);
    // 操作
    if (curOp){
        curOp->OnConfirm();
        delete curOp;
        curOp = NULL;
        return;
    }
    // 工具
    if (curTool){
        curTool->OnLeftDown();
    }
}

void UVEditWindow::OnLeftUp(int x, int y){
    UpdateCursor(x, y);
    if (curTool){
        curTool->OnLeftUp();
    }
}

void UVEditWindow::OnRightDown(int x, int y){
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
    if (curTool){
        curTool->OnRightDown();
    }
}

void UVEditWindow::OnRightUp(int x, int y){
    UpdateCursor(x, y);
    if (curTool){
        curTool->OnRightUp();
    }
}

void UVEditWindow::OnFocus(){
    focus = true;
}

void UVEditWindow::OnKillFocus(){
    focus = false;
}

void UVEditWindow::OnMenuAccel(int id, bool accel){
    switch (id){
    case IDM_MOVE:
        SetOperation(new MoveOperation(this));
        break;
    case IDM_TOOL_EMPTY:
        SetTool(new EmptyTool(this));
        break;
    case IDM_TOOL_SELECTBOX:
        SetTool(new SelectTool(this));
        break;
    case IDM_OP_X:
    case IDM_OP_Y:
    case IDM_OP_Z:
    case IDM_OP_PLANE_X:
    case IDM_OP_PLANE_Y:
    case IDM_OP_PLANE_Z:
        // 当前操作的命令
        if (curOp){
            curOp->OnCommand(id);
        }
        break;
    }
}

void UVEditWindow::OnDropFileW(const wchar_t* path){
    Main::data->mesh->SetTexture(new GLTexture2D(path));
}

void UVEditWindow::UpdateCursor(int x, int y){
    cursorPos.x = 2.0f * x / cliSize.x - 1.0f;
    cursorPos.y = 2.0f * y / cliSize.y - 1.0f;
    if (curOp)
        curOp->OnMove();
    if (curTool)
        curTool->OnMove();
}

void UVEditWindow::UpdateWindowSize(int x, int y){
    cliSize.x = x;
    cliSize.y = y;
    aspect = cliSize.x / cliSize.y;
}

void UVEditWindow::SetOperation(IOperation* op){
    if (curOp){
        DebugLog("UVEditWindow::SetOperation delete curOp");
        curOp->OnConfirm();
        delete curOp;
    }
    curOp = op;
    curOp->OnEnter();
}

void UVEditWindow::SetTool(ITool* tool){
    if (curTool){
        curTool->OnUnselect();
        delete curTool;
    }
    curTool = tool;
    tool->OnSelect();
}

PaintWindow::PaintWindow(){}
PaintWindow::~PaintWindow(){}

bool PaintWindow::IsFocus(){
    return focus;
}

void PaintWindow::OnRender(){}
void PaintWindow::OnResize(int x, int y){}
void PaintWindow::OnMouseMove(int x, int y){}
void PaintWindow::OnLeftDown(int x, int y){}
void PaintWindow::OnLeftUp(int x, int y){}
void PaintWindow::OnRightDown(int x, int y){}
void PaintWindow::OnRightUp(int x, int y){}
void PaintWindow::OnFocus(){}
void PaintWindow::OnKillFocus(){}
void PaintWindow::OnMenuAccel(int id, bool accel){}