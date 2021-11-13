#pragma once

namespace Pegasos{
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
};


#include "VulkanRenderer.hpp"