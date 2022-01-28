#ifndef __COLORBOARD__
#define __COLORBOARD__

#include "vecmath.h"

#include <windows.h>
#include <windowsx.h>

#include <gl/gl.h>

class ColorBoard;

class ColorBoard {
private:
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;
    
    Vector3 color;
    Vector2 cursorPos;
    RECT cliRect;
    float white = 1.0f;

public:
    ColorBoard();
    ~ColorBoard();
    static ATOM RegClass(HINSTANCE hInstance);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK LocalWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void Render();
    Vector3 RunAndGetColor();
};

#endif