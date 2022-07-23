#include "../Renderer.hpp"
#include "../GLRenderer.hpp"
#include "../GLGPipeline.hpp"

using namespace Pegasos;


void GLBasicPipelinePlan::setFixedFunctions(){
    this->pipeline.applyVertexAttributes = [](GLuint buffer){
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
    };
}