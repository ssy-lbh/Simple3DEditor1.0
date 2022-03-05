#ifndef __UTILS_OS_SHELL__
#define __UTILS_OS_SHELL__

#include <define.h>

#ifdef PLATFORM_WINDOWS
#define MSGBOX_YES 6
#define MSGBOX_NO 7
#define MSGBOX_CANCEL 2

#define FILESELECT_REQ_PATH 0x800
#define FILESELECT_REQ_FILE 0x1000
#define FILESELECT_MULTIPLE 0x200
#endif

String ShellFileSelectWindow(String filter, int flags, bool save = false);
WString ShellFileSelectWindow(WString filter, int flags, bool save = false);
bool ShellCommandLine(String s);
bool ShellCommandLine(WString s);
bool ShellFFmpeg(String src, String dst);
bool ShellFFmpeg(WString src, WString dst);
int ShellMsgBox(String caption, String text);
int ShellMsgBox(WString caption, WString text);
bool ShellPrint(String file);
bool ShellPrint(WString file);

String GetAppDirectoryA();
WString GetAppDirectoryW();

#endif