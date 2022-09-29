#ifndef __EDITOR_GUI_GUIUTILS__
#define __EDITOR_GUI_GUIUTILS__

#include <define.h>

#include <util/List.h>
#include <util/String.h>
#include <util/StringBuilder.h>
#include <util/Delegate.h>
#include <util/math3d/LinearAlgebra.h>
#include <editor/object/GUIObject.h>

namespace simple3deditor {

// 看来GUI组件的属性过多，而且大都不为指针，所以大部分属性不要过度封装，直接公开即可

class RoundButton : public AGUIObject {
protected:
    Point2 startPos;
    bool hover = false;

    GLTexture2D* texture = NULL;

    RoundButton();

public:
    Point2 center = Point2::zero;
    float radius = 0.0f;
    bool moveable = false;
    Vector3 hoverColor = Vector3(1.0f, 1.0f, 1.0f);
    Vector3 defaultColor = Vector3(0.8f, 0.8f, 0.8f);

    Delegate<> onClick;
    Delegate<Vector2> onDrag;

    RoundButton(Point2 center, float radius);
    virtual ~RoundButton() override;

    virtual bool OnHit2D(Point2 pos) override;
    virtual void OnLeftDown2D(Point2 pos) override;
    virtual void OnLeftDrag2D(Vector2 dir) override;
    virtual void OnRender() override;

    void SetIcon(const char* texPath);
    void SetIcon(int iconRes);
    void SetIcon(GLTexture2D* tex);
};

class IconButton : public AGUIObject {
protected:
    Vector2 startPos;
    bool hover = false;

    GLTexture2D* texture = NULL;

public:
    Vector2 position = Vector2::zero;
    Vector2 size = Vector2::one;
    float radius = 0.0f;
    bool moveable = false;
    Vector3 hoverColor = Vector3(1.0f, 1.0f, 1.0f);
    Vector3 defaultColor = Vector3(0.8f, 0.8f, 0.8f);

    Delegate<> onClick;
    Delegate<Vector2> onDrag;

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

class GUIEditA : public AGUIObject {
protected:
    StringBuilderA text;
    bool editing = false;

public:
    // 左下角位置
    Vector2 position = Vector2::zero;
    Vector2 size = Vector2::one;
    float radius = 0.0f;
    Vector3 bkColor = Vector3::zero;
    Vector3 fontColor = Vector3::one;
    Vector3 selColor = Vector3(0.0f, 0.0f, 1.0f);

    Delegate<const String&> onEdit;

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

class GUIEditW : public AGUIObject {
protected:
    StringBuilderW text;
    bool editing = false;

public:
    // 左下角位置
    Vector2 position = Vector2::zero;
    Vector2 size = Vector2::one;
    float radius = 0.0f;
    Vector3 bkColor = Vector3::zero;
    Vector3 fontColor = Vector3::one;
    Vector3 selColor = Vector3(0.0f, 0.0f, 1.0f);

    Delegate<const WString&> onEdit;

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
protected:
    bool hover = false;
    float origin;

public:
    float pos = 0.0f;
    float lowBound = -1.0f;
    float highBound = 1.0f;
    float posY = 0.0f;
    float btnX = 0.1f;
    float btnY = 0.1f;
    float lineWidth = 10.0f;
    Vector3 lineColor = Vector3(0.6f, 0.6f, 0.6f);
    Vector3 defaultBtnColor = Vector3(0.0f, 0.0f, 0.5f);
    Vector3 hoverBtnColor = Vector3(0.0f, 0.0f, 0.3f);

    Delegate<float> onPosChange;

    HorizontalProgressBar();
    virtual ~HorizontalProgressBar() override;

    virtual bool OnHit2D(Point2 pos) override;
    virtual void OnLeftDown2D(Point2 pos) override;
    virtual void OnLeftDrag2D(Vector2 dir) override;
    virtual void OnRender() override;
};

class VerticalProgressBar : public AGUIObject {
protected:
    bool hover = false;
    float origin;

public:
    float pos = 0.0f;
    float lowBound = -1.0f;
    float highBound = 1.0f;
    float posX = 0.0f;
    float btnX = 0.1f;
    float btnY = 0.1f;
    float lineWidth = 10.0f;
    Vector3 lineColor = Vector3(0.6f, 0.6f, 0.6f);
    Vector3 defaultBtnColor = Vector3(0.0f, 0.0f, 0.5f);
    Vector3 hoverBtnColor = Vector3(0.0f, 0.0f, 0.3f);

    Delegate<float> onPosChange;

    VerticalProgressBar();
    virtual ~VerticalProgressBar() override;

    virtual bool OnHit2D(Point2 pos) override;
    virtual void OnLeftDown2D(Point2 pos) override;
    virtual void OnLeftDrag2D(Vector2 dir) override;
    virtual void OnRender() override;
};

}

#endif