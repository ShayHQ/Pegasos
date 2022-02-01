#include "../Renderer.hpp"
#include "../VulkanRenderer.hpp"
#include "../VulkanGPipeline.hpp"


using namespace Pegasos;

VulkanBasicPipelinePlan::VulkanBasicPipelinePlan(VulkanRenderer* renderer){
    this->renderer = renderer;
}


void VulkanBasicPipelinePlan::createPipeline(){
    VkGraphicsPipelineCreateInfo createInfo = {};
    createInfo.basePipelineHandle   = VK_NULL_HANDLE;
    createInfo.basePipelineIndex    = -1;
    createInfo.sType                = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    createInfo.layout               = this->pipeline.layout;
    createInfo.pColorBlendState     = &this->blendStateInfo;
    createInfo.pDepthStencilState   = nullptr;
    createInfo.pDynamicState        = nullptr;
    createInfo.pInputAssemblyState  = &this->inputAssembly;
    createInfo.pMultisampleState    = &this->multisamplingInfo;
    createInfo.pRasterizationState  = &this->rasterizerInfo;
    createInfo.pStages              = this->shaderStages.data();
    createInfo.stageCount           = static_cast<uint32_t>(this->shaderStages.size());
    createInfo.pTessellationState   = nullptr;
    createInfo.pVertexInputState    = &this->vertexInputInfo;
    createInfo.pViewportState       = &this->viewportState;
    createInfo.subpass              = 0;
    createInfo.renderPass           = this->pipeline.renderPass;

    auto result = vkCreateGraphicsPipelines(this->renderer->device, VK_NULL_HANDLE, 1, &createInfo, nullptr, &(this->pipeline.pipelineRef));
    if (result != VK_SUCCESS){
        throw std::runtime_error("Failed to create graphics pipeline!");
    }
    
    for (const auto shaderModule : this->shaders){
        vkDestroyShaderModule(this->renderer->device, shaderModule, nullptr);
    }
}


VulkanGPipeline VulkanBasicPipelinePlan::getPipeline(){
    return this->pipeline;
}