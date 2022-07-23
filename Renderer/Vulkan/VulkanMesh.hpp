#include "Renderer.hpp"
#include "VulkanRenderer.hpp"

namespace Pegasos{
    class VulkanMesh : public RenderJob
    {
    private:
        VkDevice deviceRef;
        VkPhysicalDeviceMemoryProperties memoryProps;

        VkBuffer bufferRef;
        VkDeviceMemory bufferMemoryRef;
        uint64_t offset;
        uint32_t meshSize;
        bool created;
    public:
        VulkanMesh(VkDevice device, VkPhysicalDeviceMemoryProperties memoryProps, std::vector<Vertex> vertecies);
        void* data() override;
        uint32_t size() override;
        ~VulkanMesh() override;
    private:
        void allocateBuffer(VkBuffer& bufferRef, VkDeviceMemory& memRef, VkMemoryPropertyFlags memProps, VkBufferUsageFlags usage); 
        void createMesh(std::vector<Vertex> vertecies);
    };    
}