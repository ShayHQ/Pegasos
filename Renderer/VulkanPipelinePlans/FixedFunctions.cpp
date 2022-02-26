#include "../Renderer.hpp"
#include "../VulkanRenderer.hpp"
#include "../VulkanGPipeline.hpp"

using namespace Pegasos;

void setVertexInput(VkPipelineVertexInputStateCreateInfo& vertexInCreateInfo, 
                    std::vector<VkVertexInputAttributeDescription>& vertexAttributes,
                    std::vector<VkVertexInputBindingDescription>& vertexBindings){
    vertexAttributes.resize(2);
    vertexBindings.resize(1);

    vertexAttributes[0].binding                         = 0;
    vertexAttributes[0].format                          = VK_FORMAT_R32G32B32_SFLOAT;
    vertexAttributes[0].location                        = 0;
    vertexAttributes[0].offset                          = offsetof(Vertex, position);

    vertexAttributes[1].binding                         = 0;
    vertexAttributes[1].format                          = VK_FORMAT_R32G32B32_SFLOAT;
    vertexAttributes[1].location                        = 1;
    vertexAttributes[1].offset                          = offsetof(Vertex, color);

    vertexBindings[0].binding                           = 0;
    vertexBindings[0].inputRate                         = VK_VERTEX_INPUT_RATE_VERTEX;
    vertexBindings[0].stride                            = sizeof(Vertex);

    vertexInCreateInfo.sType                            = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInCreateInfo.vertexBindingDescriptionCount    = vertexBindings.size();
    vertexInCreateInfo.pVertexBindingDescriptions       = vertexBindings.data();
    vertexInCreateInfo.vertexAttributeDescriptionCount  = vertexAttributes.size();
    vertexInCreateInfo.pVertexAttributeDescriptions     = vertexAttributes.data();
}

void setInputAssembly(VkPipelineInputAssemblyStateCreateInfo& inputAssembly){
    inputAssembly.primitiveRestartEnable    = VK_FALSE;
    inputAssembly.sType                     = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology                  = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
}

void setDisplayState(VkPipelineViewportStateCreateInfo& displayStateInfo, VkViewport& viewport, VkRect2D& scissor, SwapchainDetails& details){
    scissor.offset  = {0, 0};
    scissor.extent  = details.swapChainExtent;
    
    viewport.x          = 0.0f;
    viewport.y          = 0.0f;
    viewport.width      = static_cast<float>(details.swapChainExtent.width);
    viewport.height     = static_cast<float>(details.swapChainExtent.height);
    viewport.minDepth   = 0.0f;
    viewport.maxDepth   = 1.0f;

    displayStateInfo.sType          = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    displayStateInfo.scissorCount   = 1;
    displayStateInfo.viewportCount  = 1;
    displayStateInfo.pViewports     = &viewport;
    displayStateInfo.pScissors      = &scissor;
}

void setRasterizer(VkPipelineRasterizationStateCreateInfo& rasterizerCreateInfo){
    rasterizerCreateInfo.sType                      = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizerCreateInfo.depthClampEnable           = VK_FALSE;
    rasterizerCreateInfo.rasterizerDiscardEnable    = VK_FALSE;
    rasterizerCreateInfo.polygonMode                = VK_POLYGON_MODE_FILL;
    rasterizerCreateInfo.lineWidth                  = 1.0f;
    rasterizerCreateInfo.cullMode                   = VK_CULL_MODE_BACK_BIT;
    rasterizerCreateInfo.frontFace                  = VK_FRONT_FACE_CLOCKWISE;

    rasterizerCreateInfo.depthBiasEnable            = VK_FALSE;
    rasterizerCreateInfo.depthBiasConstantFactor    = 0.0f; // Optional
    rasterizerCreateInfo.depthBiasClamp             = 0.0f; // Optional
    rasterizerCreateInfo.depthBiasSlopeFactor       = 0.0f; // Optional
}

void setMultisampling(VkPipelineMultisampleStateCreateInfo& multiSampleInfo){
    multiSampleInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multiSampleInfo.sampleShadingEnable     = VK_FALSE;
    multiSampleInfo.rasterizationSamples    = VK_SAMPLE_COUNT_1_BIT;

    multiSampleInfo.minSampleShading        = 1.0f; // Optional
    multiSampleInfo.pSampleMask             = nullptr; // Optional
    multiSampleInfo.alphaToCoverageEnable   = VK_FALSE; // Optional
    multiSampleInfo.alphaToOneEnable        = VK_FALSE; // Optional
}

void setColorBlending(VkPipelineColorBlendAttachmentState& blendAttachmentState, VkPipelineColorBlendStateCreateInfo& blendStateInfo){
    blendAttachmentState.colorWriteMask      =  VK_COLOR_COMPONENT_R_BIT | 
                                                VK_COLOR_COMPONENT_G_BIT | 
                                                VK_COLOR_COMPONENT_B_BIT | 
                                                VK_COLOR_COMPONENT_A_BIT;
    blendAttachmentState.blendEnable         = VK_FALSE; // NO BLEND!

    blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    blendAttachmentState.colorBlendOp        = VK_BLEND_OP_ADD; // Optional
    blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    blendAttachmentState.alphaBlendOp        = VK_BLEND_OP_ADD; // Optional


    blendStateInfo.sType            = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    blendStateInfo.logicOpEnable    = VK_FALSE;
    blendStateInfo.attachmentCount  = 1;
    blendStateInfo.pAttachments     = &blendAttachmentState;
}

void setDynamicStates(VkPipelineDynamicStateCreateInfo& dynamicCreateInfo){
    // Will not be supported right now 
    // VkDynamicState dynamicStates[] = {
    //     VK_DYNAMIC_STATE_VIEWPORT,
    //     VK_DYNAMIC_STATE_LINE_WIDTH
    // };

    // VkPipelineDynamicStateCreateInfo dynamicState{};
    // dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    // dynamicState.dynamicStateCount = 2;
    // dynamicState.pDynamicStates = dynamicStates;
}

void VulkanBasicPipelinePlan::setFixedFunctions(){
    setVertexInput(this->vertexInputInfo, this->vertexAttributes, this->vertexBindings);
    setInputAssembly(this->inputAssembly);
    setDisplayState(this->viewportState, this->viewport, this->scissor, this->renderer->swapchainDetails);
    setRasterizer(this->rasterizerInfo);
    setMultisampling(this->multisamplingInfo);
    setColorBlending(this->colorBlendAttachment, this->blendStateInfo);
    setDynamicStates(this->dynamicCreateInfo);

}
