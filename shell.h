#ifndef __SHELL__
#define __SHELL__

#include "define.h"

#include <windef.h>

String ShellFileSelectWindow(String filter, DWORD flags);
WString ShellFileSelectWindow(WString filter, DWORD flags);
bool ShellCommandLine(String s);
bool ShellCommandLine(WString s);
bool ShellFFmpeg(String src, String dst);
bool ShellFFmpeg(WString src, WString dst);

#endif