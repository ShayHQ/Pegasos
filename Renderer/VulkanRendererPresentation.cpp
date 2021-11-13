#include "Renderer.hpp"
#include "../Window/Window.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>

using namespace craze;

void VulkanRendererPlan::createPresentation(){
    if (this->window){
        pickSurfaceExtent();
        pickSurfaceFormat();
        pickSurfacePresentMode();
        createSwapchain();
    }   
}


void VulkanRendererPlan::pickSurfaceFormat(){
    VkFormat bestFormat = VK_FORMAT_B8G8R8A8_SRGB;
    uint32_t formatCount = 0;
    std::vector<VkSurfaceFormatKHR> surfaceFormats;

    vkGetPhysicalDeviceSurfaceFormatsKHR(this->details.physicalDetails.gpu, this->details.surface, &formatCount, nullptr);
    surfaceFormats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(this->details.physicalDetails.gpu, this->details.surface, &formatCount, surfaceFormats.data());

    if (surfaceFormats.size() == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED){
        this->details.swapchainDetails.swapChainImageFormat = {bestFormat, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }else{
        for (const auto& format : surfaceFormats){
            if (format.format == bestFormat){
                this->details.swapchainDetails.swapChainImageFormat = format;
            }
        }
    }
    
}

void VulkanRendererPlan::pickSurfaceExtent(){
    VkSurfaceCapabilitiesKHR surfaceCaps;

    int* extentArrayLike    = (int*)(&this->details.swapchainDetails.swapChainExtent);
    int& width              = extentArrayLike[1];
    int& height             = extentArrayLike[0];

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(this->details.physicalDetails.gpu, this->details.surface, &surfaceCaps);
    glfwGetWindowSize(this->window->getHandler(), &width, &height); // why create two more variables when we can use hack like this 5:-)
    
    this->details.swapchainDetails.swapPreTransform = surfaceCaps.currentTransform;
    this->details.swapchainDetails.swapChainExtent.width = std::max<uint32_t>(surfaceCaps.maxImageExtent.width, std::min<uint32_t>(width, surfaceCaps.minImageExtent.width));
    this->details.swapchainDetails.swapChainExtent.height = std::max<uint32_t>(surfaceCaps.maxImageExtent.height, std::min<uint32_t>(height, surfaceCaps.minImageExtent.height));
}

void VulkanRendererPlan::pickSurfacePresentMode(){
    uint32_t presentModeCount = 0;
    std::vector<VkPresentModeKHR> presentModes;
    VkPresentModeKHR defaultMode = VK_PRESENT_MODE_FIFO_KHR;
    VkPresentModeKHR targetMode = VK_PRESENT_MODE_MAILBOX_KHR;
    VkPresentModeKHR found = defaultMode;

    vkGetPhysicalDeviceSurfacePresentModesKHR(this->details.physicalDetails.gpu, this->details.surface, &presentModeCount, nullptr);
    presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(this->details.physicalDetails.gpu, this->details.surface, &presentModeCount, presentModes.data());

    for (const auto& mode : presentModes){
        if (mode == targetMode){
            found = mode;
            break;
        }
    }

    this->details.swapchainDetails.modeUsed = found;
}


// Don't run this function without running the picks functions above!
void VulkanRendererPlan::createSwapchain(){
    VkSurfaceCapabilitiesKHR surfaceCaps;
    VkSwapchainCreateInfoKHR swapCreateInfo;
    uint32_t imageCount = 0;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(this->details.physicalDetails.gpu, this->details.surface, &surfaceCaps);

    swapCreateInfo.pNext            = nullptr;
    swapCreateInfo.oldSwapchain     = VK_NULL_HANDLE;
    swapCreateInfo.surface          = this->details.surface;
    swapCreateInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapCreateInfo.presentMode      = this->details.swapchainDetails.modeUsed;
    swapCreateInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapCreateInfo.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapCreateInfo.preTransform     = this->details.swapchainDetails.swapPreTransform;
    swapCreateInfo.imageExtent      = this->details.swapchainDetails.swapChainExtent;
    swapCreateInfo.imageFormat      = this->details.swapchainDetails.swapChainImageFormat.format;
    swapCreateInfo.imageColorSpace  = this->details.swapchainDetails.swapChainImageFormat.colorSpace;
    swapCreateInfo.imageArrayLayers = 1;
    swapCreateInfo.clipped          = VK_TRUE;
    swapCreateInfo.minImageCount    = std::min<uint32_t>(surfaceCaps.maxImageCount ,surfaceCaps.minImageCount + 1);

    if (this->details.physicalDetails.families.graphicsFamily != this->details.physicalDetails.families.presentFamiliy){
        swapCreateInfo.imageSharingMode         = VK_SHARING_MODE_CONCURRENT;
        swapCreateInfo.queueFamilyIndexCount    = 2;
        swapCreateInfo.pQueueFamilyIndices      = (uint32_t*)&this->details.physicalDetails.families;
    }else{
        swapCreateInfo.imageSharingMode         = VK_SHARING_MODE_EXCLUSIVE;
        swapCreateInfo.queueFamilyIndexCount    = 0;
        swapCreateInfo.pQueueFamilyIndices      = nullptr;
    }

    if (vkCreateSwapchainKHR(this->details.device, &swapCreateInfo, nullptr, &this->details.swapchainDetails.swapchain) != VK_SUCCESS){
        throw std::runtime_error("Failed to create swapchain for device " + std::string(this->details.physicalDetails.props.deviceName));
    }

    vkGetSwapchainImagesKHR(this->details.device, this->details.swapchainDetails.swapchain, &imageCount, nullptr);
    this->details.swapchainDetails.images.resize(imageCount);
    vkGetSwapchainImagesKHR(this->details.device, this->details.swapchainDetails.swapchain, &imageCount, this->details.swapchainDetails.images.data());
}