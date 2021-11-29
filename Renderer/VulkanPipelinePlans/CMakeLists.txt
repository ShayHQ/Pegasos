add_library(VulkanPipelinesPlans FixedFunctions.cpp  PipelineCreator.cpp  PipelineLayout.cpp  ShadersLoader.cpp  SubpassCreator.cpp)
target_link_libraries(VulkanPipelinesPlans glm::glm)
