#include "Pegasos.hpp"
#include <iostream>


int main(int argc, char ** argv ){
    // try{
        std::vector<Pegasos::Vertex> vertecies1 = {
            {{0.0f, -0.7f, 0.0f},{1.0f, 0.0f, 0.0f}},
            {{0.4f, 0.4f, 0.0f},{0.0f, 1.0f, 0.0f}},
            {{-0.4f, 0.4f, 0.0f},{0.0f, 0.0f, 1.0f}}  
        };
        std::vector<Pegasos::Vertex> vertecies2 = {
            {{0.0f, 0.7f, 0.0f},{0.0f, 0.0f, 1.0f}},
            {{-0.4f, -0.4f, 0.0f},{1.0f, 0.0f, 0.0f}},
            {{0.4f, -0.4f, 0.0f},{0.0f, 1.0f, 0.0f}}  
        };
        auto usedAPI = Pegasos::RendererPlans::VULKAN_API;
        if (argc > 1){
            usedAPI = Pegasos::RendererPlans::OPEN_GL_API;
        }
        Pegasos::Window window("My Window", 600, 800, usedAPI);
        Pegasos::RendererBuilder builder(usedAPI, &window);
        builder.build();
        auto* vulkanRenderer = builder.getRenderer();
        vulkanRenderer->addJob(vertecies1);
        vulkanRenderer->addJob(vertecies2);
        VoidCallback drawOp = static_cast<VoidCallback>(std::bind(&Pegasos::Renderer::drawJobs, vulkanRenderer));
        window.addCycleCallback(&drawOp);
        window.run();
        delete vulkanRenderer;
    // }catch(std::exception& err){
    //     std::cerr << err.what() << "\n";
    //     return 255;
    // }
    return 0;
}