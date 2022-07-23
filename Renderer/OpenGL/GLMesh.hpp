#include "Renderer.hpp"
#include "GLRenderer.hpp"

namespace Pegasos{
    class GLMesh : public RenderJob
    {
    private:
        GLuint bufferID;
        GLsizei length;
    public:
        GLMesh(std::vector<Vertex> vertecies);
        virtual ~GLMesh() override;
        virtual uint32_t size() override;
        virtual void* data()    override;
    };
}
