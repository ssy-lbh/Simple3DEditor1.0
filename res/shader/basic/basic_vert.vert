#version 420

layout (location = 0) in vec3 inLocation;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec3 inNormal;

layout (binding = 0) uniform Transform {
    mat4x4 proj;
    mat4x4 modelview;
} trans;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec2 fragTexCoord;
layout (location = 2) out vec3 fragNormal;

void main(){
    //gl_Position = trans.proj * trans.modelview * vec4(inLocation, 1.0);
    gl_Position = vec4(inLocation, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    fragNormal = mat3x3(trans.proj * trans.modelview) * inNormal;
}
