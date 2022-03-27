#include <editor/gui/GUIUtils.h>

#include <lib/glew/gl/glew.h>

#include <exception>

#include <main.h>
#include <utils/gl/GLSimplified.h>
#include <utils/gl/GLUtils.h>
#include <utils/gl/GLTexture2D.h>
#include <utils/os/Log.h>
#include <utils/os/Font.h>
#include <editor/gui/ViewManager.h>

IconButton::IconButton(Vector2 position, Vector2 size) : AGUIObject(L"IconButton", ViewObjectType::OBJECT_GUI_ICON_BUTTON), position(position), size(size) {}
IconButton::IconButton(Vector2 position, Vector2 size, float radius) : AGUIObject(L"IconButton", ViewObjectType::OBJECT_GUI_ICON_BUTTON), position(position), size(size), radius(radius) {}

IconButton::~IconButton(){
    if (texture) delete texture;
}

bool IconButton::OnHit2D(Point2 pos){
    hover = pos.x >= position.x && pos.x <= position.x + size.x && pos.y >= position.y && pos.y <= position.y + size.y;
    return hover;
}

void IconButton::OnLeftDown2D(Point2 pos){
    if (onClick)
        onClick(userData);
    startPos = position;
}

void IconButton::OnLeftDrag2D(Vector2 dir){
    if (moveable)
        position = startPos + dir;
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

bool IconButton::IsMoveable(){
    return moveable;
}

void IconButton::SetMoveable(bool moveable){
    this->moveable = moveable;
}

GUIEditA::GUIEditA(Vector2 pos, float width) :
    AGUIObject(L"GUIEditA", ViewObjectType::OBJECT_GUI_EDIT_A), position(pos), size(Vector2(width, 0.0f)) {}
GUIEditA::GUIEditA(Vector2 pos, float width, void(*onEdit)(String, void*)) :
    AGUIObject(L"GUIEditA", ViewObjectType::OBJECT_GUI_EDIT_A), position(pos), size(Vector2(width, 0.0f)), onEdit(onEdit) {}
GUIEditA::GUIEditA(Vector2 pos, float width, void(*onEdit)(String, void*), void* userData) :
    AGUIObject(L"GUIEditA", ViewObjectType::OBJECT_GUI_EDIT_A), position(pos), size(Vector2(width, 0.0f)), onEdit(onEdit), userData(userData) {}
GUIEditA::~GUIEditA(){}

bool GUIEditA::OnHit2D(Point2 pos){
    return pos.x >= position.x && pos.x <= position.x + size.x
            && pos.y >= position.y && pos.y <= position.y + size.y;
}

void GUIEditA::OnMouseMove2D(Point2 pos){
    Main::SetCursor(IDC_IBEAM);
}

void GUIEditA::OnLeftDown2D(Point2 pos){
    Main::SetCursor(IDC_IBEAM);
    editing = true;
}

void GUIEditA::OnKillFocus(){
    if (editing){
        editing = false;
        if (onEdit)
            onEdit(text.ToString(), userData);
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
                onEdit(text.ToString(), userData);
        }
        return;
    case '\b':
        if (text.GetLength() == 0)
            return;
        text.SetLength(text.GetLength() - 1);
        return;
    }
    text += c;
    return;
}

void GUIEditA::OnRender(){
    ViewManager* viewMgr = ViewManager::GetLocalInst();

    Vector2 cliSize = viewMgr->GetSize();
    Vector2 cliInvSize = Vector2(1.0f / cliSize.x, 1.0f / cliSize.y);

    size.y = cliInvSize.y * 30.0f;

    glColor3f(bkColor.x, bkColor.y, bkColor.z);
    if (radius == 0.0f){
        GLUtils::DrawRect(position, position + size);
    }else{
        GLUtils::DrawRoundRect(Rect(position, position + size), radius * size.y, 0.05f);
    }

    glColor3f(fontColor.x, fontColor.y, fontColor.z);
    glRasterPos2f(position.x, position.y + 6.0f * cliInvSize.y);
    glDrawCNString(text.GetString());

    if (editing){
        float strWidth = glGetStringWidth(text.GetString());
        glColor3f(1.0f, 1.0f, 1.0f);
        GLUtils::DrawRect(position.x + (strWidth - 1.0f) * cliInvSize.x * 2.0f, position.y, position.x + (strWidth + 1.0f) * cliInvSize.x * 2.0f, position.y + size.y);
    }
}

String GUIEditA::GetText(){
    return text.ToString();
}

void GUIEditA::SetBackgroundColor(Vector3 color){ bkColor = color; }
void GUIEditA::SetFontColor(Vector3 color){ fontColor = color; }
void GUIEditA::SetSelectionColor(Vector3 color){ selColor = color; }
void GUIEditA::SetCornerRadius(float radius){ this->radius = radius; }
Vector3 GUIEditA::GetBackgroundColor(){ return bkColor; }
Vector3 GUIEditA::GetFontColor(){ return fontColor; }
Vector3 GUIEditA::GetSelectionColor(){ return selColor; }
float GUIEditA::GetCornerRadius(){ return radius; }

GUIEditW::GUIEditW(Vector2 pos, float width) :
    AGUIObject(L"GUIEditW", ViewObjectType::OBJECT_GUI_EDIT_W), position(pos), size(Vector2(width, 0.0f)) {}
GUIEditW::GUIEditW(Vector2 pos, float width, void(*onEdit)(WString, void*)) :
    AGUIObject(L"GUIEditW", ViewObjectType::OBJECT_GUI_EDIT_W), position(pos), size(Vector2(width, 0.0f)), onEdit(onEdit) {}
GUIEditW::GUIEditW(Vector2 pos, float width, void(*onEdit)(WString, void*), void* userData) :
    AGUIObject(L"GUIEditW", ViewObjectType::OBJECT_GUI_EDIT_W), position(pos), size(Vector2(width, 0.0f)), onEdit(onEdit), userData(userData) {}

GUIEditW::~GUIEditW(){}

bool GUIEditW::OnHit2D(Point2 pos){
    return pos.x >= position.x && pos.x <= position.x + size.x
            && pos.y >= position.y && pos.y <= position.y + size.y;
}

void GUIEditW::OnMouseMove2D(Point2 pos){
    Main::SetCursor(IDC_IBEAM);
}

void GUIEditW::OnLeftDown2D(Point2 pos){
    Main::SetCursor(IDC_IBEAM);
    editing = true;
}

void GUIEditW::OnKillFocus(){
    editing = false;
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
                onEdit(text.ToString(), userData);
        }
        return;
    case L'\b':
        if (text.GetLength() == 0)
            return;
        text.SetLength(text.GetLength() - 1);
        return;
    }
    text += c;
    return;
}

void GUIEditW::OnRender(){
    ViewManager* viewMgr = ViewManager::GetLocalInst();

    Vector2 cliSize = viewMgr->GetSize();
    Vector2 cliInvSize = Vector2(1.0f / cliSize.x, 1.0f / cliSize.y);

    size.y = cliInvSize.y * 30.0f;

    glColor3f(bkColor.x, bkColor.y, bkColor.z);
    if (radius == 0.0f){
        GLUtils::DrawRect(position, position + size);
    }else{
        GLUtils::DrawRoundRect(Rect(position, position + size), radius * size.y, 0.05f);
    }

    glColor3f(fontColor.x, fontColor.y, fontColor.z);
    glRasterPos2f(position.x, position.y + 6.0f * cliInvSize.y);
    glDrawCNString(text.GetString());

    if (editing){
        float strWidth = glGetCNStringWidth(text.GetString());
        glColor3f(1.0f, 1.0f, 1.0f);
        GLUtils::DrawRect(position.x + (strWidth - 1.0f) * cliInvSize.x * 2.0f, position.y, position.x + (strWidth + 1.0f) * cliInvSize.x * 2.0f, position.y + size.y);
    }
}

WString GUIEditW::GetText(){ return text.ToString(); }

void GUIEditW::SetBackgroundColor(Vector3 color){ bkColor = color; }
void GUIEditW::SetFontColor(Vector3 color){ fontColor = color; }
void GUIEditW::SetSelectionColor(Vector3 color){ selColor = color; }
void GUIEditW::SetCornerRadius(float radius){ this->radius = radius; }
Vector3 GUIEditW::GetBackgroundColor(){ return bkColor; }
Vector3 GUIEditW::GetFontColor(){ return fontColor; }
Vector3 GUIEditW::GetSelectionColor(){ return selColor; }
float GUIEditW::GetCornerRadius(){ return radius; }