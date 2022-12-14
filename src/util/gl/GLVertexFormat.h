#ifndef __UTIL_GL_GLVERTEXFORMAT__
#define __UTIL_GL_GLVERTEXFORMAT__

#include <define.h>

#include <util/List.h>

namespace simple3deditor {

class GLVertexFormat {
public:
    struct VertexAttrib {
        int length; // between 1 and 4
        uenum type; // GL_FLOAT
    };

private:
    List<VertexAttrib> attrs;

public:
    GLVertexFormat();
    ~GLVertexFormat();
};

}

#endif