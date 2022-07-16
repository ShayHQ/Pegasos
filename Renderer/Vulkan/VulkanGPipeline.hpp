#ifndef VK_RENDERER
#include <vulkan/vulkan.h>
#endif

namespace Pegasos{

    class VulkanRenderer;
    struct VulkanGPipeline{
        VkPipelineLayout layout;
        VkRenderPass renderPass;
        VkPipeline pipelineRef;
    };
    

    class VulkanPipelinePlan{
    // Execution by this order
    public:
        virtual void loadShaders()              = 0;
        virtual void setFixedFunctions()        = 0;
        virtual void createLayout()             = 0;
        virtual void createRenderPass()         = 0;
        virtual void createPipeline()           = 0;

        virtual VulkanGPipeline getPipeline()   = 0;
    };

    class VulkanBasicPipelinePlan : public VulkanPipelinePlan{
    protected:
        VulkanRenderer* renderer;

        std::vector<VkShaderModule> shaders;
        std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
        
        VkPipelineVertexInputStateCreateInfo vertexInputInfo        = {};
        VkPipelineInputAssemblyStateCreateInfo inputAssembly        = {};
        VkPipelineViewportStateCreateInfo viewportState             = {};
        VkPipelineRasterizationStateCreateInfo rasterizerInfo       = {};
        VkPipelineMultisampleStateCreateInfo multisamplingInfo      = {};
        VkPipelineColorBlendAttachmentState colorBlendAttachment    = {};
        VkPipelineColorBlendStateCreateInfo blendStateInfo          = {};
        VkPipelineDynamicStateCreateInfo dynamicCreateInfo          = {};
        VkPipelineLayoutCreateInfo pipelineLayoutInfo               = {};
        VkRenderPassCreateInfo renderPassCreateInfo                 = {};
        
        std::vector<VkSubpassDependency> subpassDependencies;
        std::vector<VkAttachmentDescription> colourAttachments;
        std::vector<VkAttachmentReference> colourAttachmentRefs;
        std::vector<VkVertexInputAttributeDescription> vertexAttributes;
        std::vector<VkVertexInputBindingDescription> vertexBindings;
        VkSubpassDescription subpass = {};

        VkViewport viewport;
        VkRect2D scissor;


        VulkanGPipeline pipeline;
    public:
        virtual void loadShaders() override;
        virtual void setFixedFunctions() override;
        virtual void createLayout() override;
        virtual void createRenderPass() override;
        virtual void createPipeline() override;

        virtual VulkanGPipeline getPipeline() override;

        VulkanBasicPipelinePlan(VulkanRenderer* renderer);
    };
};