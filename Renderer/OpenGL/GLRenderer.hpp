#pragma once
#include <glad/glad.h>
#include <optional>
#include <vector>

namespace Pegasos{
    class Window;

    struct GLDetails{
        GLfloat width, height;
        bool isGLADInit;
    };

    class GLBasicPipelinePlan;
    class GLGPipeline;
    class GLRenderer : public Renderer
    {
        GLGPipeline* pipeline;        
        GLDetails details;
        friend class GLBasicPipelinePlan;
    public:
        GLRenderer(GLDetails& details);
        ~GLRenderer() override;
    protected:
        virtual void recordJobs() override;
        virtual RenderJob* createJob(std::vector<Vertex>) override; 
    private:
        void createFramebuffers();
        void createCmdBuffers();
        virtual void drawJobs() override;
    };

    class GLRendererPlan : public RendererPlan{
        GLDetails details;
        Window* window;
        bool wasBuilt;
        std::vector<const char *> instancesExtensions;
        std::vector<const char *> deviceExtensions;
        
    public:
        GLRendererPlan(Window* window);
        void initRendererAPI() override;
        void createPresentation() override;
        Renderer* getRenderer() override;
        ~GLRendererPlan() override;
    };
};