#include "../Renderer.hpp"
#include "../VulkanRenderer.hpp"
#include "../VulkanGPipeline.hpp"

using namespace Pegasos;

void setAttachments(std::vector<VkAttachmentDescription>& colourAttachments, std::vector<VkAttachmentReference>& colourAttachmentRefs, VkFormat& format){
	colourAttachments.push_back({});
	colourAttachmentRefs.push_back({});

	auto& colourAttachment = colourAttachments[0];
	auto& colourAttachmentReference = colourAttachmentRefs[0];

	colourAttachment.format = format;
	colourAttachment.samples = VK_SAMPLE_COUNT_1_BIT;					// Number of samples to write for multisampling
	colourAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;				// Describes what to do with attachment before rendering
	colourAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;			// Describes what to do with attachment after rendering
	colourAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;	// Describes what to do with stencil before rendering
	colourAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;	// Describes what to do with stencil after rendering

	colourAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colourAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;		// Image data layout after render pass (to change to)

	colourAttachmentReference.attachment = 0;
	colourAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
}

void setSubpass(VkSubpassDescription& subpass, std::vector<VkSubpassDependency>& subpassDependencies, std::vector<VkAttachmentReference>& colourAttachmentRefs){
	subpassDependencies.resize(2);
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = static_cast<uint32_t>(colourAttachmentRefs.size());
	subpass.pColorAttachments = colourAttachmentRefs.data();


	subpassDependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;		
	subpassDependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	subpassDependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;		
	subpassDependencies[0].dstSubpass = 0;
	subpassDependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	subpassDependencies[0].dependencyFlags = 0;

	subpassDependencies[1].srcSubpass = 0;											
	subpassDependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;;

	subpassDependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	subpassDependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	subpassDependencies[1].dependencyFlags = 0;
}


void VulkanBasicPipelinePlan::createRenderPass(){
	setAttachments(this->colourAttachments, this->colourAttachmentRefs, this->renderer->swapchainDetails.swapChainImageFormat.format);
	setSubpass(this->subpass, this->subpassDependencies, this->colourAttachmentRefs);

	VkRenderPassCreateInfo& renderPassCreateInfo = this->renderPassCreateInfo;
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(this->colourAttachments.size());
	renderPassCreateInfo.pAttachments = this->colourAttachments.data();
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &this->subpass;
	renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(this->subpassDependencies.size());
	renderPassCreateInfo.pDependencies = this->subpassDependencies.data();

	VkResult result = vkCreateRenderPass(this->renderer->device, &renderPassCreateInfo, nullptr, &this->pipeline.renderPass);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a Render Pass!");
	}
}

