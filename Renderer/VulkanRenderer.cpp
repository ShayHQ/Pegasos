#include "Renderer.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanGPipeline.hpp"
#include "VulkanMesh.hpp"
#include <iostream>

using namespace Pegasos;

VulkanRenderer::VulkanRenderer(VulkanDetails& details){
    this->instance          = details.instance;
    this->graphic           = details.graphic;
    this->present           = details.present;
    this->physicalDetails   = details.physicalDetails;
    this->surface           = details.surface;
    this->swapchainDetails  = details.swapchainDetails;
    this->device            = details.device;

    VulkanBasicPipelinePlan plan(this);

    plan.createRenderPass();
    plan.loadShaders();
    plan.setFixedFunctions();
    plan.createLayout();
    plan.createPipeline();
    

    this->pipeline = new VulkanGPipeline;
    *(this->pipeline) = plan.getPipeline();
    createFramebuffers();
}

VulkanRenderer::~VulkanRenderer(){
    for (const auto& job : this->jobs){
        delete job.second;
    }
    this->jobs.clear();

    for (const auto& fbuffer : this->swapchainDetails.framebuffers){
        vkDestroyFramebuffer(this->device, fbuffer, nullptr);
    }
    vkDestroyPipeline(this->device, this->pipeline->pipelineRef, nullptr);
    vkDestroyPipelineLayout(this->device, this->pipeline->layout, nullptr);
    vkDestroyRenderPass(this->device, this->pipeline->renderPass, nullptr);
    delete this->pipeline;
    for (int i = 0; i < this->swapchainDetails.imagesView.size(); i ++){
        vkDestroySemaphore(this->device, this->swapchainDetails.signalsImageAvailable[i], nullptr);
        vkDestroySemaphore(this->device, this->swapchainDetails.signalsImageRendered[i], nullptr);
        vkDestroyFence(this->device, this->swapchainDetails.imagesInUse[i], nullptr);
    }
    for (const auto& imageView : this->swapchainDetails.imagesView){
        vkDestroyImageView(this->device, imageView, nullptr);
    }
    vkDestroySwapchainKHR(this->device, this->swapchainDetails.swapchain, nullptr);
    vkDestroyDevice(this->device, nullptr);
    vkDestroySurfaceKHR(this->instance, this->surface, nullptr);
    vkDestroyInstance(this->instance, nullptr);
}

void VulkanRenderer::createFramebuffers(){
    this->swapchainDetails.framebuffers.resize(this->swapchainDetails.imagesView.size());

    for (int i = 0; i < this->swapchainDetails.imagesView.size(); i++){
        VkFramebufferCreateInfo createInfo = {};
        std::array<VkImageView, 1> attachments = {
            this->swapchainDetails.imagesView[i]
        };
        createInfo.width            = this->swapchainDetails.swapChainExtent.width;
        createInfo.height           = this->swapchainDetails.swapChainExtent.height;
        createInfo.layers           = 1;
        createInfo.sType            = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        createInfo.attachmentCount  = attachments.size();
        createInfo.pAttachments     = attachments.data();
        createInfo.renderPass       = this->pipeline->renderPass;
        
        if (vkCreateFramebuffer(this->device, &createInfo, nullptr, &this->swapchainDetails.framebuffers[i]) != VK_SUCCESS){
            throw std::runtime_error("Failed to create frame buffer");
        }
    }
}

void VulkanRenderer::drawJobs(){
    //TODO: drawing
}

void VulkanRenderer::deleteJob(int jobID){
    // Implementation is still not synced with drawing
    auto found = this->jobs.find(jobID);
    if (found == this->jobs.end()){
        throw std::runtime_error(std::string("JobID: "+ jobID) + " Is not exists!");
    }

    this->jobs.erase(found);
}


int VulkanRenderer::addJob(std::vector<Vertex> job){
    // Implementation is still not synced with drawing
    const int MAXIMUM_RETRIES = 10;
    int jobID = 0;
    int retries = 0;
    std::map<int, RenderJob*>::iterator found;
    do{
        jobID = rand() / INT16_MAX;
        found = this->jobs.find(jobID);
        if (MAXIMUM_RETRIES == retries++){
            throw std::runtime_error("Failed to add render job, nax retries reached!");
        }
    }while(found != this->jobs.end());

    this->jobs.insert(std::make_pair(jobID, new VulkanMesh(this->device, this->physicalDetails.deviceMemProps, job)));

    return jobID;
}