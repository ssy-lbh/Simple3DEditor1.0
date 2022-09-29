#include <editor/UVEditWindow.h>

#include <windows.h>
#include <gl/gl.h>

#include <main.h>
#include <res.h>
#include <io/IOStream.h>
#include <util/List.h>
#include <util/gl/GLTexture2D.h>
#include <util/gl/GLUtils.h>
#include <util/math3d/Mesh.h>
#include <util/math3d/Geometry.h>
#include <editor/main/ViewObject.h>
#include <editor/main/Tool.h>
#include <editor/main/Operation.h>
#include <editor/gui/Menu.h>

#include <json/nlohmann/json.hpp>

namespace simple3deditor {

class UVEditWindow::MoveOperation : public IOperation {
private:
    struct MoveInfo {
        Vertex* vert;
        Vector2 uv;
    };

    Vector2 start;
    List<MoveInfo> moveInfo;
    bool x, y;
    UVEditWindow* main;

public:
    MoveOperation(UVEditWindow* main) : main(main) {}
    virtual ~MoveOperation() override{}

    virtual void OnEnter() override{
        DebugLog("MoveOperation OnEnter");
        x = y = true;
        start = main->cursorPos;
        if (Main::data->selPoints.Size() > 0){
            Main::data->selPoints.Foreach([=](Vertex* v){
                this->moveInfo.Add({v, v->uv});
            });
        }
    }
    virtual void OnMove() override{
        Vector2 mov;
        if (moveInfo.Size() > 0){
            mov = (main->cursorPos - start) * 0.5f;
            mov = Vector2(x ? mov.x : 0.0f, y ? mov.y : 0.0f);
            moveInfo.Foreach([=](MoveInfo info){
                info.vert->uv = info.uv + mov;
            });
            DebugLog("MoveOperation OnMove %f %f", x ? mov.x : 0.0f, y ? mov.y : 0.0f);
        }
    }

    virtual void OnCommand(int id) override{
        switch (id){
        case IDM_OP_X: x = true; y = false; break;
        case IDM_OP_Y: y = true; x = false; break;
        case IDM_OP_Z: x = y = false; break;
        case IDM_OP_PLANE_X: x = false; y = true; break;
        case IDM_OP_PLANE_Y: y = false; x = true; break;
        case IDM_OP_PLANE_Z: x = y = true; break;
        }
    }

    virtual void OnConfirm() override{
        DebugLog("MoveOperation OnConfirm");
    }

    virtual void OnUndo() override{
        DebugLog("MoveOperation OnUndo");
        moveInfo.Foreach([](MoveInfo info){
            info.vert->uv = info.uv;
        });
    }
};

class UVEditWindow::EmptyTool : public ITool {
private:
    UVEditWindow* window;

public:
    EmptyTool(UVEditWindow* window) : window(window) {}
    ~EmptyTool() override{}

    virtual void OnLeftDown() override{
        if (!Main::data->curObject)
            return;
        Main::data->curObject->OnSelectUV(Vector2((window->cursorPos.x + 1.0f) * 0.5f, (window->cursorPos.y + 1.0f) * 0.5f), 5.0f / window->cliSize.y);
    }
};

class UVEditWindow::SelectTool : public ITool {
private:
    UVEditWindow* window;
    Vector2 start;
    Vector2 end;
    bool leftDown;
    
public:
    SelectTool(UVEditWindow* window) : window(window) {}
    virtual ~SelectTool() override{}

    virtual void OnLeftDown() override{
        start = window->cursorPos;
        end = window->cursorPos;
        leftDown = true;
    }

    virtual void OnLeftUp() override{
        leftDown = false;
        if (start.x == end.x && start.y == end.y){
            Main::data->selPoints.Clear();
            return;
        }
        if (!Main::data->curObject)
            return;
        Main::data->curObject->OnSelectUV(
            Vector2((start.x + 1.0f) * 0.5f, (start.y + 1.0f) * 0.5f),
            Vector2((end.x + 1.0f) * 0.5f, (end.y + 1.0f) * 0.5f)
        );
    }

    virtual void OnMove() override{
        end = window->cursorPos;
    }

    virtual void OnRender() override{
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 2.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        if (leftDown){
            glColor4f(1.0f, 1.0f, 0.0f, 0.1f);
            GLUtils::DrawRect(start, end);
        }
    }
};

WINDOW_INFO_DECL(simple3deditor::UVEditWindow, L"UV编辑器");

UVEditWindow::UVEditWindow(){
    DebugLog("UVEditWindow Launched");
    SetTool(new EmptyTool(this));
}

UVEditWindow::~UVEditWindow(){
    DebugLog("UVEditWindow Destroyed");
    if (curOp) delete curOp;
    if (curTool) delete curTool;
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

    Mesh* mesh = Main::GetMesh();
    if (mesh && mesh->EnableTexture()){
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // V坐标反转
        // 变换矩阵会以(0.5,0.5)为中心对纹理坐标产生相应变换
        glOrtho(-1.0, 1.0, 1.0, -1.0, 0.0, 2.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
        glEnd();
        mesh->DisableTexture();
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, 0.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (Main::data->curObject)
        Main::data->curObject->OnRenderUVMap();

    // 已选择点绘制
    glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(8.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POINTS);
    Main::data->selPoints.Foreach([](Vertex* p){
        glVertex2f(p->uv.x, p->uv.y);
    });
    glEnd();
    glDisable(GL_POINT_SMOOTH);

    if (curTool)
        curTool->OnRender();
}

void UVEditWindow::OnResize(int x, int y){
    AWindow::OnResize(x, y);
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

void UVEditWindow::OnDropFileW(const wchar_t* path, uint len){
    Mesh* mesh = Main::GetMesh();
    if (!mesh)
        return;
    mesh->SetTexture(new GLTexture2D(path));
}

void UVEditWindow::UpdateCursor(int x, int y){
    AWindow::UpdateCursor(x, y);
    if (curOp)
        curOp->OnMove();
    if (curTool)
        curTool->OnMove();
}

void UVEditWindow::UpdateWindowSize(int x, int y){}

void UVEditWindow::Serialize(nlohmann::json& o){
    o["id"] = WINDOW_ID;
}

void UVEditWindow::Deserialize(nlohmann::json& o){}

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

}