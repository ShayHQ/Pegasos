add_library(VulkanPipelinesPlans FixedFunctions.cpp  PipelineCreator.cpp  PipelineLayout.cpp  ShadersLoader.cpp  RenderpassCreator.cpp)
target_link_libraries(VulkanPipelinesPlans glm::glm Vulkan::Vulkan)
