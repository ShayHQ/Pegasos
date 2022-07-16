add_library(VulkanRenderer 
    VulkanRenderer.cpp VulkanRendererInstance.cpp VulkanRendererDevice.cpp VulkanRendererPresentation.cpp VulkanRendererDeviceQueuesFamilies.cpp VulkanMesh.cpp
)

add_library(GLRenderer 
    GLRenderer.cpp
    GLRendererPlan.cpp
)

add_library(Renderer 
    RendererDefaults.cpp
)

add_subdirectory(VulkanPipelinePlans)
add_subdirectory(GLPipelinePlans)

target_link_libraries(VulkanRenderer glfw glm::glm Vulkan::Vulkan glm::glm VulkanPipelinesPlans)
target_link_libraries(GLRenderer glfw glm::glm GLAD_LIB GLPipelinePlans)
target_link_libraries(Renderer VulkanRenderer GLRenderer)

