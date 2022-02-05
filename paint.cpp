#include "paint.h"

#include "main.h"
#include "geodef.h"

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

UVEditWindow::UVEditWindow(){
    SetTool(new EmptyTool(this));
}

UVEditWindow::~UVEditWindow(){}

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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (Main::data->mesh->EnableTexture()){
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
        glEnd();
        Main::data->mesh->DisableTexture();
    }

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

void UVEditWindow::OnMenuAccel(int id, bool accel){}

void UVEditWindow::UpdateCursor(int x, int y){
    cursorPos.x = 2.0f * x / cliSize.x - 1.0f;
    cursorPos.y = 2.0f * y / cliSize.y - 1.0f;
    if (curTool)
        curTool->OnMove();
}

void UVEditWindow::UpdateWindowSize(int x, int y){
    cliSize.x = x;
    cliSize.y = y;
    aspect = cliSize.x / cliSize.y;
}

void UVEditWindow::SetTool(ITool* tool){
    if (curTool){
        curTool->OnUnselect();
        delete curTool;
    }
    curTool = tool;
    tool->OnSelect();
}