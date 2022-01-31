#include "../Renderer.hpp"
#include "../VulkanRenderer.hpp"
#include "../VulkanGPipeline.hpp"


using namespace Pegasos;

VulkanBasicPipelinePlan::VulkanBasicPipelinePlan(VulkanRenderer* renderer){
    this->renderer = renderer;
}


void VulkanBasicPipelinePlan::createPipeline(){

    
    for (const auto shaderModule : this->shaders){
        vkDestroyShaderModule(this->renderer->device, shaderModule, nullptr);
    }
}


VulkanGPipeline VulkanBasicPipelinePlan::getPipeline(){
    return this->pipeline;
}