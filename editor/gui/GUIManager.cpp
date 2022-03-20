#include <editor/gui/GUIManager.h>

#include <lib/glew/gl/glew.h>

#include <main.h>
#include <utils/gl/GLSimplified.h>
#include <utils/gl/GLUtils.h>
#include <utils/gl/GLTexture2D.h>
#include <utils/os/Log.h>
#include <utils/os/Font.h>
#include <editor/gui/ViewManager.h>

GUIManager::GUIManager() : AViewObject(L"GUIManager", ViewObjectType::GUI_MANAGER) {}

GUIManager::~GUIManager(){}

void GUIManager::UpdateCursor(Point3 ori, Vector3 dir){
    camPos = transform.chainInvMat * ori;
    cursorDir = transform.chainInvMat * dir;
    // 与XY上的正方形相交
    if (cursorDir.z == 0.0f)
        return;
    cursorPos = camPos - cursorDir * (camPos.z / cursorDir.z);
    FindCurrent();
}

bool GUIManager::DeleteChild(AViewObject* o){
    if (!AViewObject::DeleteChild(o)){
        DebugLog("GUIManager::DeleteButton Failed");
        return false;
    }
    if (cur == o){
        cur = NULL;
    }
    return true;
}

void GUIManager::OnMouseMove(Point3 ori, Vector3 dir){
    UpdateCursor(ori, dir);
    if (leftDown && cur){
        cur->OnLeftDrag(cursorPos - startPos, dir);
        return;
    }
    if (rightDown && cur){
        cur->OnRightDrag(cursorPos - startPos, dir);
        return;
    }
    FindCurrent();
}

void GUIManager::OnRender(){
    AViewObject::OnRender();

    // glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
    // GLUtils::DrawRect(Rect(-1.0f, 1.0f, -1.0f, 1.0f));
}

void GUIManager::OnLeftDown(Point3 ori, Vector3 dir){
    UpdateCursor(ori, dir);
    startPos = cursorPos;
    if (focus != cur){
        if (focus)
            focus->OnKillFocus();
        focus = cur;
        if (focus)
            focus->OnFocus();
    }
    if (cur){
        leftDown = true;
        cur->OnLeftDown(cursorPos, cursorDir);
    }
}

void GUIManager::OnLeftUp(Point3 ori, Vector3 dir){
    UpdateCursor(ori, dir);
    if (cur){
        AViewObject* preObj = cur;
        FindCurrent();
        preObj->OnLeftUp(cursorPos, dir);
    }
    if (leftDown)
        leftDown = false;
}

void GUIManager::OnRightDown(Point3 ori, Vector3 dir){
    UpdateCursor(ori, dir);
    startPos = cursorPos;
    if (focus != cur){
        if (focus)
            focus->OnKillFocus();
        focus = cur;
        if (focus)
            focus->OnFocus();
    }
    if (cur){
        rightDown = true;
        cur->OnRightDown(cursorPos, dir);
    }
}

void GUIManager::OnRightUp(Point3 ori, Vector3 dir){
    UpdateCursor(ori, dir);
    if (cur){
        AViewObject* preObj = cur;
        FindCurrent();
        preObj->OnRightUp(cursorPos, dir);
    }
    if (rightDown)
        rightDown = false;
}

void GUIManager::OnMouseWheel(int delta){
    if (cur)
        cur->OnMouseWheel(delta);
}

void GUIManager::OnMenuAccel(int id, bool accel){
    if (cur)
        cur->OnMenuAccel(id, accel);
}

void GUIManager::OnKillFocus(){
    if (focus){
        focus->OnKillFocus();
        focus = NULL;
    }
    cur = NULL;
}

void GUIManager::OnChar(char c){
    if (cur)
        cur->OnChar(c);
}

void GUIManager::OnUnichar(wchar_t c){
    if (cur)
        cur->OnUnichar(c);
}

void GUIManager::OnDropFileA(const char* path){
    if (cur)
        cur->OnDropFileA(path);
}

void GUIManager::OnDropFileW(const wchar_t* path){
    if (cur)
        cur->OnDropFileW(path);
}

AViewObject* GUIManager::GetCurrent(){
    return cur;
}

AViewObject* GUIManager::FindCurrent(){
    cur = NULL;
    children.Foreach<GUIManager*>([](AViewObject* o, GUIManager* mgr){
        if(o->OnHit(mgr->cursorPos, mgr->cursorDir)){
            mgr->cur = o;
            o->OnMouseMove(mgr->cursorPos, mgr->cursorDir);
        }
    }, this);
    return cur;
}

IconButton::IconButton(Vector2 position, Vector2 size) : AViewObject(L"IconButton", ViewObjectType::GUI_ICON_BUTTON), position(position), size(size) {}
IconButton::IconButton(Vector2 position, Vector2 size, float radius) : AViewObject(L"IconButton", ViewObjectType::GUI_ICON_BUTTON), position(position), size(size), radius(radius) {}

IconButton::~IconButton(){
    if (texture) delete texture;
}

bool IconButton::OnHit(Point3 ori, Vector3 dir){
    hover = ori.x >= position.x && ori.x <= position.x + size.x && ori.y >= position.y && ori.y <= position.y + size.y;
    return hover;
}

void IconButton::OnLeftDown(Point3 ori, Vector3 dir){
    if (onClick)
        onClick(userData);
    recPos = position;
}

void IconButton::OnLeftDrag(Vector3 delta, Vector3 dir){
    DebugLog("Drag %f %f", delta.x, delta.y);
    position = recPos + delta.XY();
}

void IconButton::OnRender(){
    if (texture)
        texture->Enable();
    glColorv3(hover ? Vector3(1.0f, 1.0f, 1.0f) : Vector3(0.8f, 0.8f, 0.8f));
    GLUtils::DrawRoundRectWithUV(
        Rect(position, position + size),
        radius, 0.05f
    );
    GLTexture2D::Disable();
}

void IconButton::OnClick(void(*func)(void*)){
    onClick = func;
}

void IconButton::SetUserData(void* data){
    userData = data;
}

void IconButton::SetIcon(const char* texPath){
    if (texture)
        delete texture;
    texture = new GLTexture2D(texPath);
}

void IconButton::SetIcon(int iconRes){
    if (texture)
        delete texture;
    texture = new GLTexture2D(iconRes);
}

void IconButton::SetIcon(GLTexture2D* tex){
    if (texture)
        delete texture;
    texture = tex;
}

GUIEditA::GUIEditA(Vector2 pos, float width) : AViewObject(L"GUIEditA", ViewObjectType::GUI_EDIT_A), position(pos), size(Vector2(width, 0.0f)) {
    text[0] = '\0';
}

GUIEditA::GUIEditA(Vector2 pos, float width, void(*onEdit)(char*, void*)) : AViewObject(L"GUIEditA", ViewObjectType::GUI_EDIT_A), position(pos), size(Vector2(width, 0.0f)), onEdit(onEdit) {
    text[0] = '\0';
}

GUIEditA::GUIEditA(Vector2 pos, float width, void(*onEdit)(char*, void*), void* userData) : AViewObject(L"GUIEditA", ViewObjectType::GUI_EDIT_A), position(pos), size(Vector2(width, 0.0f)), onEdit(onEdit), userData(userData) {
    text[0] = '\0';
}

GUIEditA::~GUIEditA(){}

bool GUIEditA::OnHit(Point3 ori, Vector3 dir){
    return ori.x >= position.x && ori.x <= position.x + size.x && ori.y >= position.y && ori.y <= position.y + size.y;
}

void GUIEditA::OnMouseMove(Point3 ori, Vector3 dir){
    Main::SetCursor(IDC_IBEAM);
}

void GUIEditA::OnLeftDown(Point3 ori, Vector3 dir){
    Main::SetCursor(IDC_IBEAM);
    editing = true;
}

void GUIEditA::OnKillFocus(){
    if (editing){
        editing = false;
        text[editPos] = '\0';
        if (onEdit)
            onEdit(text, userData);
    }
}

void GUIEditA::OnChar(char c){
    if (!editing){
        return;
    }
    switch (c){
    case '\r':
    case '\n':
        if (editing){
            editing = false;
            if (onEdit)
                onEdit(text, userData);
        }
        return;
    case '\b':
        if (editPos == 0)
            return;
        text[--editPos] = '\0';
        return;
    }
    if (editPos >= DEFAULT_STRING_LENGTH){
        return;
    }
    text[editPos++] = c;
    text[editPos] = '\0';
    return;
}

void GUIEditA::OnRender(){
    ViewManager* viewMgr = ViewManager::GetLocalInst();

    Vector2 cliSize = viewMgr->GetSize();
    Vector2 cliInvSize = Vector2(1.0f / cliSize.x, 1.0f / cliSize.y);
    float strWidth;

    size.y = cliInvSize.y * 30.0f;

    glColor3f(bkColor.x, bkColor.y, bkColor.z);
    if (radius == 0.0f){
        GLUtils::DrawRect(position, position + size);
    }else{
        GLUtils::DrawRoundRect(Rect(position, position + size), radius * size.y, 0.05f);
    }

    glColor3f(fontColor.x, fontColor.y, fontColor.z);
    glRasterPos2f(position.x, position.y + 6.0f * cliInvSize.y);
    glDrawCNString(text);

    if (editing){
        strWidth = glGetStringWidth(text);
        glColor3f(1.0f, 1.0f, 1.0f);
        GLUtils::DrawRect(position.x + (strWidth - 1.0f) * cliInvSize.x * 2.0f, position.y, position.x + (strWidth + 1.0f) * cliInvSize.x * 2.0f, position.y + size.y);
    }
}

char* GUIEditA::GetText(){ return text; }
size_t GUIEditA::GetTextMaxLength(){ return DEFAULT_STRING_LENGTH; }

void GUIEditA::SetBackgroundColor(Vector3 color){ bkColor = color; }
void GUIEditA::SetFontColor(Vector3 color){ fontColor = color; }
void GUIEditA::SetSelectionColor(Vector3 color){ selColor = color; }
void GUIEditA::SetCornerRadius(float radius){ this->radius = radius; }
Vector3 GUIEditA::GetBackgroundColor(){ return bkColor; }
Vector3 GUIEditA::GetFontColor(){ return fontColor; }
Vector3 GUIEditA::GetSelectionColor(){ return selColor; }
float GUIEditA::GetCornerRadius(){ return radius; }

GUIEditW::GUIEditW(Vector2 pos, float width) : AViewObject(L"GUIEditW", ViewObjectType::GUI_EDIT_W), position(pos), size(Vector2(width, 0.0f)) {
    text[0] = L'\0';
}

GUIEditW::GUIEditW(Vector2 pos, float width, void(*onEdit)(wchar_t*, void*)) : AViewObject(L"GUIEditW", ViewObjectType::GUI_EDIT_W), position(pos), size(Vector2(width, 0.0f)), onEdit(onEdit) {
    text[0] = L'\0';
}

GUIEditW::GUIEditW(Vector2 pos, float width, void(*onEdit)(wchar_t*, void*), void* userData) : AViewObject(L"GUIEditW", ViewObjectType::GUI_EDIT_W), position(pos), size(Vector2(width, 0.0f)), onEdit(onEdit), userData(userData) {
    text[0] = L'\0';
}

GUIEditW::~GUIEditW(){}

bool GUIEditW::OnHit(Point3 ori, Vector3 dir){
    return ori.x >= position.x && ori.x <= position.x + size.x && ori.y >= position.y && ori.y <= position.y + size.y;
}

void GUIEditW::OnMouseMove(Point3 ori, Vector3 dir){
    Main::SetCursor(IDC_IBEAM);
}

void GUIEditW::OnLeftDown(Point3 ori, Vector3 dir){
    Main::SetCursor(IDC_IBEAM);
    editing = true;
}

void GUIEditW::OnKillFocus(){
    editing = false;
    text[editPos] = L'\0';
}

void GUIEditW::OnUnichar(wchar_t c){
    if (!editing){
        return;
    }
    switch (c){
    case '\r':
    case '\n':
        if (editing){
            editing = false;
            if (onEdit)
                onEdit(text, userData);
        }
        return;
    case L'\b':
        if (editPos == 0)
            return;
        text[--editPos] = L'\0';
        return;
    }
    if (editPos >= DEFAULT_STRING_LENGTH){
        return;
    }
    text[editPos++] = c;
    text[editPos] = L'\0';
    return;
}

void GUIEditW::OnRender(){
    ViewManager* viewMgr = ViewManager::GetLocalInst();

    Vector2 cliSize = viewMgr->GetSize();
    Vector2 cliInvSize = Vector2(1.0f / cliSize.x, 1.0f / cliSize.y);
    float strWidth;

    size.y = cliInvSize.y * 30.0f;

    glColor3f(bkColor.x, bkColor.y, bkColor.z);
    if (radius == 0.0f){
        GLUtils::DrawRect(position, position + size);
    }else{
        GLUtils::DrawRoundRect(Rect(position, position + size), radius * size.y, 0.05f);
    }

    glColor3f(fontColor.x, fontColor.y, fontColor.z);
    glRasterPos2f(position.x, position.y + 6.0f * cliInvSize.y);
    glDrawCNString(text);

    if (editing){
        strWidth = glGetCNStringWidth(text);
        glColor3f(1.0f, 1.0f, 1.0f);
        GLUtils::DrawRect(position.x + (strWidth - 1.0f) * cliInvSize.x * 2.0f, position.y, position.x + (strWidth + 1.0f) * cliInvSize.x * 2.0f, position.y + size.y);
    }
}

wchar_t* GUIEditW::GetText(){ return text; }
size_t GUIEditW::GetTextMaxLength(){ return DEFAULT_STRING_LENGTH; }

void GUIEditW::SetBackgroundColor(Vector3 color){ bkColor = color; }
void GUIEditW::SetFontColor(Vector3 color){ fontColor = color; }
void GUIEditW::SetSelectionColor(Vector3 color){ selColor = color; }
void GUIEditW::SetCornerRadius(float radius){ this->radius = radius; }
Vector3 GUIEditW::GetBackgroundColor(){ return bkColor; }
Vector3 GUIEditW::GetFontColor(){ return fontColor; }
Vector3 GUIEditW::GetSelectionColor(){ return selColor; }
float GUIEditW::GetCornerRadius(){ return radius; }