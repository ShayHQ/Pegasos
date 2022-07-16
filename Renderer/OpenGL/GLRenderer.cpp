#include "Renderer.hpp"
#include "GLGPipeline.hpp"
#include "GLRenderer.hpp"

using namespace Pegasos;

GLRenderer::GLRenderer(GLDetails& details){
    memcpy(&this->details, &details, sizeof(GLDetails));
}

GLRenderer::~GLRenderer(){

}


void GLRenderer::recordJobs(){

}
void GLRenderer::drawJobs(){

}
RenderJob* GLRenderer::createJob(std::vector<Vertex> job){
    return nullptr;
}