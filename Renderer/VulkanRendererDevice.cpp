#include "Renderer.hpp"
#include <set>
#include <map>

using namespace Pegasos;

uint32_t VulkanRendererPlan::ratePhysicalDevice(VkPhysicalDevice physicalDevice){
    uint32_t score = 0;
    QueueFamiliyIndices indices;
    indices.findFamilies(physicalDevice, this->details.surface);
    if (indices.isValid()){
        VkPhysicalDeviceProperties deviceProps;
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProps);

        score += deviceProps.limits.maxImageDimension2D;
        score += deviceProps.limits.maxColorAttachments;
        score += deviceProps.limits.framebufferColorSampleCounts;
        score += deviceProps.limits.framebufferStencilSampleCounts;

        // Discrete GPUs have a significant performance advantage
        if (deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score = INT32_MAX;
        }
    }
    return score;
}

void VulkanRendererPlan::pickPhysicalDevice(){
    uint32_t optionalDevicesCount = 0;
    std::vector<VkPhysicalDevice> optionalDevices;
    std::multimap<uint32_t, VkPhysicalDevice> suitableDevices;

    vkEnumeratePhysicalDevices(this->details.instance, &optionalDevicesCount, nullptr);
    optionalDevices.resize(optionalDevicesCount);
    vkEnumeratePhysicalDevices(this->details.instance, &optionalDevicesCount, optionalDevices.data());

    for (const auto& optionallDevice : optionalDevices){
        uint32_t score = 0;
        score = ratePhysicalDevice(optionallDevice);
        if (score){
            suitableDevices.insert(std::make_pair(score, optionallDevice));
        }
    }

    if (suitableDevices.size() == 0 || suitableDevices.rbegin()->first <= 0) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    auto& physicalDetails = this->details.physicalDetails;
    physicalDetails.gpu   = suitableDevices.rbegin()->second;
    physicalDetails.families.findFamilies(physicalDetails.gpu, this->details.surface);
    vkGetPhysicalDeviceFeatures(physicalDetails.gpu, &physicalDetails.features);
    vkGetPhysicalDeviceProperties(physicalDetails.gpu, &physicalDetails.props);
    vkGetPhysicalDeviceMemoryProperties(physicalDetails.gpu, &physicalDetails.deviceMemProps);
}

void VulkanRendererPlan::createDevice(){
    VkDeviceCreateInfo createInfo = {};
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> queueFamilies;
    
    float priority = 1.0f;
    
    pickPhysicalDevice();

    queueFamilies.insert(this->details.physicalDetails.families.graphicsFamily);
    queueFamilies.insert(this->details.physicalDetails.families.presentFamiliy);

    for (const auto& familyQueue : queueFamilies){
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.pNext               = nullptr;
        queueCreateInfo.sType               = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.pQueuePriorities    = &priority;
        queueCreateInfo.queueCount          = 1; // Development
        queueCreateInfo.queueFamilyIndex    = familyQueue;

        queueCreateInfos.push_back(queueCreateInfo);
    }
    
    createInfo.enabledExtensionCount    = this->deviceExtensions.size();
    createInfo.ppEnabledExtensionNames  = this->deviceExtensions.data();
    createInfo.enabledLayerCount        = 0;
    createInfo.ppEnabledLayerNames      = nullptr;
    createInfo.pEnabledFeatures         = &this->details.physicalDetails.features;
    createInfo.queueCreateInfoCount     = queueCreateInfos.size();
    createInfo.pQueueCreateInfos        = queueCreateInfos.data();
    createInfo.sType                    = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    if (vkCreateDevice(this->details.physicalDetails.gpu, &createInfo, nullptr, &this->details.device) != VK_SUCCESS){
        throw std::runtime_error(std::string("Failed to create vulkan device of " + std::string(this->details.physicalDetails.props.deviceName)));
    }

    vkGetDeviceQueue(this->details.device, this->details.physicalDetails.families.graphicsFamily, 0, &this->details.graphic);
    vkGetDeviceQueue(this->details.device, this->details.physicalDetails.families.presentFamiliy, 0, &this->details.present);
}