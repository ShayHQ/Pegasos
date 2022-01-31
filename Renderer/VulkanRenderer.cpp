#include "Renderer.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanGPipeline.hpp"
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

    plan.loadShaders();
    plan.setFixedFunctions();
    plan.createLayout();
    plan.createSubpass();
    plan.createPipeline();
    

    this->pipeline = new VulkanGPipeline;
    *(this->pipeline) = plan.getPipeline();
}

VulkanRenderer::~VulkanRenderer(){
    // vkDestroyPipeline(this->device, this->pipeline->pipelineRef, nullptr);
    vkDestroyPipelineLayout(this->device, this->pipeline->layout, nullptr);
    // vkDestroyRenderPass(this->device, this->pipeline->renderPass, nullptr);
    delete this->pipeline;
    for (const auto& imageView : this->swapchainDetails.imagesView){
        vkDestroyImageView(this->device, imageView, nullptr);
    }
    vkDestroySwapchainKHR(this->device, this->swapchainDetails.swapchain, nullptr);
    vkDestroyDevice(this->device, nullptr);
    vkDestroySurfaceKHR(this->instance, this->surface, nullptr);
    vkDestroyInstance(this->instance, nullptr);
}

void VulkanRenderer::drawJobs(){
    std::cout << "Trying to render stuff on the screen\n";
}

void VulkanRenderer::deleteJob(int jobID){
    // Implementation is still not synced with drawing
    auto found = this->jobs.find(jobID);
    if (found == this->jobs.end()){
        throw std::runtime_error(std::string("JobID: "+ jobID) + " Is not exists!");
    }

    this->jobs.erase(found);
}


int VulkanRenderer::addJob(RenderJob job){
    // Implementation is still not synced with drawing
    const int MAXIMUM_RETRIES = 10;
    int jobID = 0;
    int retries = 0;
    std::map<int, RenderJob>::iterator found;
    do{
        jobID = rand() / INT16_MAX;
        found = this->jobs.find(jobID);
        if (MAXIMUM_RETRIES == retries++){
            throw std::runtime_error("Failed to add render job, nax retries reached!");
        }
    }while(found != this->jobs.end());

    this->jobs.insert(std::make_pair(jobID, job));

    return jobID;
}