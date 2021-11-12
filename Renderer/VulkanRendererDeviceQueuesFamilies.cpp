#include "Renderer.hpp"


using namespace craze;


void QueueFamiliyIndices::findFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface){
    uint32_t queuePropsCount = 0;
    std::vector<VkQueueFamilyProperties> queueProps;

    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queuePropsCount, nullptr);
    queueProps.resize(queuePropsCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queuePropsCount, queueProps.data());

    this->graphicsFamily = -1;
    this->presentFamiliy = -1;

    int i = 0;
    for (const auto& queueProp : queueProps){
        VkBool32 presentSupport;
        if (queueProp.queueCount > 0 && queueProp.queueFlags & VK_QUEUE_GRAPHICS_BIT){
            this->graphicsFamily = i;
        }

        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
        if (presentSupport == VK_TRUE){
            this->presentFamiliy = i;
        }

        if (isValid()){
            break;
        }
        i++;
    }
}


bool QueueFamiliyIndices::isValid(){
    return graphicsFamily > -1 && presentFamiliy > -1;
}