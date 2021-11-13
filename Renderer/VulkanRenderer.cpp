#include "Renderer.hpp"

using namespace Pegasos;

VulkanRenderer::VulkanRenderer(VulkanDetails& details){
    this->instance          = details.instance;
    this->graphic           = details.graphic;
    this->present           = details.present;
    this->physicalDetails   = details.physicalDetails;
    this->surface           = details.surface;
    this->swapchainDetails  = details.swapchainDetails;
    this->device            = details.device;
}

VulkanRenderer::~VulkanRenderer(){
    for (const auto& imageView : this->swapchainDetails.imagesView){
        vkDestroyImageView(this->device, imageView, nullptr);
    }
    vkDestroySwapchainKHR(this->device, this->swapchainDetails.swapchain, nullptr);
    vkDestroyDevice(this->device, nullptr);
    vkDestroySurfaceKHR(this->instance, this->surface, nullptr);
    vkDestroyInstance(this->instance, nullptr);
}