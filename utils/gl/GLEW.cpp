#include <utils/gl/GLEW.h>

#include <lib/glew/gl/wglew.h>

#include <utils/os/Log.h>

void glInitGLEW(){
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK){
        DebugError("GLEW Initialization Failed");
    }
}