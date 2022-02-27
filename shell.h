#ifndef __SHELL__
#define __SHELL__

#include "define.h"

#ifdef PLATFORM_WINDOWS
#include <windef.h>
#define MSGBOX_YES IDYES
#define MSGBOX_NO IDNO
#define MSGBOX_CANCEL IDCANCEL
#endif

String ShellFileSelectWindow(String filter, int flags);
WString ShellFileSelectWindow(WString filter, int flags);
bool ShellCommandLine(String s);
bool ShellCommandLine(WString s);
bool ShellFFmpeg(String src, String dst);
bool ShellFFmpeg(WString src, WString dst);
int ShellMsgBox(String caption, String text);
int ShellMsgBox(WString caption, WString text);

#endif