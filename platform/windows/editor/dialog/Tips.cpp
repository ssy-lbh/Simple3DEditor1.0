#include <editor/dialog/Tips.h>

#include <res.h>
#include <util/os/Log.h>
#include <util/os/System.h>

#include <atomic>

#include <windows.h>

namespace simple3deditor {

static volatile std::atomic_flag lock;
static wchar_t* pstr;
static size_t ssize;
static bool inputConfirm;

static INT_PTR __stdcall DlgTextInputCallback(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg){
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_CLOSE:
        EndDialog(hDlg, LOWORD(wParam));
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)){
        case IDOK:
            inputConfirm = true;
            GetDlgItemTextW(hDlg, IDC_TEXT_EDIT, pstr, ssize);
            EndDialog(hDlg, LOWORD(wParam));
            break;
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            break;
        }
        return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}

/* synchronized */
void DialogTextInput(wchar_t* str, size_t size){
    // 自旋锁(听说D语言不允许空的循环体，也许可以这样水一水)
    while(std::atomic_flag_test_and_set(&lock))
        continue;

    pstr = str;
    ssize = size;
    inputConfirm = false;
    DialogBox(GetModule(), MAKEINTRESOURCE(IDD_TEXT), NULL, DlgTextInputCallback);
    if (!inputConfirm)
        str[0] = L'\0';
    std::atomic_flag_clear(&lock);
}

static INT_PTR __stdcall DlgVersionInfoCallback(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg){
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL){
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void DialogVersionInfo(){
    DialogBoxA(GetModule(), MAKEINTRESOURCEA(IDD_ABOUTBOX), NULL, DlgVersionInfoCallback);
}

}