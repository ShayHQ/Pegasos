#include "GLMesh.hpp"

using namespace Pegasos;

GLMesh::GLMesh(std::vector<Vertex> vertecies){
    glGenVertexArrays(1, &this->bufferID);
    glGenBuffers(1, &this->bufferID);
    if (!this->bufferID){
        throw std::runtime_error("failed to generate vertex buffer");
    }
    this->length = sizeof(Vertex) * vertecies.size();
    glBindBuffer(GL_ARRAY_BUFFER, this->bufferID);
    glBufferData(GL_ARRAY_BUFFER, this->length, vertecies.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
GLMesh::~GLMesh(){
    glDeleteBuffers(1, &this->bufferID);
    this->bufferID = 0;
    this->length = 0;
}
uint32_t GLMesh::size(){
    return this->length;
}
void* GLMesh::data(){
    return reinterpret_cast<void*>(&this->bufferID);
}