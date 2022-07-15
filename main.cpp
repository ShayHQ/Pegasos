#include "Pegasos.hpp"
#include <iostream>


int main(){
    try{
        Pegasos::Window window("My Window", 600, 800);
        Pegasos::RendererBuilder builder(Pegasos::RendererPlans::VULKAN_API, &window);
        builder.build();
        auto* vulkanRenderer = builder.getRenderer();
        VoidCallback drawOp = static_cast<VoidCallback>(std::bind(&Pegasos::Renderer::drawJobs, vulkanRenderer));
        window.addCycleCallback(&drawOp);
        window.run();
        delete vulkanRenderer;
    }catch(std::exception& err){
        std::cerr << err.what() << "\n";
        return 255;
    }
    return 0;
}