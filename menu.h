#ifndef __MENU__
#define __MENU__

#include <gl/gl.h>

#include "vecmath.h"
#include "list.h"

#define MENUITEM_LAMBDA_TRANS(t) (void(*)(void*))(void(*)(t*))

interface IMenuItem : public Object {
public:
    enum class ItemType {
        DEFAULT,
        GROUP,
        SEPERATOR
    };

    IMenuItem();
    virtual ~IMenuItem();

    virtual IMenuItem::ItemType GetType();
    virtual const wchar_t* GetName();
    virtual Menu* GetMenu();

    virtual void OnClick();
};

class MenuItem final : public IMenuItem {
public:
    ItemType type;
    const wchar_t* name = NULL;
    Menu* menu = NULL;

    void(*click)(void*) = NULL;
    void* userData = NULL;

    MenuItem();
    MenuItem(const wchar_t* name);
    MenuItem(const wchar_t* name, void(*click)(void*));
    MenuItem(const wchar_t* name, void(*click)(void*), void* userData);
    MenuItem(const wchar_t* name, Menu* menu);
    virtual ~MenuItem() override;

    virtual IMenuItem::ItemType GetType() override;
    virtual const wchar_t* GetName() override;
    virtual Menu* GetMenu() override;

    virtual void OnClick() override;
};

class SwitchMenuItem final : public IMenuItem {
public:
    const wchar_t* nameOn = NULL;
    const wchar_t* nameOff = NULL;

    bool state = false;

    void(*click)(bool, void*) = NULL;
    void* userData = NULL;

    SwitchMenuItem(const wchar_t* nameOn, const wchar_t* nameOff);
    SwitchMenuItem(const wchar_t* nameOn, const wchar_t* nameOff, void(*click)(bool, void*));
    SwitchMenuItem(const wchar_t* nameOn, const wchar_t* nameOff, void(*click)(bool, void*), void* userData);
    virtual ~SwitchMenuItem() override;

    virtual IMenuItem::ItemType GetType() override;
    virtual const wchar_t* GetName() override;

    virtual void OnClick() override;
};

// 可以继承以实现多种样式的菜单
class Menu : public Object {
protected:
    List<IMenuItem*> items;

    Vector2 minPos;
    size_t drawCounter;
    size_t selected = -1;

    Vector2 cliSize, cliInvSize;
    Vector2 cursorPos;

    Menu* curMenu = NULL;
    Vector2 menuPos;
    
public:
    Menu();
    ~Menu();
    void AddItem(IMenuItem* item);
    void SetClientSize(Vector2 size);
    bool InMenu(Vector2 relaPos);
    bool InChainMenu(Vector2 relaPos);
    void CursorMove(Vector2 relaPos);
    void Click();
    void RenderItem(IMenuItem* item);
    void Render(float x, float y);
    void ResetSelect();
};

#endif