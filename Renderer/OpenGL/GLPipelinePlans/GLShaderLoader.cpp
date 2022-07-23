#include "../Renderer.hpp"
#include "../GLRenderer.hpp"
#include "../GLGPipeline.hpp"
#include <algorithm>

#include <PegasosCommon/common.hpp>
using namespace Pegasos;

using ShaderBuffer = std::vector<char>;
using GL_ID        = GLuint;

const char* vertexShader = "\n\
#version 330 core \n\
#extension GL_ARB_separate_shader_objects : require\n\
layout (location = 0) in vec3 vertexPos; \n\
layout (location = 1) in vec3 vertexColor; \n\
layout (location = 0) out vec3 fragOut; \n\
void main(){ \n\
    gl_Position = vec4(vertexPos, 1.0); \n\
    fragOut = vertexColor;   \n\
}";


const char* fragShader = "\
#version 330 core \n\
#extension GL_ARB_separate_shader_objects : require\n\
layout (location = 0) in vec3 fragOut; \n\
layout (location = 0) out vec4 finalOut;\n\
void main(){\n\
    finalOut = vec4(fragOut, 1.0);\n\
}";


void throwOnGLError(std::function<void(GLint*)> bindedIV, std::function<void(GLsizei, GLsizei*, char*)> bindedIVlog){
    char messageBuffer[100] = {};
    GLint status;
    bindedIV(&status);
    if (status == GL_FALSE){
        bindedIVlog(100, nullptr, messageBuffer);
        throw std::runtime_error(messageBuffer);
    }
}

template<unsigned int SHADER_TYPE>
GL_ID loadShader(std::string src){
    GL_ID shader = glCreateShader(SHADER_TYPE);
    const char* cStr = src.c_str();
    glShaderSource(shader, 1, const_cast<const GLchar *const *>(&cStr), nullptr);
    glCompileShader(shader);

    throwOnGLError(std::bind(glGetShaderiv, shader, GL_COMPILE_STATUS, std::placeholders::_1),
                   std::bind(glGetShaderInfoLog, shader, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    
    return shader;
}

GL_ID linkProgram(std::vector<GL_ID>& shaders){
    GL_ID progID = glCreateProgram();

    for (const auto& shaderID : shaders)
        glAttachShader(progID, shaderID);
    glLinkProgram(progID);

    throwOnGLError(std::bind(glGetProgramiv, progID ,GL_LINK_STATUS, std::placeholders::_1),
                   std::bind(glGetProgramInfoLog, progID, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    return progID;
}

void GLBasicPipelinePlan::loadShaders(){
    std::vector<GL_ID> shaders = {
        loadShader<GL_VERTEX_SHADER>(vertexShader),
        loadShader<GL_FRAGMENT_SHADER>(fragShader)
    };
    this->pipeline.program = linkProgram(shaders);

    std::for_each(shaders.begin(), shaders.end(), glDeleteShader);
}