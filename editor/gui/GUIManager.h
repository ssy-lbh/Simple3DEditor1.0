#ifndef __EDITOR_GUI_GUIMANAGER__
#define __EDITOR_GUI_GUIMANAGER__

#include <define.h>

#include <utils/List.h>
#include <utils/String.h>
#include <utils/StringBuilder.h>
#include <utils/math3d/LinearAlgebra.h>
#include <editor/main/ViewObject.h>

// 3D GUI管理器已经基本可用
class GUIManager final : public AViewObject {
private:
    AViewObject* focus = NULL;
    AViewObject* cur = NULL;
    Point3 startPos;
    Point3 camPos;
    Point3 cursorPos;
    Vector3 cursorDir;
    bool leftDown = false;
    bool rightDown = false;

protected:
    void UpdateCursor(Point3 ori, Vector3 dir);

public:
    GUIManager();
    ~GUIManager();

    virtual bool DeleteChild(AViewObject* o) override;

    virtual void OnRender() override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnMouseMove(Point3 ori, Vector3 dir) override;
    virtual void OnLeftDown(Point3 ori, Vector3 dir) override;
    virtual void OnLeftUp(Point3 ori, Vector3 dir) override;
    virtual void OnRightDown(Point3 ori, Vector3 dir) override;
    virtual void OnRightUp(Point3 ori, Vector3 dir) override;
    virtual void OnKillFocus() override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;
    virtual void OnDropFileA(const char* path) override;
    virtual void OnDropFileW(const wchar_t* path) override;

    AViewObject* GetCurrent();
    AViewObject* FindCurrent();
};

class IconButton final : public AViewObject {
private:
    Vector2 position;
    Vector2 recPos;
    Vector2 size;
    float radius;
    bool hover = false;

    GLTexture2D* texture = NULL;
    void(*onClick)(void*) = NULL;
    void* userData = NULL;

public:
    IconButton(Vector2 position, Vector2 size);
    IconButton(Vector2 position, Vector2 size, float radius);
    virtual ~IconButton() override;

    virtual bool OnHit(Point3 ori, Vector3 dir) override;
    virtual void OnLeftDown(Point3 ori, Vector3 dir) override;
    virtual void OnLeftDrag(Vector3 delta, Vector3 dir) override;
    virtual void OnRender() override;

    void OnClick(void(*func)(void*));
    void SetUserData(void* data);
    void SetIcon(const char* texPath);
    void SetIcon(int iconRes);
    void SetIcon(GLTexture2D* tex);
};

class GUIEditA final : public AViewObject {
private:
    // 左上角位置
    Vector2 position;
    Vector2 size;
    // 意为半径占高度的比例
    float radius = 0.0f;
    Vector3 bkColor = Vector3::zero;
    Vector3 fontColor = Vector3::one;
    Vector3 selColor = Vector3(0.0f, 0.0f, 1.0f);

    char text[DEFAULT_STRING_LENGTH + 1];
    bool editing = false;
    size_t editPos;

    void(*onEdit)(char*, void*) = NULL;
    void* userData = NULL;

public:
    GUIEditA(Vector2 pos, float width);
    GUIEditA(Vector2 pos, float width, void(*onEdit)(char*, void*));
    GUIEditA(Vector2 pos, float width, void(*onEdit)(char*, void*), void* userData);
    virtual ~GUIEditA();

    virtual bool OnHit(Point3 ori, Vector3 dir) override;
    virtual void OnMouseMove(Point3 ori, Vector3 dir) override;
    virtual void OnLeftDown(Point3 ori, Vector3 dir) override;
    virtual void OnKillFocus() override;
    virtual void OnChar(char c) override;
    virtual void OnRender() override;

    char* GetText();
    size_t GetTextMaxLength();

    void SetBackgroundColor(Vector3 color);
    void SetFontColor(Vector3 color);
    void SetSelectionColor(Vector3 color);
    void SetCornerRadius(float radius);
    Vector3 GetBackgroundColor();
    Vector3 GetFontColor();
    Vector3 GetSelectionColor();
    float GetCornerRadius();
};

class GUIEditW final : public AViewObject {
private:
    // 左上角位置
    Vector2 position;
    Vector2 size;
    // 意为半径占高度的比例
    float radius = 0.0f;
    Vector3 bkColor = Vector3::zero;
    Vector3 fontColor = Vector3::one;
    Vector3 selColor = Vector3(0.0f, 0.0f, 1.0f);

    wchar_t text[DEFAULT_STRING_LENGTH + 1];
    bool editing = false;
    size_t editPos;

    void(*onEdit)(wchar_t*, void*) = NULL;
    void* userData = NULL;

public:
    GUIEditW(Vector2 pos, float width);
    GUIEditW(Vector2 pos, float width, void(*onEdit)(wchar_t*, void*));
    GUIEditW(Vector2 pos, float width, void(*onEdit)(wchar_t*, void*), void* userData);
    virtual ~GUIEditW();

    virtual bool OnHit(Point3 ori, Vector3 dir) override;
    virtual void OnMouseMove(Point3 ori, Vector3 dir) override;
    virtual void OnLeftDown(Point3 ori, Vector3 dir) override;
    virtual void OnKillFocus() override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnRender() override;

    wchar_t* GetText();
    size_t GetTextMaxLength();

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