#include <vulkan/vulkan.h>
#include <optional>
#include <vector>

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
    };

    struct SwapchainDetails{
        VkSwapchainKHR swapchain;
        std::vector<VkImage> images;
        std::vector<VkImageView> imagesView;
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

    class VulkanRenderer : public Renderer
    {
        VkInstance instance;
        VkSurfaceKHR surface;
        PhysicalDeviceDetails physicalDetails;
        VkDevice device;
        VkQueue graphic, present;
        SwapchainDetails swapchainDetails;

    public:
        VulkanRenderer(VulkanDetails& details);
        ~VulkanRenderer() override;
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
    };
};