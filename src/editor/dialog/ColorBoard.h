#ifndef __EDITOR_DIALOG_COLORBOARD__
#define __EDITOR_DIALOG_COLORBOARD__

#include <windows.h>
#include <windowsx.h>

#include <util/gl/GLUtils.h>
#include <util/math3d/LinearAlgebra.h>

namespace simple3deditor {

class ColorBoard final : public Object {
private:
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;

    static void Init(HINSTANCE hInstance);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK LocalWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    Vector3 color;
    Vector2 cursorPos;
    Vector2 cliSize;
    float white = 1.0f;

    static ColorBoard* inst;

    void Render();

public:
    ColorBoard();
    ~ColorBoard();
    
    static ColorBoard* GetInst();
    static Vector3 GetColor();
    Vector3 RunAndGetColor();

    void UpdateWindowSize(int x, int y);
    void UpdateCursor(int x, int y);
};

}

#endif