#include "../Window/Window.hpp"
#include "Renderer.hpp"
#include <exception>

using namespace Pegasos;

RendererPlan::~RendererPlan(){}
Renderer::~Renderer(){}

RenderJob::~RenderJob(){}
RendererBuilder::RendererBuilder(RendererPlans plan, Window* window){
    switch (plan)
    {
    case RendererPlans::VULKAN_API:
        this->plan = new VulkanRendererPlan(window);
        break;
    
    default:
        throw std::runtime_error("Unvalid renderer plan");
        break;
    }
}

void RendererBuilder::build(){
    if (this->built){
        throw std::runtime_error("Can't build twice for the same plan instance!");
    }
    this->plan->initRendererAPI();
    this->plan->createPresentation();

    this->built = true;
}

Renderer* RendererBuilder::getRenderer(){
    if (!this->renderer){
        if (this->built){
            this->renderer = this->plan->getRenderer();
        }
    }

    return this->renderer;
}

RendererBuilder::~RendererBuilder(){
    delete this->plan;
}