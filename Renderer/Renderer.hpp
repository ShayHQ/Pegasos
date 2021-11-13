#pragma once
#include <map>

namespace Pegasos{
    struct Vertex{
        float x, y, z;
        float r, g, b;
    };

    struct RenderJob{
        std::vector<Vertex> vertcies;
        std::string vertexShader;
        std::string fragmentShader;
    };

    class Window;
    class Renderer
    {
    protected:
        std::map<int, RenderJob> jobs;
    public:
        virtual int addJob(RenderJob job)   = 0;
        virtual void deleteJob(int jobID)   = 0;
        virtual void drawJobs()             = 0;
        virtual ~Renderer()                 = 0;
    };

    class RendererPlan{
    public:
        virtual void initRendererAPI()      = 0;
        virtual void createPresentation()   = 0;
        virtual Renderer* getRenderer()     = 0;
        virtual ~RendererPlan()             = 0;
    };

    enum RendererPlans{
        VULKAN_API
    };
    class RendererBuilder{
        bool built = false;
        RendererPlan* plan;
        Renderer* renderer = nullptr;

    public:
        RendererBuilder(RendererPlans plan, Window* window);
        void build();
        Renderer* getRenderer();
        ~RendererBuilder();
    };
};


#include "VulkanRenderer.hpp"