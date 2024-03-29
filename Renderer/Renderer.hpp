#pragma once
#include <map>
#include <vector>
#include <glm/glm.hpp>

#ifdef _WIN32
#include <stdexcept>
#else
#include <cstring>
#endif

namespace Pegasos{
    struct Vertex{
        glm::vec3 position;
        glm::vec3 color;
    };

    class RenderJob{
    public:
        virtual uint32_t size() = 0;
        virtual void* data()    = 0;
        virtual ~RenderJob()    = 0;
    };

    class Window;
    class Renderer
    {
    protected:
        std::map<int, RenderJob*> jobs;
        virtual void recordJobs()           = 0;
        
        virtual RenderJob* createJob(std::vector<Vertex>) = 0;
    public:
        int addJob(std::vector<Vertex> job);
        void deleteJob(int jobID);
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
        VULKAN_API,
        OPEN_GL_API
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