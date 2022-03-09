#include <editor/dialog/Tips.h>

#include <windows.h>

#include <res.h>
#include <utils/os/Log.h>
#include <utils/os/System.h>

/* synchronized */
void DialogTextInput(wchar_t* str, size_t size){
    static volatile bool lock = false;
    static wchar_t* pstr;
    static size_t ssize;
    static bool inputConfirm;

    // 自旋锁(听说D语言不允许空的循环体，也许可以这样水一水)
    while(__atomic_test_and_set(&lock, true))
        continue;

    pstr = str;
    ssize = size;
    inputConfirm = false;
    DialogBoxA(GetModule(), MAKEINTRESOURCE(IDD_TEXT), NULL,
        (DLGPROC)[]
#if (!_WIN64) && _WIN32
        __attribute__((__stdcall__))
#endif
        (HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
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
    });
    if (!inputConfirm)
        str[0] = L'\0';
    lock = false;
}

void DialogVersionInfo(){
    DialogBoxA(GetModule(), MAKEINTRESOURCEA(IDD_ABOUTBOX), NULL,
        (DLGPROC)[]
#if (!_WIN64) && _WIN32
        __attribute__((__stdcall__))
#endif
        (HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
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
    });
}