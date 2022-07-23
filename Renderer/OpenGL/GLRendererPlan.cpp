#include "Renderer.hpp"
#include "GLRenderer.hpp"
#include "../Window/Window.hpp"
#include <GLFW/glfw3.h>

using namespace Pegasos;

GLRendererPlan::GLRendererPlan(Window* window){
    this->window = window;
    this->details.isGLADInit = false;
}

GLRendererPlan::~GLRendererPlan(){
    // if GLRenderer fails there is nothing to do since glad is only loading opengl functions
    // no memory was allocated 
}

Renderer* GLRendererPlan::getRenderer(){
    this->wasBuilt = true;
    return new GLRenderer(this->details);
}


void  GLRendererPlan::initRendererAPI(){
    glfwMakeContextCurrent(static_cast<GLFWwindow*>(this->window->getHandler()));
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        throw std::runtime_error("Failed to initialize GLAD");
    }
    this->details.isGLADInit = true;
}
void  GLRendererPlan::createPresentation(){
    glfwGetWindowSize(static_cast<GLFWwindow*>(this->window->getHandler()), 
                    &this->details.width,  
                    &this->details.height);
    glViewport(0, 0, this->details.width, this->details.height);
}
