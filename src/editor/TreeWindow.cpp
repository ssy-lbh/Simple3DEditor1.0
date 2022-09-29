#include <editor/TreeWindow.h>

#include <windows.h>
#include <gl/gl.h>

#include <main.h>
#include <res.h>
#include <editor/gui/Menu.h>
#include <editor/gui/GUIUtils.h>
#include <editor/dialog/Tips.h>
#include <util/os/Font.h>
#include <util/gl/GLUtils.h>
#include <editor/main/ViewObject.h>
#include <editor/object/AllObjects.h>
#include <editor/windows/AllWindows.h>

#include <json/nlohmann/json.hpp>

#include <windows.h>

namespace simple3deditor {

WINDOW_INFO_DECL(simple3deditor::TreeWindow, L"树形图");

TreeWindow::TreeWindow(){
    DebugLog("TreeWindow Launched");

    basicMenu = new Menu();

    Menu* objectMenu = new Menu();
    objectMenu->AddItem(new MenuItem(L"空对象", [=]{ this->AddObject(new AViewObject()); }));
    objectMenu->AddItem(new MenuItem(L"网格体", [=]{ this->AddObject(new MeshObject()); }));
    objectMenu->AddItem(new MenuItem(L"二次贝塞尔曲线", [=]{ this->AddObject(new SquareBezierObject()); }));
    objectMenu->AddItem(new MenuItem(L"三次贝塞尔曲线", [=]{ this->AddObject(new CubicBezierObject()); }));
    objectMenu->AddItem(new MenuItem(L"点光源", [=]{ this->AddObject(new PointLightObject()); }));
    objectMenu->AddItem(new MenuItem(L"GUI管理器", [=]{ this->AddObject(new GUIManagerObject()); }));
    objectMenu->AddItem(new MenuItem(L"GUI网格体", [=]{ this->AddObject(new GUIMeshObject()); }));

    Menu* objGUIMenu = new Menu();
    objGUIMenu->AddItem(new MenuItem(L"图标按钮", [=]{ this->AddObject(new IconButton()); }));
    objGUIMenu->AddItem(new MenuItem(L"编辑框", [=]{ this->AddObject(new GUIEditA()); }));
    objGUIMenu->AddItem(new MenuItem(L"横进度条", [=]{ this->AddObject(new HorizontalProgressBar()); }));
    objGUIMenu->AddItem(new MenuItem(L"竖进度条", [=]{ this->AddObject(new VerticalProgressBar()); }));
    objectMenu->AddItem(new MenuItem(L"GUI", objGUIMenu));

    Menu* windowsMenu = new Menu();
    windowsMenu->AddItem(new MenuItem(L"音频播放器", [=]{ this->AddObject(new AudioPlayerWindowObject()); }));
    windowsMenu->AddItem(new MenuItem(L"树形图", [=]{ this->AddObject(new TreeWindowObject()); }));
    objectMenu->AddItem(new MenuItem(L"窗口", windowsMenu));

    objectMenu->AddItem(new MenuItem(L"音频收听器", [=]{ LocalData::GetLocalInst()->CreateAudioListener(); }));
    objectMenu->AddItem(new MenuItem(L"摄像机", [=]{ LocalData::GetLocalInst()->CreateCamera(); }));
    basicMenu->AddItem(new MenuItem(L"添加对象", objectMenu));

    basicMenu->AddItem(new MenuItem(L"删除对象", [=]{
        if (!this->selObject){
            DebugError("No Object Selected");
            return;
        }
        Main::DeleteObject(this->selObject);
    }));
    basicMenu->AddItem(new MenuItem(L"重命名", [=]{
        wchar_t name[DEFAULT_STRING_LENGTH];

        if (!this->selObject){
            DebugError("No Object Selected");
            return;
        }
        DialogTextInput(name, DEFAULT_STRING_LENGTH);
        if (*name == L'\0'){
            DebugLog("Rename Operation Cancelled");
            return;
        }
        this->selObject->name = name;
    }));
}

TreeWindow::~TreeWindow(){
    DebugLog("TreeWindow Destroyed");
    if (basicMenu) delete basicMenu;
}

void TreeWindow::AddObject(AViewObject* o){
    if (selObject){
        selObject->AddChild(o);
        return;
    }
    this->AddObject(o);
}

void TreeWindow::OnRender(){
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_AUTO_NORMAL);
    glEnable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_ALPHA_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLUtils::ResetProjection();
    GLUtils::ResetModelView();

    objectList.Clear();

    depth = 0;
    RenderItem(Main::data->scene);
    RenderItem(Main::data->screen);
}

void TreeWindow::RenderItem(AViewObject* o){
    float xbase = -1.0f + 30.0f * (depth + 1) * cliInvSize.x;
    float ybase = 1.0f - 30.0f * objectList.Size() * cliInvSize.y;

    if (Main::data->curObject == o){
        glColor3f(0.0f, 0.0f, 1.0f);
        GLUtils::DrawRect(Vector2(-1.0f, ybase), Vector2(1.0f, ybase - 30.0f * cliInvSize.y));
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(xbase, ybase - 24.0f * cliInvSize.y);
    glDrawCNString(o->name.GetString());

    if (o->GetChildren().Size() > 0){
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLES);
        if (o->unfold){
            glVertex2f(xbase - 6.0f * cliInvSize.x, ybase - 10.0f * cliInvSize.y);
            glVertex2f(xbase - 15.0f * cliInvSize.x, ybase - 20.0f * cliInvSize.y);
            glVertex2f(xbase - 24.0f * cliInvSize.x, ybase - 10.0f * cliInvSize.y);
        }else{
            glVertex2f(xbase - 20.0f * cliInvSize.x, ybase - 6.0f * cliInvSize.y);
            glVertex2f(xbase - 10.0f * cliInvSize.x, ybase - 15.0f * cliInvSize.y);
            glVertex2f(xbase - 20.0f * cliInvSize.x, ybase - 24.0f * cliInvSize.y);
        }
        glEnd();
    }
    
    objectList.Add(o);

    if (o->unfold){
        depth++;
        o->GetChildren().Foreach([=](AViewObject* o){
            this->RenderItem(o);
        });
        depth--;
    }
}

void TreeWindow::OnChar(char c){}

void TreeWindow::OnUnichar(wchar_t c){}

void TreeWindow::OnResize(int x, int y){
    AWindow::OnResize(x, y);
    UpdateWindowSize(x, y);
}

void TreeWindow::UpdateCursor(int x, int y){
    AWindow::UpdateCursor(x, y);
}

void TreeWindow::UpdateWindowSize(int x, int y){}

void TreeWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
    if (dragObject)
        Main::SetCursor(IDC_UPARROW);
}

void TreeWindow::OnLeftDown(int x, int y){
    size_t selected;

    UpdateCursor(x, y);

    selected = (size_t)((1.0f - cursorPos.y) / (30.0f * cliInvSize.y));
    dragObject = (selected < objectList.Size() ? objectList[selected] : NULL);
}

void TreeWindow::OnLeftUp(int x, int y){
    size_t selected;

    UpdateCursor(x, y);

    if (!dragObject)
        return;

    selected = (size_t)((1.0f - cursorPos.y) / (30.0f * cliInvSize.y));
    if (selected < objectList.Size()){
        AViewObject* o = objectList[selected];
        if (o == dragObject){
            if (Main::data->curObject != o){
                DebugLog("TreeWindow Select Object %S", o->name.GetString());
                Main::SelectObject(o);
                dragObject = NULL;
                return;
            }
            o->unfold = !o->unfold;
            DebugLog("TreeWindow %s Object %S", o->unfold ? "Unfold" : "Fold", o->name.GetString());
        }else{
            if (!o || !Main::AddObjectChild(o, dragObject)){
                dragObject = NULL;
                return;
            }
            DebugLog("TreeWindow Set %S As Child Of %S", dragObject->name.GetString(), o->name.GetString());
        }
    }
    dragObject = NULL;
}

void TreeWindow::OnRightDown(int x, int y){
    size_t selPos;

    UpdateCursor(x, y);
    
    selPos = (size_t)((1.0f - cursorPos.y) / (30.0f * cliInvSize.y));
    selObject = (selPos < objectList.Size() ? objectList[selPos] : NULL);
    Main::SetMenu(basicMenu);
}

void TreeWindow::OnRightUp(int x, int y){
    UpdateCursor(x, y);
}

void TreeWindow::OnMouseWheel(int delta){}

void TreeWindow::OnMenuAccel(int id, bool accel){
    switch (id){
    case IDM_DELETE:
        if (Main::data->curObject)
            Main::DeleteObject(Main::data->curObject);
        break;
    case IDM_MENU_BASIC:
        selObject = Main::data->curObject;
        Main::SetMenu(basicMenu);
        break;
    }
}

void TreeWindow::Serialize(nlohmann::json& o){
    o["id"] = WINDOW_ID;
}

void TreeWindow::Deserialize(nlohmann::json& o){}

}