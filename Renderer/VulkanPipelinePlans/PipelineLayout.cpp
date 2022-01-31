#include "../Renderer.hpp"
#include "../VulkanRenderer.hpp"
#include "../VulkanGPipeline.hpp"

using namespace Pegasos;


void VulkanBasicPipelinePlan::createLayout(){
    this->pipelineLayoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    this->pipelineLayoutInfo.setLayoutCount         = 0;
    this->pipelineLayoutInfo.pushConstantRangeCount = 0;
    this->pipelineLayoutInfo.pSetLayouts            = nullptr;
    this->pipelineLayoutInfo.pPushConstantRanges    = nullptr;

    if (vkCreatePipelineLayout( this->renderer->device, 
                                &this->pipelineLayoutInfo, 
                                nullptr, 
                                &this->pipeline.layout) != VK_SUCCESS){
        throw std::runtime_error("Failed to create pipeline layout!");
    }
}