#version 140

in vec4 fragColor;
in vec4 fragTexCoord;
in vec3 fragNormal;

void main(){
    gl_FragColor = fragColor;
}
