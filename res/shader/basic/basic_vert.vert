#version 140

in vec3 inLocation;

out vec4 fragColor;
out vec4 fragTexCoord;
out vec3 fragNormal;

void main(){
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(inLocation, 1.0);
    fragColor = gl_Color;
    fragTexCoord = gl_MultiTexCoord0;
    fragNormal = gl_NormalMatrix * gl_Normal;
}
