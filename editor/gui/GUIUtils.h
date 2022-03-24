#ifndef __EDITOR_GUI_GUIUTILS__
#define __EDITOR_GUI_GUIUTILS__

#include <define.h>

#include <utils/List.h>
#include <utils/String.h>
#include <utils/StringBuilder.h>
#include <utils/math3d/LinearAlgebra.h>
#include <editor/object/GUIObject.h>

class IconButton final : public AGUIObject {
private:
    Vector2 position;
    Vector2 startPos;
    Vector2 size;
    float radius;
    bool hover = false;

    GLTexture2D* texture = NULL;
    bool moveable = false;
    void(*onClick)(void*) = NULL;
    void* userData = NULL;

public:
    IconButton(Vector2 position, Vector2 size);
    IconButton(Vector2 position, Vector2 size, float radius);
    virtual ~IconButton() override;

    virtual bool OnHit2D(Point2 pos) override;
    virtual void OnLeftDown2D(Point2 pos) override;
    virtual void OnLeftDrag2D(Vector2 dir) override;
    virtual void OnRender() override;

    void OnClick(void(*func)(void*));
    void SetUserData(void* data);
    void SetIcon(const char* texPath);
    void SetIcon(int iconRes);
    void SetIcon(GLTexture2D* tex);
    bool IsMoveable();
    void SetMoveable(bool moveable);
};

class GUIEditA final : public AGUIObject {
private:
    // 左上角位置
    Vector2 position;
    Vector2 size;
    // 意为半径占高度的比例
    float radius = 0.0f;
    Vector3 bkColor = Vector3::zero;
    Vector3 fontColor = Vector3::one;
    Vector3 selColor = Vector3(0.0f, 0.0f, 1.0f);

    StringBuilderA text;
    bool editing = false;

    void(*onEdit)(String, void*) = NULL;
    void* userData = NULL;

public:
    GUIEditA(Vector2 pos, float width);
    GUIEditA(Vector2 pos, float width, void(*onEdit)(String, void*));
    GUIEditA(Vector2 pos, float width, void(*onEdit)(String, void*), void* userData);
    virtual ~GUIEditA();

    virtual bool OnHit2D(Point2 pos) override;
    virtual void OnMouseMove2D(Point2 pos) override;
    virtual void OnLeftDown2D(Point2 pos) override;
    virtual void OnKillFocus() override;
    virtual void OnChar(char c) override;
    virtual void OnRender() override;

    String GetText();

    void SetBackgroundColor(Vector3 color);
    void SetFontColor(Vector3 color);
    void SetSelectionColor(Vector3 color);
    void SetCornerRadius(float radius);
    Vector3 GetBackgroundColor();
    Vector3 GetFontColor();
    Vector3 GetSelectionColor();
    float GetCornerRadius();
};

class GUIEditW final : public AGUIObject {
private:
    // 左上角位置
    Vector2 position;
    Vector2 size;
    // 意为半径占高度的比例
    float radius = 0.0f;
    Vector3 bkColor = Vector3::zero;
    Vector3 fontColor = Vector3::one;
    Vector3 selColor = Vector3(0.0f, 0.0f, 1.0f);

    StringBuilderW text;
    bool editing = false;

    void(*onEdit)(WString, void*) = NULL;
    void* userData = NULL;

public:
    GUIEditW(Vector2 pos, float width);
    GUIEditW(Vector2 pos, float width, void(*onEdit)(WString, void*));
    GUIEditW(Vector2 pos, float width, void(*onEdit)(WString, void*), void* userData);
    virtual ~GUIEditW();

    virtual bool OnHit2D(Point2 pos) override;
    virtual void OnMouseMove2D(Point2 pos) override;
    virtual void OnLeftDown2D(Point2 pos) override;
    virtual void OnKillFocus() override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnRender() override;

    WString GetText();

    void SetBackgroundColor(Vector3 color);
    void SetFontColor(Vector3 color);
    void SetSelectionColor(Vector3 color);
    void SetCornerRadius(float radius);
    Vector3 GetBackgroundColor();
    Vector3 GetFontColor();
    Vector3 GetSelectionColor();
    float GetCornerRadius();
};

#endif