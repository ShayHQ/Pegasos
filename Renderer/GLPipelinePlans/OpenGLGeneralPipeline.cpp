#include "../Renderer.hpp"
#include "../GLRenderer.hpp"
#include "../GLGPipeline.hpp"

using namespace Pegasos;

GLBasicPipelinePlan::GLBasicPipelinePlan(GLRenderer* renderer){
    this->renderer = renderer;
    this->pipeline = {};
}


void GLBasicPipelinePlan::createPipeline(){

}