#ifndef __COLORBOARD__
#define __COLORBOARD__

#include "vecmath.h"

#include <windows.h>
#include <windowsx.h>

#include <gl/gl.h>

class ColorBoard : public Object {
private:
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;
    
    Vector3 color;
    Vector2 cursorPos;
    RECT cliRect;
    float white = 1.0f;

    static ColorBoard* inst;

public:
    ColorBoard();
    ~ColorBoard();
    static void Init(HINSTANCE hInstance);
    static ColorBoard* GetInst();
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK LocalWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void Render();
    static Vector3 GetColor();
    Vector3 RunAndGetColor();
};

#endif