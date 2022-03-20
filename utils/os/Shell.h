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

String ShellFileSelectWindow(const String filter, int flags, bool save = false);
WString ShellFileSelectWindow(const WString filter, int flags, bool save = false);
bool ShellCommandLine(const String s);
bool ShellCommandLine(const WString s);
bool ShellFFmpeg(const String src, const String dst);
bool ShellFFmpeg(const WString src, const WString dst);
int ShellMsgBox(const String caption, const String text);
int ShellMsgBox(const WString caption, const WString text);
bool ShellPrint(const String file);
bool ShellPrint(const WString file);

String GetAppDirectoryA();
WString GetAppDirectoryW();

#endif