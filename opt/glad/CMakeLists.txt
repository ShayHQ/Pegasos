# glad directory content was created with https://glad.dav1d.de/
#https://glad.dav1d.de/#language=c&specification=gl&api=gl%3D3.3&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&profile=core&loader=on

add_library(GLAD_LIB src/glad.c)
target_include_directories(GLAD_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
