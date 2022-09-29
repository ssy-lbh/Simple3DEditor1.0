#include <util/gl/GLEW.h>

#include <glew/gl/wglew.h>

#include <util/os/Log.h>

namespace simple3deditor {

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

}