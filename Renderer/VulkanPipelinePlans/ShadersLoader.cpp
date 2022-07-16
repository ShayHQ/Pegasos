#include "../Renderer.hpp"
#include "../VulkanRenderer.hpp"
#include "../VulkanGPipeline.hpp"

#include <PegasosCommon/common.hpp>
using namespace Pegasos;



VkShaderModule createShaderModule(VkDevice device, std::string path){
    VkShaderModule shaderModule;
    VkShaderModuleCreateInfo createInfo{};

    auto codeBuffer = readBinaryFile(path);

    createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = static_cast<std::size_t>(codeBuffer.size());
    createInfo.pCode    = reinterpret_cast<uint32_t*>(codeBuffer.data());

    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS){
        throw std::runtime_error(std::string("Unable to create a shader module from: " + path));
    }

    return shaderModule;
}

void VulkanBasicPipelinePlan::loadShaders(){
    // Temp implementation
    this->shaders.push_back(createShaderModule(this->renderer->device, "vert.spv"));
    this->shaders.push_back(createShaderModule(this->renderer->device, "frag.spv"));

    this->shaderStages.resize(this->shaders.size());

    this->shaderStages[0].module    = this->shaders[0];
    this->shaderStages[0].pName     = "main";
    this->shaderStages[0].sType     = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    this->shaderStages[0].stage     = VK_SHADER_STAGE_VERTEX_BIT;

    this->shaderStages[1].module    = this->shaders[1];
    this->shaderStages[1].pName     = "main";
    this->shaderStages[1].sType     = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    this->shaderStages[1].stage     = VK_SHADER_STAGE_FRAGMENT_BIT;

}