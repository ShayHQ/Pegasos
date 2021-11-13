#include "Pegasos.hpp"
#include <iostream>


int main(){
    Pegasos::Window window("My Window", 600, 800);
    Pegasos::RendererPlan* plan = new Pegasos::VulkanRendererPlan(&window);
    plan->initRendererAPI();
    plan->createPresentation();
    const auto* Renderer = plan->getRenderer();
    delete plan;
    window.run();
    delete Renderer;
    return 0;
}