#include <utils/gl/GLLights.h>

#include <lib/opengl/gl/gl.h>

#include <utils/os/Log.h>

bool GLLights::use[8] = {false, false, false, false,
                         false, false, false, false};
uenum GLLights::idx[8] = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3,
                           GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};
                        
uenum GLLights::Create(){
    for (int i = 0; i < 8; i++){
        if (!use[i]){
            use[i] = true;
            return idx[i];
        }
    }
    DebugError("GLLights::CreateLight Failed");
    return GL_NONE;
}

bool GLLights::Destroy(uenum light){
    for (int i = 0; i < 8; i++){
        if (light == idx[i]){
            if (!use[i]){
                DebugError("GLLights::Destroy Light %d Has Already Been Destroyed", light);
                return false;
            }
            use[i] = false;
            return true;
        }
    }
    DebugError("GLLights::Destroy Light %d Not Found", light);
    return false;
}

void GLLights::Enable(uenum light){
    glEnable(light);
}

void GLLights::Disable(uenum light){
    glDisable(light);
}