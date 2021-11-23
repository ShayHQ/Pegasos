#version 450

layout (location = 0) in vec3 fragOut;

layout (location = 0) out vec4 finalOut;

void main(){
    finalOut = vec4(fragOut, 1.0);
}