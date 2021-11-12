#include "Renderer.hpp"


using namespace craze;


void QueueFamiliyIndices::findFamilies(VkPhysicalDevice physicalDevice){

}


bool QueueFamiliyIndices::isValid(){
    return graphicsFamily > -1 && presentFamiliy > -1;
}