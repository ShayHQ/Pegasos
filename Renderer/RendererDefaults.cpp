#include "../Window/Window.hpp"
#include "PegasosRenderer.hpp"
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
    case RendererPlans::OPEN_GL_API:
        this->plan = new GLRendererPlan(window);
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

void Renderer::deleteJob(int jobID){
    // Implementation is still not synced with drawing
    auto found = this->jobs.find(jobID);
    if (found == this->jobs.end()){
        throw std::runtime_error(std::string("JobID: "+ jobID) + " Is not exists!");
    }

    this->jobs.erase(found);
    recordJobs();
}


int Renderer::addJob(std::vector<Vertex> job){
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

    this->jobs.insert(std::make_pair(jobID, createJob(job)));
    recordJobs();
    return jobID;
}