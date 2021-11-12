#include "Crazy.hpp"
#include <iostream>

int main(){
    craze::Window window("My Window", 600, 800);
    craze::RendererPlan* plan = new craze::VulkanRendererPlan(&window);
    plan->initRendererAPI();
    window.run();
    return 0;
}