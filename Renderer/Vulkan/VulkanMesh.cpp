#include "./VulkanMesh.hpp"

#include <cstring>

using namespace Pegasos;

VulkanMesh::VulkanMesh(VulkanRenderer* renderer, std::vector<Vertex> vertecies){
    this->deviceRef = renderer->device;
    this->memoryProps = renderer->physicalDetails.deviceMemProps;
    this->bufferRef = VK_NULL_HANDLE;
    this->transfer = transfer;

    this->rendererRef = renderer;
    createMesh(vertecies);
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

void VulkanMesh::allocateBuffer(VkBuffer& bufferRef, VkDeviceMemory& memRef, VkMemoryPropertyFlags memProps, VkBufferUsageFlags usage){
    VkMemoryRequirements requiremets;
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.size = this->meshSize * sizeof(Vertex);

    if (vkCreateBuffer(this->deviceRef, &bufferInfo, nullptr, &bufferRef) != VK_SUCCESS){
        throw std::runtime_error("Failed to create vertex buffer reference");
    }

    vkGetBufferMemoryRequirements(this->deviceRef, bufferRef, &requiremets);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = requiremets.size;
    allocInfo.memoryTypeIndex = findMemoryTypeIndex(this->memoryProps, requiremets.memoryTypeBits, 
                                                    memProps);
    
    if (vkAllocateMemory(this->deviceRef, &allocInfo, nullptr, &memRef) != VK_SUCCESS){
        throw std::runtime_error("Failed to allocate memory for vertex buffer");
    }

    vkBindBufferMemory(this->deviceRef, bufferRef, memRef, static_cast<VkDeviceSize>(this->offset));
}

void VulkanMesh::createMesh(std::vector<Vertex> vertecies){
    this->created = true;
    this->offset = 0;
    void* dataPtr;
    this->meshSize = vertecies.size();
    size_t totalSize = this->meshSize * sizeof(Vertex);

    allocateBuffer(this->stagedRef, this->stagedMemoryRef, 
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

    vkMapMemory(this->deviceRef, this->stagedMemoryRef, static_cast<VkDeviceSize>(this->offset), totalSize, 0, &dataPtr);
    std::memcpy(dataPtr, vertecies.data(), totalSize);
    vkUnmapMemory(this->deviceRef, this->stagedMemoryRef);

    allocateBuffer(this->bufferRef, this->bufferMemoryRef, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT , VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);

    copyStagedToFinalBuffers();
}

void VulkanMesh::copyStagedToFinalBuffers(){
    VkCommandBuffer copyCmd;

    VkCommandBufferAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.commandPool = this->rendererRef->cmdPool;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(this->deviceRef, &allocateInfo, &copyCmd) != VK_SUCCESS){
        throw std::runtime_error("Failed to allocate command buffer for transfer!");
    }

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pInheritanceInfo = nullptr;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    beginInfo.pNext = nullptr;

    vkBeginCommandBuffer(copyCmd, &beginInfo);
    VkBufferCopy cpyRegion;
    cpyRegion.dstOffset = 0;
    cpyRegion.srcOffset = 0;
    cpyRegion.size = this->meshSize * sizeof(Vertex);
    vkCmdCopyBuffer(copyCmd, this->stagedRef, this->bufferRef, 1, &cpyRegion);
    vkEndCommandBuffer(copyCmd);
    
    VkSubmitInfo submitInfo = {};
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &copyCmd;
    submitInfo.pNext = nullptr;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    vkQueueSubmit(this->rendererRef->transfer, 1, & submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(this->rendererRef->transfer);

    vkFreeCommandBuffers(this->deviceRef, this->rendererRef->cmdPool, 1, &copyCmd);
    vkDestroyBuffer(this->deviceRef, this->stagedRef, nullptr);
    vkFreeMemory(this->deviceRef, this->stagedMemoryRef, nullptr);
}