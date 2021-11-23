#version 450

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexColor;

layout (location = 0) out vec3 fragOut;

void main(){
    gl_Position = vec4(vertexPos, 1.0);
    fragOut = vertexColor;
}