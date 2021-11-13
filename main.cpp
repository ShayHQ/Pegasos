#include "Pegasos.hpp"
#include <iostream>


int main(){
    Pegasos::Window window("My Window", 600, 800);
    Pegasos::RendererBuilder builder(Pegasos::RendererPlans::VULKAN_API, &window);
    builder.build();
    const auto* Renderer = builder.getRenderer();
    window.run();
    delete Renderer;
    return 0;
}