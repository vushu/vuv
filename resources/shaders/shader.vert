#version 300 es
precision highp float;
precision highp int;
in vec3 aPos;
in vec4 aColor;
in vec2 aTexCoords;
in float aTexId;
in float aType;
uniform mat4 uMvp;

out vec4 fColor;
out vec2 fTexCoords;
out float fTexId;
out float fType;

void main() {
    fColor = aColor;
    fTexCoords = aTexCoords;
    fTexId = aTexId;
    fType = aType;

    gl_Position = uMvp * vec4(aPos, 1.0);
}