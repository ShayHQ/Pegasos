#include "Renderer.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanGPipeline.hpp"
#include "VulkanMesh.hpp"
#include <iostream>

#define MAX_FPS_DRAW 2
#define MAX_TIMEOUT static_cast<uint64_t>(-1)
using namespace Pegasos;

VulkanRenderer::VulkanRenderer(VulkanDetails& details){
    this->instance          = details.instance;
    this->graphic           = details.graphic;
    this->present           = details.present;
    this->transfer          = details.transfer;
    this->compute           = details.compute;
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
    createCmdBuffers();
    recordJobs();
}

VulkanRenderer::~VulkanRenderer(){
    vkDeviceWaitIdle(this->device);
    for (const auto& job : this->jobs){
        delete job.second;
    }
    this->jobs.clear();

    vkDestroyCommandPool(this->device, this->cmdPool, nullptr);
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
        std::vector<VkImageView> attachments = {
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

void VulkanRenderer::createCmdBuffers(){
    VkCommandPoolCreateInfo createInfo = {};
    createInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.queueFamilyIndex = this->physicalDetails.families.graphicsFamily;
    createInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(this->device, &createInfo, nullptr, &this->cmdPool) != VK_SUCCESS){
        throw std::runtime_error("Failed to create command pool");
    }

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = this->cmdPool;
    allocInfo.commandBufferCount = this->swapchainDetails.framebuffers.size();
    allocInfo.level  = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    this->commandBuffers.resize(allocInfo.commandBufferCount);
    if (vkAllocateCommandBuffers(this->device, &allocInfo, this->commandBuffers.data()) != VK_SUCCESS){
        throw std::runtime_error("Failed to allocate command buffers!");
    }
}

void VulkanRenderer::recordJobs(){
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    VkRenderPassBeginInfo renderPassBeginInfo = {};
    VkClearValue clearVals[] = {
        {0.0f, 0.0f, 0.0f, 0.0f}
    };
    renderPassBeginInfo.pClearValues = clearVals;
    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.renderArea.extent = this->swapchainDetails.swapChainExtent;
    renderPassBeginInfo.renderArea.offset = {0, 0};
    renderPassBeginInfo.renderPass = this->pipeline->renderPass;
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    
    for (int i = 0; i < this->commandBuffers.size(); i++){
        renderPassBeginInfo.framebuffer = this->swapchainDetails.framebuffers[i];
        vkResetCommandBuffer(this->commandBuffers[i], 0);
        if (vkBeginCommandBuffer(this->commandBuffers[i], &beginInfo) != VK_SUCCESS){
            throw std::runtime_error("Failed to start record cmd buffer");
        }
        {
            vkCmdBeginRenderPass(this->commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
            {
                for (auto& job : this->jobs){
                    vkCmdBindPipeline(this->commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, this->pipeline->pipelineRef);
                    VkBuffer vertexeBuffers[] ={
                        reinterpret_cast<VkBuffer>(job.second->data())
                    };
                    VkDeviceSize vertexOffset = 0;
                    vkCmdBindVertexBuffers(this->commandBuffers[i], 0, 1, vertexeBuffers, &vertexOffset);
                    vkCmdDraw(this->commandBuffers[i], job.second->size(), 1, 0, 0);
                }
            }
            vkCmdEndRenderPass(this->commandBuffers[i]);
        }

        if (vkEndCommandBuffer(this->commandBuffers[i]) != VK_SUCCESS){
            throw std::runtime_error("Failed to end record cmd buffer");
        }
    }
}

void VulkanRenderer::drawJobs(){
    static int currentFrame;
    uint32_t imageIndex;

    vkWaitForFences(this->device, 1, &this->swapchainDetails.imagesInUse[currentFrame], VK_TRUE, MAX_TIMEOUT);

    vkResetFences(this->device, 1, &this->swapchainDetails.imagesInUse[currentFrame]);

    vkAcquireNextImageKHR(this->device, this->swapchainDetails.swapchain, MAX_TIMEOUT, 
                        this->swapchainDetails.signalsImageAvailable[currentFrame], VK_NULL_HANDLE, &imageIndex);
    VkSubmitInfo sendInfo = {};
    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };
    sendInfo.commandBufferCount = 1;
    sendInfo.pCommandBuffers = &this->commandBuffers[imageIndex];
    sendInfo.signalSemaphoreCount = 1;
    sendInfo.pWaitSemaphores = &this->swapchainDetails.signalsImageAvailable[currentFrame];
    sendInfo.pWaitDstStageMask = waitStages;
    sendInfo.waitSemaphoreCount = 1;
    sendInfo.pSignalSemaphores = &this->swapchainDetails.signalsImageRendered[currentFrame];
    sendInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;


    if (vkQueueSubmit(this->graphic, 1, &sendInfo ,this->swapchainDetails.imagesInUse[currentFrame]) != VK_SUCCESS){
        throw std::runtime_error("Failed to submit job to graphics queue");
    }

    VkPresentInfoKHR presentInfo = {};
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pSwapchains = &this->swapchainDetails.swapchain;
    presentInfo.swapchainCount = 1;
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pWaitSemaphores = &this->swapchainDetails.signalsImageRendered[currentFrame];
    presentInfo.waitSemaphoreCount = 1;
    
    if (vkQueuePresentKHR(this->present, &presentInfo) != VK_SUCCESS){
        throw std::runtime_error("Failed to submit present");
    }

    currentFrame = (currentFrame + 1) % MAX_FPS_DRAW;
}

RenderJob* VulkanRenderer::createJob(std::vector<Vertex> job){
    return new VulkanMesh(this, job);
}