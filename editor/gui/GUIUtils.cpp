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

IconButton::IconButton() : AGUIObject(L"IconButton", ViewObjectType::OBJECT_GUI_ICON_BUTTON) {}
IconButton::IconButton(Vector2 position, Vector2 size) : AGUIObject(L"IconButton", ViewObjectType::OBJECT_GUI_ICON_BUTTON), position(position), size(size) {}
IconButton::IconButton(Vector2 position, Vector2 size, float radius) : AGUIObject(L"IconButton", ViewObjectType::OBJECT_GUI_ICON_BUTTON), position(position), size(size), radius(radius) {}

IconButton::~IconButton(){
    if (texture) delete texture;
}

bool IconButton::OnHit2D(Point2 pos){
    hover = pos.x >= position.x && pos.x <= position.x + size.x &&
            pos.y >= position.y && pos.y <= position.y + size.y;
    return hover;
}

void IconButton::OnLeftDown2D(Point2 pos){
    onClick();
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

GUIEditA::GUIEditA() :
    AGUIObject(L"GUIEditA", ViewObjectType::OBJECT_GUI_EDIT_A) {}
GUIEditA::GUIEditA(Vector2 pos, float width) :
    AGUIObject(L"GUIEditA", ViewObjectType::OBJECT_GUI_EDIT_A), position(pos), size(Vector2(width, 0.0f)) {}
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
        onEdit(text.ToString());
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
            onEdit(text.ToString());
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

void GUIEditA::SetText(String s){
    text.Clear();
    text += s;
}

GUIEditW::GUIEditW(Vector2 pos, float width) :
    AGUIObject(L"GUIEditW", ViewObjectType::OBJECT_GUI_EDIT_W), position(pos), size(Vector2(width, 0.0f)) {}

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
            onEdit(text.ToString());
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

WString GUIEditW::GetText(){
    return text.ToString();
}

void GUIEditW::SetText(WString s){
    text.Clear();
    text += s;
}

HorizontalProgressBar::HorizontalProgressBar() :
    AGUIObject(L"HorizontalProgressBar", ViewObjectType::OBJECT_HORIZONTAL_PROGRESSBAR) {}
HorizontalProgressBar::~HorizontalProgressBar(){}

bool HorizontalProgressBar::OnHit2D(Point2 pos){
    float btnPos = Lerp(lowBound, highBound, this->pos);
    hover = (pos.x >= btnPos - btnX &&
            pos.x <= btnPos + btnX &&
            pos.y >= posY - btnY &&
            pos.y <= posY + btnY);
    return hover;
}

void HorizontalProgressBar::OnLeftDown2D(Point2 pos){
    origin = this->pos;
}

void HorizontalProgressBar::OnLeftDrag2D(Vector2 dir){
    pos = Clamp(origin + (dir.x / (highBound - lowBound)), 0.0f, 1.0f);
    onPosChange(pos);
}

void HorizontalProgressBar::OnRender(){
    glLineWidth(lineWidth);
    glColorv3(lineColor);
    glBegin(GL_LINES);
    glVertex2f(lowBound, posY);
    glVertex2f(highBound, posY);
    glEnd();
    glLineWidth(1.0f);

    float btnPos = Lerp(lowBound, highBound, this->pos);
    glColorv3(hover ? hoverBtnColor : defaultBtnColor);
    GLUtils::DrawRect(btnPos - btnX, posY - btnY,
                        btnPos + btnX, posY + btnY);
}

VerticalProgressBar::VerticalProgressBar() :
    AGUIObject(L"VerticalProgressBar", ViewObjectType::OBJECT_VERTICAL_PROGRESSBAR) {}
VerticalProgressBar::~VerticalProgressBar(){}

bool VerticalProgressBar::OnHit2D(Point2 pos){
    float btnPos = Lerp(lowBound, highBound, this->pos);
    hover = (pos.x >= posX - btnX &&
            pos.x <= posX + btnX &&
            pos.y >= btnPos - btnY &&
            pos.y <= btnPos + btnY);
    return hover;
}

void VerticalProgressBar::OnLeftDown2D(Point2 pos){
    origin = this->pos;
}

void VerticalProgressBar::OnLeftDrag2D(Vector2 dir){
    pos = Clamp(origin + (dir.y / (highBound - lowBound)), 0.0f, 1.0f);
    onPosChange(pos);
}

void VerticalProgressBar::OnRender(){
    glLineWidth(lineWidth);
    glColorv3(lineColor);
    glBegin(GL_LINES);
    glVertex2f(posX, lowBound);
    glVertex2f(posX, highBound);
    glEnd();
    glLineWidth(1.0f);

    float btnPos = Lerp(lowBound, highBound, this->pos);
    glColorv3(hover ? hoverBtnColor : defaultBtnColor);
    GLUtils::DrawRect(pos - btnX, btnPos - btnY,
                        pos + btnX, btnPos + btnY);
}