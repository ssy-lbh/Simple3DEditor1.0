#ifndef __EDITOR_GUI_MENU__
#define __EDITOR_GUI_MENU__

#include <define.h>

#include <util/List.h>
#include <util/Value.h>
#include <util/Delegate.h>
#include <util/math3d/LinearAlgebra.h>

namespace simple3deditor {

enum class MenuItemType {
    DEFAULT,
    GROUP,
    SEPERATOR
};

class MenuItem : public Object {
public:
    MenuItem();
    MenuItem(Value<const wchar_t*>&& name);
    MenuItem(Value<const wchar_t*>&& name, std::function<void()>&& click);
    MenuItem(Value<const wchar_t*>&& name, Menu* menu);
    virtual ~MenuItem();

    Value<MenuItemType> type;
    Value<const wchar_t*> name;
    Value<Menu*> menu;

    Delegate<> onClick;
    Delegate<MenuItem*> onDestory;
};

// �?以继承以实现多�?�样式的菜单
class Menu : public Object {
protected:
    static float WIDTH_PIXELS;
    static float CORNER_PIXELS;
    static float LINE_PIXELS;

    List<MenuItem*> items;

    Vector2 renderPos;
    Vector2 minPos;
    size_t drawCounter;
    size_t selected = -1;

    Vector2 cliSize, cliInvSize;
    Vector2 cursorPos;

    Menu* curMenu = NULL;
    Vector2 menuPos;
    bool openMenu = false;
    bool rightPressed = false;

    friend class GlobalData;

    void SetMenu(Menu* m, size_t pos);
    
public:
    Menu();
    ~Menu();

    void AddItem(MenuItem* item);
    void SetClientSize(Vector2 size);
    bool InMenu(Vector2 relaPos);
    bool InChainMenu(Vector2 relaPos);
    void CursorMove(Vector2 relaPos);
    void Click();
    void RenderItem(MenuItem* item);
    void Render(Vector2 pos);
    void Render();
    void ResetSelect();

    void PressUp();
    void PressDown();
    void PressLeft();
    void PressRight();
};

}

#endif