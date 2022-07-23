#include "Renderer.hpp"
#include "GLGPipeline.hpp"
#include "GLRenderer.hpp"
#include "GLMesh.hpp"
using namespace Pegasos;

GLRenderer::GLRenderer(GLDetails& details){
    memcpy(&this->details, &details, sizeof(GLDetails));
    auto plan = new GLBasicPipelinePlan(this);
    plan->setFixedFunctions();
    plan->loadShaders();
    plan->createPipeline();
    maxAVOAllocated = 2;
    this->vaos.resize(maxAVOAllocated);
    glGenVertexArrays(maxAVOAllocated, this->vaos.data());
    this->pipeline = new GLGPipeline; 
    *this->pipeline = plan->getPipeline();
}

GLRenderer::~GLRenderer(){
    for (const auto& job : this->jobs){
        delete job.second;
    }
    glUseProgram(0);
    glBindVertexArray(0);
    glDeleteProgram(this->pipeline->program);
    glDeleteVertexArrays(maxAVOAllocated, this->vaos.data());
}

void GLRenderer::recordJobs(){
    if (maxAVOAllocated < this->jobs.size()){
        this->vaos.resize(2 * maxAVOAllocated);
        glGenVertexArrays(2 * maxAVOAllocated, this->vaos.data() + maxAVOAllocated);
        maxAVOAllocated *= 2;;
    }
    int i = 0;
    for (auto& job : this->jobs){
        glBindVertexArray(vaos[i++]);
        GLuint bufferID = *reinterpret_cast<GLuint*>(job.second->data());
        this->pipeline->applyVertexAttributes(bufferID);
        glBindVertexArray(0);
    }
}
void GLRenderer::drawJobs(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(this->pipeline->program);
    int i = 0;
    for (auto& job : this->jobs){
        glBindVertexArray(this->vaos[i++]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    glBindVertexArray(0);
}
RenderJob* GLRenderer::createJob(std::vector<Vertex> job){
    return new GLMesh(job);
}