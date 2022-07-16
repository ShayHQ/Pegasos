#pragma once
#include <vulkan/vulkan.h>
#include <optional>
#include <vector>

#ifndef VK_RENDERER
#define VK_RENDERER
#endif

namespace Pegasos{
    class Window;

    struct QueueFamiliyIndices{
        int32_t graphicsFamily = -1, presentFamiliy = -1;

        bool isValid();
        void findFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    };


    struct PhysicalDeviceDetails{
        VkPhysicalDevice gpu;
        VkPhysicalDeviceProperties props;
        VkPhysicalDeviceFeatures features;
        QueueFamiliyIndices families;
        VkPhysicalDeviceMemoryProperties deviceMemProps;
    };

    struct SwapchainDetails{
        VkSwapchainKHR swapchain;
        std::vector<VkImage> images;
        std::vector<VkImageView> imagesView;
        std::vector<VkSemaphore> signalsImageAvailable;
        std::vector<VkSemaphore> signalsImageRendered;
        std::vector<VkFence> imagesInUse;
        std::vector<VkFramebuffer> framebuffers;
        VkSurfaceFormatKHR swapChainImageFormat;
        VkExtent2D swapChainExtent;
        VkSurfaceTransformFlagBitsKHR swapPreTransform;
        VkPresentModeKHR modeUsed;
    };

    struct VulkanDetails{
        VkInstance instance;
        VkSurfaceKHR surface;
        PhysicalDeviceDetails physicalDetails;
        VkDevice device;
        VkQueue graphic, present;
        SwapchainDetails swapchainDetails;
    };

    class VulkanBasicPipelinePlan;
    class VulkanGPipeline;
    class VulkanRenderer : public Renderer
    {
        VkInstance instance;
        VkSurfaceKHR surface;
        PhysicalDeviceDetails physicalDetails;
        VkDevice device;
        VkQueue graphic, present;
        SwapchainDetails swapchainDetails;
        VulkanGPipeline* pipeline;        
        VkCommandPool cmdPool;
        std::vector<VkCommandBuffer> commandBuffers;

        friend class VulkanBasicPipelinePlan;
    public:
        VulkanRenderer(VulkanDetails& details);
        ~VulkanRenderer() override;
    protected:
        virtual void recordJobs() override;
        virtual RenderJob* createJob(std::vector<Vertex>) override;
    private:
        void createFramebuffers();
        void createCmdBuffers();
        virtual void drawJobs() override;
    };

    class VulkanRendererPlan : public RendererPlan{
        VulkanDetails details;
        Window* window;
        bool wasBuilt;
        std::vector<const char *> instancesExtensions;
        std::vector<const char *> deviceExtensions;
        
    public:
        VulkanRendererPlan(Window* window);
        void initRendererAPI() override;
        void createPresentation() override;
        Renderer* getRenderer() override;
        ~VulkanRendererPlan() override;
    private:
        void createInstance();
        uint32_t ratePhysicalDevice(VkPhysicalDevice physicalDevice);
        void pickPhysicalDevice();
        void createSurface();
        void createDevice();
        void pickSurfaceFormat();
        void pickSurfaceExtent();
        void pickSurfacePresentMode();
        void createSwapchain();
        void createSwapchainImageViews();
        void createSynchros();
    };
};