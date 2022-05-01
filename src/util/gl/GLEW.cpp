#include <util/gl/GLEW.h>

#include <lib/glew/gl/wglew.h>

#include <util/os/Log.h>

static bool init = false;

void glInitGLEW(){
    if (init){
        return;
    }
    init = true;
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK){
        DebugError("GLEW Initialization Failed");
    }
}