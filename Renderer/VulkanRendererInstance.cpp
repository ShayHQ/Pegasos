#define GLFW_INCLUDE_VULKAN
#include "Renderer.hpp"
#include "../Window/Window.hpp"
#include <GLFW/glfw3.h>

using namespace craze;

VkApplicationInfo getCrazyApplicationInfo(){
    VkApplicationInfo appInfo;

    appInfo.apiVersion          = VK_API_VERSION_1_0;
    appInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext               = nullptr;
    appInfo.applicationVersion  = VK_MAKE_VERSION(0, 0, 1);
    appInfo.engineVersion       = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pApplicationName    = "CrazyApp";
    appInfo.pEngineName         = "Crazy Engine";

    return appInfo;
}

VulkanRendererPlan::VulkanRendererPlan(Window* window){
    this->window    = window;
    this->wasBuilt  = false;
    if (window){
        GLFWwindow* handler = static_cast<GLFWwindow*>(this->window->getHandler());
        uint32_t glfwExtensionCount = 0;
        auto extensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        for (int i = 0; i < glfwExtensionCount; i++){
            this->instancesExtensions.push_back(extensions[i]);
        }
        if (glfwExtensionCount){
            this->deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        }
    }
}


void VulkanRendererPlan::initRendererAPI(){
    createInstance();
    createSurface();
    createDevice();
}

Renderer* VulkanRendererPlan::getRenderer(){
    return nullptr; //new VulkanRenderer(this->details);
}

void VulkanRendererPlan::createInstance(){
    VkApplicationInfo crazyAppInfo = getCrazyApplicationInfo();
    VkInstanceCreateInfo createInfo;

    createInfo.pNext                    = nullptr;
    createInfo.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.ppEnabledLayerNames      = nullptr;
    createInfo.enabledLayerCount        = 0;
    createInfo.pApplicationInfo         = &crazyAppInfo;
    createInfo.enabledExtensionCount    = static_cast<uint32_t>(this->instancesExtensions.size());
    createInfo.ppEnabledExtensionNames  = this->instancesExtensions.data();
    
    if (vkCreateInstance(&createInfo, nullptr, &this->details.instance) != VK_SUCCESS){
        throw std::runtime_error("Failed to create vulkan instance!");
    }
}

void VulkanRendererPlan::createSurface(){
    if (this->window){
        GLFWwindow* handler = static_cast<GLFWwindow*>(this->window->getHandler());

        if (glfwCreateWindowSurface(this->details.instance, handler, nullptr, &this->details.surface) != VK_SUCCESS){
            throw std::runtime_error("Failed to create surface!");
        }
    }
}

VulkanRendererPlan::~VulkanRendererPlan(){
    if (!wasBuilt){
        // failed to create renderer cleanup resources
        vkDestroySwapchainKHR(this->details.device, this->details.swapchainDetails.swapchain, nullptr);
        vkDestroyDevice(this->details.device, nullptr);
        vkDestroySurfaceKHR(this->details.instance, this->details.surface, nullptr);
        vkDestroyInstance(this->details.instance, nullptr);
    }
}