#pragma once

namespace Pegasos{
    class Window;
    class Renderer
    {
    public:
        virtual ~Renderer() = 0;
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