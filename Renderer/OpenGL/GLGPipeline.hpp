#include <glad/glad.h>
#include <functional>

namespace Pegasos{

    class GLRenderer;
    struct GLGPipeline{
        // TBD
        GLuint program;
        std::function<void(GLuint)> applyVertexAttributes;
    };
    

    class GLPipelinePlan{
    // Execution by this order
    public:
        virtual void loadShaders()              = 0;
        virtual void setFixedFunctions()        = 0;
        virtual void createPipeline()           = 0;

        virtual GLGPipeline getPipeline()   = 0;
    };

    class GLBasicPipelinePlan : public GLPipelinePlan{
    protected:
        GLRenderer* renderer;

        GLGPipeline pipeline;
    public:
        virtual void loadShaders() override;
        virtual void setFixedFunctions() override;
        virtual void createPipeline() override;

        virtual GLGPipeline getPipeline() override;

        GLBasicPipelinePlan(GLRenderer* renderer);
    };
};