add_library(GLPipelinePlans OpenGLGeneralPipeline.cpp GLFixedFunctions.cpp GLShaderLoader.cpp)

target_link_libraries(GLPipelinePlans GLAD_LIB glm::glm PegasosCommon)