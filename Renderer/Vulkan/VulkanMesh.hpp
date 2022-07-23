#include "Renderer.hpp"
#include "VulkanRenderer.hpp"

namespace Pegasos{
    class VulkanMesh : public RenderJob
    {
    private:
        VulkanRenderer* rendererRef;
        
        VkDevice deviceRef;
        VkPhysicalDeviceMemoryProperties memoryProps;
        VkQueue transfer;
        VkBuffer bufferRef;
        VkDeviceMemory bufferMemoryRef;
        VkBuffer stagedRef;
        VkDeviceMemory stagedMemoryRef;
        uint64_t offset;
        uint32_t meshSize;
        bool created;
    public:
        VulkanMesh(VulkanRenderer* renderer, std::vector<Vertex> vertecies);
        void* data() override;
        uint32_t size() override;
        ~VulkanMesh() override;
    private:
        void allocateBuffer(VkBuffer& bufferRef, VkDeviceMemory& memRef, VkMemoryPropertyFlags memProps, VkBufferUsageFlags usage); 
        void copyStagedToFinalBuffers();
        void createMesh(std::vector<Vertex> vertecies);
    };    
}