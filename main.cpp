#include "Pegasos.hpp"
#include <iostream>


int main(){
    Pegasos::Window window("My Window", 600, 800);
    Pegasos::RendererPlan* plan = new Pegasos::VulkanRendererPlan(&window);
    plan->initRendererAPI();
    plan->createPresentation();
    window.run();
    delete plan;
    return 0;
}