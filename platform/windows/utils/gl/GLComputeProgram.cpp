#include <utils/gl/GLComputeProgram.h>

#include <windows.h>

#include <res.h>
#include <lib/opengl/gl/gl.h>
#include <utils/os/Log.h>
#include <utils/os/GLFunc.h>

GLComputeProgram::GLComputeProgram(int resid){
    HINSTANCE hInst = GetModuleHandleA(NULL);
    HRSRC kernelSrc;
    HGLOBAL resIdx;
    LPVOID resPtr;
    DWORD resSize;
    char* srcData;

    kernelSrc = FindResourceA(hInst, MAKEINTRESOURCE(resid), MAKEINTRESOURCE(SHADER));
    resIdx = LoadResource(hInst, kernelSrc);
    resPtr = LockResource(resIdx);
    resSize = SizeofResource(hInst, kernelSrc);
    srcData = new char[resSize];
    RtlCopyMemory(srcData, resPtr, resSize);
    FreeResource(resIdx);

    prog = glCreateProgram();
    shader = glCreateShader(GL_COMPUTE_SHADER);

    glShaderSource(shader, 1, &srcData, NULL);
    glCompileShader(shader);

    glAttachShader(prog, shader);
    glLinkProgram(prog);
}

GLComputeProgram::GLComputeProgram(const char* source){
    prog = glCreateProgram();
    shader = glCreateShader(GL_COMPUTE_SHADER);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glAttachShader(prog, shader);
    glLinkProgram(prog);
}

GLComputeProgram::~GLComputeProgram(){
    glDeleteProgram(prog);
    glDeleteShader(shader);
}

GLuint GLComputeProgram::GetProgram(){
    return prog;
}

bool GLComputeProgram::CheckProgramError(){
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &progLog);
    return progLog > 0;// OpenGL 程序的日志长度为0时正常
}

bool GLComputeProgram::CheckShaderError(){
    glGetShaderiv(prog, GL_INFO_LOG_LENGTH, &shaderLog);
    return shaderLog > 0;// OpenGL 着色器的日志长度为-1时正常，不过为什么跟程序不一样？保险起见这样写
}

void GLComputeProgram::PrintProgramLog(){
    char* log;
    GLint logLen;

    if (progLog == -1)
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &progLog);
    if (progLog > 0){
        log = new char[progLog];
        glGetProgramInfoLog(prog, progLog, &progLog, log);
        DebugError("%s", log);
        delete[] log;
    }
}

void GLComputeProgram::PrintShaderLog(){
    char* log;
    GLint logLen;

    if (shaderLog == -1)
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &shaderLog);
    if (shaderLog > 0){
        log = new char[shaderLog];
        glGetShaderInfoLog(shader, shaderLog, &shaderLog, log);
        DebugError("%s", log);
        delete[] log;
    }
}

void GLComputeProgram::Dispatch(int x, int y, int z){
    glUseProgram(prog);
    glDispatchCompute(x, y, z);
}

GLuint GLComputeProgram::GetLoc(const char* name){
    return glGetUniformLocation(prog, name);
}

void GLComputeProgram::BindTexture(uint unit, uint texture, uenum access, uenum format){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 省略了glActiveTexture(GL_TEXTURE0);
    glBindImageTexture(unit, texture, 0, GL_FALSE, 0, access, format);
    glDisable(GL_TEXTURE_2D);
}