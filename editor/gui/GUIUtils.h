#ifndef __EDITOR_GUI_GUIUTILS__
#define __EDITOR_GUI_GUIUTILS__

#include <define.h>

#include <utils/List.h>
#include <utils/String.h>
#include <utils/StringBuilder.h>
#include <utils/Delegate.h>
#include <utils/math3d/LinearAlgebra.h>
#include <editor/object/GUIObject.h>

// 看来GUI组件的属性过多，而且大都不为指针，所以大部分属性不要过度封装，直接公开即可
class IconButton final : public AGUIObject {
private:
    Vector2 startPos;
    bool hover = false;

    GLTexture2D* texture = NULL;

public:
    Vector2 position;
    Vector2 size;
    float radius;
    bool moveable = false;

    DelegateWithData<void, void*> onClick;

    IconButton();
    IconButton(Vector2 position, Vector2 size);
    IconButton(Vector2 position, Vector2 size, float radius);
    virtual ~IconButton() override;

    virtual bool OnHit2D(Point2 pos) override;
    virtual void OnLeftDown2D(Point2 pos) override;
    virtual void OnLeftDrag2D(Vector2 dir) override;
    virtual void OnRender() override;

    void SetIcon(const char* texPath);
    void SetIcon(int iconRes);
    void SetIcon(GLTexture2D* tex);
};

class GUIEditA final : public AGUIObject {
private:
    StringBuilderA text;
    bool editing = false;

public:
    // 左上角位置
    Vector2 position;
    Vector2 size;
    // 意为半径占高度的比例
    float radius = 0.0f;
    Vector3 bkColor = Vector3::zero;
    Vector3 fontColor = Vector3::one;
    Vector3 selColor = Vector3(0.0f, 0.0f, 1.0f);

    DelegateWithData<void, void*, String> onEdit;

    GUIEditA();
    GUIEditA(Vector2 pos, float width);
    virtual ~GUIEditA();

    virtual bool OnHit2D(Point2 pos) override;
    virtual void OnMouseMove2D(Point2 pos) override;
    virtual void OnLeftDown2D(Point2 pos) override;
    virtual void OnKillFocus() override;
    virtual void OnChar(char c) override;
    virtual void OnRender() override;

    String GetText();
    void SetText(String s);
};

class GUIEditW final : public AGUIObject {
private:
    StringBuilderW text;
    bool editing = false;

public:
    // 左上角位置
    Vector2 position;
    Vector2 size;
    // 意为半径占高度的比例
    float radius = 0.0f;
    Vector3 bkColor = Vector3::zero;
    Vector3 fontColor = Vector3::one;
    Vector3 selColor = Vector3(0.0f, 0.0f, 1.0f);

    DelegateWithData<void, void*, WString> onEdit;

    GUIEditW();
    GUIEditW(Vector2 pos, float width);
    virtual ~GUIEditW();

    virtual bool OnHit2D(Point2 pos) override;
    virtual void OnMouseMove2D(Point2 pos) override;
    virtual void OnLeftDown2D(Point2 pos) override;
    virtual void OnKillFocus() override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnRender() override;

    WString GetText();
    void SetText(WString s);
};

class HorizontalProgressBar : public AGUIObject {
private:
    bool hover = false;
    float origin;

public:
    float pos = 0.0f;
    float lowBound;
    float highBound;
    float posY;
    float btnX;
    float btnY;
    float lineWidth = 10.0f;
    Vector3 lineColor = Vector3(0.6f, 0.6f, 0.6f);
    Vector3 defaultBtnColor = Vector3(0.0f, 0.0f, 0.5f);
    Vector3 hoverBtnColor = Vector3(0.0f, 0.0f, 0.3f);

    DelegateWithData<void, void*, float> onPosChange;

    HorizontalProgressBar();
    virtual ~HorizontalProgressBar() override;

    virtual bool OnHit2D(Point2 pos) override;
    virtual void OnLeftDown2D(Point2 pos) override;
    virtual void OnLeftDrag2D(Vector2 dir) override;
    virtual void OnRender() override;
};

class VerticalProgressBar : public AGUIObject {
private:
    bool hover = false;
    float origin;

public:
    float pos = 0.0f;
    float lowBound;
    float highBound;
    float posX;
    float btnX;
    float btnY;
    float lineWidth = 10.0f;
    Vector3 lineColor = Vector3(0.6f, 0.6f, 0.6f);
    Vector3 defaultBtnColor = Vector3(0.0f, 0.0f, 0.5f);
    Vector3 hoverBtnColor = Vector3(0.0f, 0.0f, 0.3f);

    DelegateWithData<void, void*, float> onPosChange;

    VerticalProgressBar();
    virtual ~VerticalProgressBar() override;

    virtual bool OnHit2D(Point2 pos) override;
    virtual void OnLeftDown2D(Point2 pos) override;
    virtual void OnLeftDrag2D(Vector2 dir) override;
    virtual void OnRender() override;
};

#endif