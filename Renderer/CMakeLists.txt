include_directories(./ ./Vulkan ./OpenGL)

add_library(VulkanRenderer 
    Vulkan/VulkanRenderer.cpp 
    Vulkan/VulkanRendererInstance.cpp 
    Vulkan/VulkanRendererDevice.cpp 
    Vulkan/VulkanRendererPresentation.cpp 
    Vulkan/VulkanRendererDeviceQueuesFamilies.cpp
    Vulkan/VulkanMesh.cpp
)

add_library(GLRenderer 
    OpenGL/GLRenderer.cpp
    OpenGL/GLRendererPlan.cpp
    OpenGL/GLMesh.cpp
)

add_library(Renderer 
    RendererDefaults.cpp
)

add_subdirectory(Vulkan/VulkanPipelinePlans)
add_subdirectory(OpenGL/GLPipelinePlans)

target_link_libraries(VulkanRenderer glfw glm::glm Vulkan::Vulkan glm::glm VulkanPipelinesPlans)
target_link_libraries(GLRenderer glfw glm::glm GLAD_LIB GLPipelinePlans)
target_link_libraries(Renderer VulkanRenderer GLRenderer)

