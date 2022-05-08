#version 120

attribute vec3 inLocation;

varying vec4 fragColor;
varying vec4 fragTexCoord;
varying vec3 fragNormal;

void main(){
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(inLocation, 1.0);
    fragColor = gl_Color;
    fragTexCoord = gl_MultiTexCoord0;
    fragNormal = gl_NormalMatrix * gl_Normal;
}
