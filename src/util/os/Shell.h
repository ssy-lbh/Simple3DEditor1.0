#ifndef __UTIL_OS_SHELL__
#define __UTIL_OS_SHELL__

#include <define.h>

namespace simple3deditor {

#ifdef PLATFORM_WINDOWS
#define MSGBOX_YES 6
#define MSGBOX_NO 7
#define MSGBOX_CANCEL 2

#define FILESELECT_REQ_PATH 0x800
#define FILESELECT_REQ_FILE 0x1000
#define FILESELECT_MULTIPLE 0x200
#endif

enum class ShaderType {
    SPIR_V,
    GLSL,
    HLSL,
    VULKAN_GLSL,
    MSL
};

String ShellFileSelectWindow(const String filter, int flags, bool save = false);
WString ShellFileSelectWindow(const WString filter, int flags, bool save = false);
bool ShellCommandLineArgs(const String s, const String args);
bool ShellCommandLineArgs(const WString s, const WString args);
bool ShellCommandLine(const String s);
bool ShellCommandLine(const WString s);
bool ShellCheckFileExistence(const String file, size_t times, size_t interval);
bool ShellCheckFileExistence(const WString file, size_t times, size_t interval);
bool ShellFFmpeg(const String src, const String dst);
bool ShellFFmpeg(const WString src, const WString dst);
bool ShellShaderToBinary(const String src, const String dst, ShaderType type);
bool ShellShaderToBinary(const WString src, const WString dst, ShaderType type);
bool ShellBinaryToShader(const String src, const String dst, ShaderType type);
bool ShellBinaryToShader(const WString src, const WString dst, ShaderType type);
int ShellMsgBox(const String caption, const String text);
int ShellMsgBox(const WString caption, const WString text);
bool ShellPrint(const String file);
bool ShellPrint(const WString file);

String GetAppDirectoryA();
WString GetAppDirectoryW();

}

#endif