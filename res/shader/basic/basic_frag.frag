#version 120

varying vec4 fragColor;
varying vec4 fragTexCoord;
varying vec3 fragNormal;

void main(){
    gl_FragColor = fragColor;
}
