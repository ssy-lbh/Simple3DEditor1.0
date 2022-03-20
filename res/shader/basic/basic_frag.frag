#version 420

layout (location = 0) in vec4 fragColor;
layout (location = 1) in vec4 fragTexCoord;
layout (location = 2) in vec2 fragNormal;

layout (location = 0) out vec4 outColor;

void main(){
    outColor = fragColor;
}
