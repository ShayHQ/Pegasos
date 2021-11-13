#include "Window.hpp"

using namespace Pegasos;

#ifdef Pegasos_GLFW
#include <GLFW/glfw3.h>


Window::Window(char* windowName, uint32_t height, uint32_t width, uint32_t apiHint){
    this->details.height = height;
    this->details.width = width;
    this->details.windowName = windowName;
    this->innerClose = false;

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, apiHint);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    this->handler = glfwCreateWindow(width, height, static_cast<const char*>(windowName), nullptr, nullptr);
}

Window::~Window(){
    glfwDestroyWindow(static_cast<GLFWwindow*>(this->handler));
    glfwTerminate();
}

void Window::run(){
    while (!glfwWindowShouldClose(this->handler)){
        callbackMutex.lock();

        for(const auto& callback : this->cycleCallbacks){
            (*callback)();
        }

        glfwPollEvents();

        callbackMutex.unlock();
    }
}

GLFWwindow* Window::getHandler(){
    return this->handler;
}

#endif

bool Window::addCycleCallback(VoidCallback* callback){
    bool success = false;
    callbackMutex.lock();
    if (callback){
        cycleCallbacks.push_back(callback);
        success = true;
    }
    callbackMutex.unlock();
    return success;
}

bool Window::rmCycleCallback(VoidCallback* callback){
    bool success = false;
    callbackMutex.lock();
    if (callback){
        for (int i = 0; i < cycleCallbacks.size(); i++){
            if (cycleCallbacks[i] == callback){
                cycleCallbacks.erase(cycleCallbacks.begin()+i);
                success = true;
                break;
            }
        }
    }
    callbackMutex.unlock();
    return success;
}     
