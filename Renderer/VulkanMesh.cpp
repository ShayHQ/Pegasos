#include "./VulkanMesh.hpp"

#include <cstring>

using namespace Pegasos;

VulkanMesh::VulkanMesh(VkDevice device, VkPhysicalDeviceMemoryProperties memoryProps, std::vector<Vertex> vertecies){
    this->deviceRef = device;
    this->memoryProps = memoryProps;
    this->bufferRef = VK_NULL_HANDLE;
    createBuffer(vertecies);
}


void* VulkanMesh::data(){
    return reinterpret_cast<void*>(this->bufferRef);
}

uint32_t VulkanMesh::size(){
    return this->meshSize; 
}

VulkanMesh::~VulkanMesh(){
    if (this->created){
        vkDestroyBuffer(this->deviceRef, this->bufferRef, nullptr);
        vkFreeMemory(this->deviceRef, this->bufferMemoryRef, nullptr);
    }
}


// Taken
uint32_t findMemoryTypeIndex(VkPhysicalDeviceMemoryProperties deviceMemProps,uint32_t allowedTypes, VkMemoryPropertyFlags properties)
{
	for (uint32_t i = 0; i < deviceMemProps.memoryTypeCount; i++)
	{
		if ((allowedTypes & (1 << i))														// Index of memory type must match corresponding bit in allowedTypes
			&& (deviceMemProps.memoryTypes[i].propertyFlags & properties) == properties)	// Desired property bit flags are part of memory type's property flags
		{
			// This memory type is valid, so return its index
			return i;
		}
	}
    return -1;
}

void VulkanMesh::createBuffer(std::vector<Vertex> vertecies){
    this->created = true;
    this->offset = 0;
    void* dataPtr;

    VkMemoryRequirements requiremets;
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.size = vertecies.size() * sizeof(Vertex);

    if (vkCreateBuffer(this->deviceRef, &bufferInfo, nullptr, &this->bufferRef) != VK_SUCCESS){
        throw std::runtime_error("Failed to create vertex buffer reference");
    }

    vkGetBufferMemoryRequirements(this->deviceRef, this->bufferRef, &requiremets);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = requiremets.size;
    allocInfo.memoryTypeIndex = findMemoryTypeIndex(this->memoryProps, requiremets.memoryTypeBits, 
                                                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    
    if (vkAllocateMemory(this->deviceRef, &allocInfo, nullptr, &this->bufferMemoryRef) != VK_SUCCESS){
        throw std::runtime_error("Failed to allocate memory for vertex buffer");
    }

    vkBindBufferMemory(this->deviceRef, this->bufferRef, this->bufferMemoryRef, static_cast<VkDeviceSize>(this->offset));

    vkMapMemory(this->deviceRef, this->bufferMemoryRef, static_cast<VkDeviceSize>(this->offset), bufferInfo.size, 0, &dataPtr);
    std::memcpy(dataPtr, vertecies.data(), (size_t)bufferInfo.size);
    vkUnmapMemory(this->deviceRef, this->bufferMemoryRef);
    this->meshSize = vertecies.size();
}