#include "Arctix/Renderer/Backend/Vulkan/VkHelper.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Misc/String/AXString.h"
#include "Arctix/Core/Platform/Window/Window.h"
#include "Arctix/Core/Containers/Array/Array.h"
#include "Arctix/Core/Containers/String/String.h"
#include "Arctix/Core/Logging/Logging.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"

#include <SDL2/SDL_vulkan.h>


AX_API
Int32
AX_Renderer_Backend_Vulkan_Helper_SearchMemoryIndex
(const SVulkanContext *context, const VkMemoryPropertyFlags properties, const UInt32 filter)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(context->device.gpu.instance, &memProperties);

	for (UInt32 i = 0; i < memProperties.memoryTypeCount; i++) {
		if (AX_BIT_FLAG_IF_SET(filter, AX_BIT_FLAG_FIELD(i)) &&
			AX_BIT_FLAG_IF_ALL_SET(memProperties.memoryTypes[i].propertyFlags, properties)) {
			return i;
		}
	}

	return -1;
}

AX_API
RawString
AX_Renderer_Backend_Vulkan_Helper_GetDeviceTypeString
(const VkPhysicalDeviceType type)
{
	switch (type) {
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			return "Integrated GPU";

		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			return "Discrete GPU";

		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			return "Virtual GPU";

		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			return "CPU";

		case VK_PHYSICAL_DEVICE_TYPE_OTHER:
			return "Other";

		default:
			return "Unknown Device";
	}
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_CreateWindowSurface
(const SVulkanContext *context, VkSurfaceKHR *outSurface)
{
	if (!context || !outSurface)
		return false;

	SDL_Window *contextWindow = AX_Window_GetSDLWindow(context->window);
	return SDL_Vulkan_CreateSurface(contextWindow, context->instance, outSurface);
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_QuerySwapchainSupportDetails
(const SVulkanContext *context, const VkPhysicalDevice gpu, const VkSurfaceKHR surface, SVulkanSwapchainSupportDetails *outSupportDetails)
{
	// surface capabilities
	AX_VK_ASSERT(
		context->instanceCallbacks.GetPhysicalDeviceSurfaceCapabilitiesKHR(
			gpu,
			surface,
			&(outSupportDetails->surfaceCapabilities)
		)
	);

	// surface formats
	{
		AX_VK_ASSERT(
			context->instanceCallbacks.GetPhysicalDeviceSurfaceFormatsKHR(
				gpu,
				surface,
				&(outSupportDetails->surfaceFormatCount),
				NULL
			)
		);

		AX_RENDERER_ALLOCATE(VkSurfaceFormatKHR, outSupportDetails->surfaceFormatCount, formats);
		outSupportDetails->surfaceFormats = formats;

		AX_VK_ASSERT(
			context->instanceCallbacks.GetPhysicalDeviceSurfaceFormatsKHR(
				gpu,
				surface,
				&(outSupportDetails->surfaceFormatCount),
				outSupportDetails->surfaceFormats
			)
		);
	}

	// present modes
	{
		AX_VK_ASSERT(
			context->instanceCallbacks.GetPhysicalDeviceSurfacePresentModesKHR(
				gpu,
				surface,
				&(outSupportDetails->presentModeCount),
				NULL
			)
		);

		AX_RENDERER_ALLOCATE(VkPresentModeKHR, outSupportDetails->presentModeCount, modes);
		outSupportDetails->presentModes = modes;

		AX_VK_ASSERT(
			context->instanceCallbacks.GetPhysicalDeviceSurfacePresentModesKHR(
				gpu,
				surface,
				&(outSupportDetails->presentModeCount),
				outSupportDetails->presentModes
			)
		);
	}

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_GetQueue
(const SVulkanContext *context, const UInt32 familyIndex, VkQueue *outQueue)
{
	if (!context || !outQueue)
		return false;

	vkGetDeviceQueue(context->device.instance, familyIndex, 0, outQueue);
	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_SearchDepthFormat
(SVulkanDevice *outDevice)
{
	VkFormat potentialFormats[] = {
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D24_UNORM_S8_UINT
	};

	UInt32 flags = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

	for (UInt32 i = 0; i < AX_STATIC_ARRAY_SIZE(potentialFormats); ++i) {
		VkFormatProperties properties;
		vkGetPhysicalDeviceFormatProperties(outDevice->gpu.instance, potentialFormats[i], &properties);

		Bool found =
			AX_BIT_FLAG_IF_ALL_SET(properties.linearTilingFeatures, flags) ||
			AX_BIT_FLAG_IF_ALL_SET(properties.optimalTilingFeatures, flags);

		if (found) {
			outDevice->depthFormat = potentialFormats[i];
			return true;
		}
	}

	return false;
}

AX_API
VkSurfaceFormatKHR
AX_Renderer_Backend_Vulkan_Helper_SelectSurfaceFormat
(const VkSurfaceFormatKHR *surfaceFormats, const UInt32 formatCount)
{
	if (!surfaceFormats)
		return AX_STRUCT(VkSurfaceFormatKHR, 0);

	for (UInt32 i = 0; i < formatCount; i++) {
		const VkFormat format = surfaceFormats[i].format;

		if (
			(format == VK_FORMAT_R8G8B8A8_UNORM) ||
			(format == VK_FORMAT_B8G8R8A8_UNORM) ||
			(format == VK_FORMAT_A2B10G10R10_UNORM_PACK32) ||
			(format == VK_FORMAT_A2R10G10B10_UNORM_PACK32) ||
			(format == VK_FORMAT_R16G16B16A16_SFLOAT)
			) {
			return surfaceFormats[i];
		}
	}

	AX_ASSERT(formatCount >= 1);

	AX_LOG_DEBUG("Engine", "Failed to find preferred format! Resorting to first available format...");
	return surfaceFormats[0];
}

AX_API
VkPresentModeKHR
AX_Renderer_Backend_Vulkan_Helper_SelectPresentMode
(const VkPresentModeKHR *presentModes, const UInt32 modeCount)
{
	if (!presentModes)
		return VK_PRESENT_MODE_FIFO_KHR;

	// best: VK_PRESENT_MODE_MAILBOX_KHR
	for (UInt32 i = 0; i < modeCount; i++) {
		VkPresentModeKHR mode = presentModes[i];

		if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
			return mode;
	}

	AX_ASSERT(modeCount >= 1);

	AX_LOG_DEBUG("Engine", "Present mode VK_PRESENT_MODE_MAILBOX_KHR unavailable! Resorting to VK_PRESENT_MODE_FIFO_KHR...");
	return VK_PRESENT_MODE_FIFO_KHR;
}

AX_API
VkExtent2D
AX_Renderer_Backend_Vulkan_Helper_SelectSwapchainExtent
(const VkSurfaceCapabilitiesKHR capabilities, const VoidPtr window)
{
	if (!window)
		return AX_STRUCT(VkExtent2D, 0);

	if (capabilities.currentExtent.width != UINT32_MAX)
		return capabilities.currentExtent;

	UInt32 width, height;
	SDL_Window *sdlWindow = AX_Window_GetSDLWindow(window);

	SDL_Vulkan_GetDrawableSize(sdlWindow, &width, &height);

	VkExtent2D actualExtent = {
		.width = AX_MATH_CLAMP(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
		.height = AX_MATH_CLAMP(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
	};

	return actualExtent;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_CreateImage
(
	const SVulkanContext *context,
	const VkImageType imageType,
	const UInt32 width,
	const UInt32 height,
	const VkFormat format,
	const VkImageTiling tiling,
	const VkImageUsageFlags usageFlags,
	const VkMemoryPropertyFlags memoryFlags,
	const Bool createView,
	const VkImageAspectFlags aspectFlags,
	SVulkanImage *outImage
)
{
	if (!context)
		return false;

	outImage->width = width;
	outImage->height = height;

	// create instance
	{
		VkImageCreateInfo createInfo = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,

			.imageType = VK_IMAGE_TYPE_2D,

			.extent = {
				.width = width,
				.height = height,
				.depth = 1
			},

			.mipLevels = 4,
			.arrayLayers = 1,

			.format = format,
			.tiling = tiling,

			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.usage = usageFlags,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE
		};

		AX_VK_ASSERT(
			vkCreateImage(
				context->device.instance, &createInfo, context->allocator, &(outImage->instance)
			)
		);
	}

	VkMemoryRequirements memReq;
	vkGetImageMemoryRequirements(context->device.instance, outImage->instance, &memReq);

	Int32 memIndex = AX_Renderer_Backend_Vulkan_Helper_SearchMemoryIndex(context, memoryFlags, memReq.memoryTypeBits);
	if (memIndex == -1)
		AX_LOG_ERROR("Engine", "Failed to detect memory index - invalid image!");

	// allocate memory
	{
		VkMemoryAllocateInfo allocateInfo = {
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.allocationSize = memReq.size,
			.memoryTypeIndex = memIndex
		};
		AX_VK_ASSERT(
			vkAllocateMemory(
				context->device.instance, &allocateInfo, context->allocator, &(outImage->memory)
			)
		);
	}

	// bind memory
	AX_VK_ASSERT(
		vkBindImageMemory(
			context->device.instance, outImage->instance, outImage->memory, 0
		)
	);

	// create owning image view
	if (createView) {
		if (!AX_Renderer_Backend_Vulkan_Helper_CreateImageView(context, format, aspectFlags, outImage, &(outImage->view)))
			return false;
	}

	return true;
}


AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_CreateImageView
(
	const SVulkanContext *context,
	const VkFormat format,
	const VkImageAspectFlags aspectFlags,
	const SVulkanImage *image,
	VkImageView *outImageView
)
{
	if (!context || !image)
		return false;

	VkImageViewCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,

		.image = image->instance,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = format,

		.subresourceRange = {
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1,
			.aspectMask = aspectFlags
		}
	};

	AX_VK_ASSERT(
		vkCreateImageView(
			context->device.instance, &createInfo, context->allocator, outImageView
		)
	);

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_DestroyImage
(const SVulkanContext *context, SVulkanImage *outImage)
{
	if (!context || !outImage)
		return false;

	vkDestroyImageView(context->device.instance, outImage->view, context->allocator);
	vkFreeMemory(context->device.instance, outImage->memory, context->allocator);
	vkDestroyImage(context->device.instance, outImage->instance, context->allocator);

	AX_HAL_Memory_Memzero(outImage, sizeof(SVulkanImage));
	return true;
}

AX_API
ShaderCode
AX_Renderer_Backend_Vulkan_Helper_GetShaderCode
(ReadOnlyString shaderFilePath, ByteSize *outShaderCodeSize)
{
	SFileHandle shaderFile;
	AX_ASSERT(AX_Filesystem_OpenFile(shaderFilePath, AX_FILE_IO_MODE_READ, AX_FILE_MODE_BINARY, &shaderFile));

	*outShaderCodeSize = AX_Filesystem_GetFileSize(&shaderFile);
	ShaderCode shaderCode = AX_Filesystem_ReadDataFromFile(&shaderFile, sizeof(ShaderByte), NULL);

	AX_ASSERT(AX_Filesystem_CloseFile(&shaderFile));
	return shaderCode;
}

AX_API AX_INLINE
VkShaderModuleCreateInfo
AX_Renderer_Backend_Vulkan_Helper_GetShaderModuleProperties
(const ByteSize shaderCodeSize, const ShaderCode shaderCode)
{
	return AX_STRUCT(
		VkShaderModuleCreateInfo,

		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,

		.codeSize = shaderCodeSize,
		.pCode = shaderCode
	);
}

AX_API AX_INLINE
VkPipelineShaderStageCreateInfo
AX_Renderer_Backend_Vulkan_Helper_GetShaderStageProperties
(
	const VkShaderStageFlagBits shaderStageFlag,
	const VkShaderModule module
)
{
	return AX_STRUCT(
		VkPipelineShaderStageCreateInfo,

		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		
		.stage = shaderStageFlag,
		.module = module,
		.pName = "main"
	);
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_CreateGraphicsPipeline
(
	const SVulkanContext *context,
	const SVulkanRenderPass *renderPass,
	const UInt32 attributeCount,
	const VkVertexInputAttributeDescription *attributes,
	const UInt32 descriptorSetLayoutCount,
	const VkDescriptorSetLayout *descriptorSetLayouts,
	const UInt32 stageCount,
	const VkPipelineShaderStageCreateInfo *stages,
	const VkViewport viewport,
	const VkRect2D scissor,
	const Bool wireframe,
	SVulkanPipeline *outGraphicsPipeline
)
{
	if (!context || !renderPass || !attributes /*|| !descriptorSetLayouts*/ || !stages)
		return false;

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = { 
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, 
		
		.viewportCount = 1,
		.pViewports = &viewport,
		
		.scissorCount = 1,
		.pScissors = &scissor
	};

	VkPipelineRasterizationStateCreateInfo rasterStateCreateInfo = { 
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, 
		
		.depthClampEnable = VK_FALSE,
		.rasterizerDiscardEnable = VK_FALSE,
		
		.polygonMode = wireframe ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL,
		.lineWidth = 1,
		.cullMode = VK_CULL_MODE_BACK_BIT,
		.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
		
		.depthBiasEnable = VK_FALSE,
		.depthBiasConstantFactor = 0,
		.depthBiasClamp = 0,
		.depthBiasSlopeFactor = 0
	};

	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		
		.sampleShadingEnable = VK_FALSE,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
		
		.minSampleShading = 1,
		.pSampleMask = 0,
		
		.alphaToCoverageEnable = VK_FALSE,
		.alphaToOneEnable = VK_FALSE
	};

	VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = { 
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		
		.depthTestEnable = VK_TRUE,
		.depthWriteEnable = VK_TRUE,
		
		.depthCompareOp = VK_COMPARE_OP_LESS,
		
		.depthBoundsTestEnable = VK_FALSE,
		.stencilTestEnable = VK_FALSE
	};

	VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {
		.blendEnable = VK_TRUE,
		
		.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
		.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		.colorBlendOp = VK_BLEND_OP_ADD,
		
		.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
		.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		.alphaBlendOp = VK_BLEND_OP_ADD,
		
		.colorWriteMask = 
			VK_COLOR_COMPONENT_R_BIT | 
			VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT | 
			VK_COLOR_COMPONENT_A_BIT
	};

	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = { 
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY,
		
		.attachmentCount = 1,
		.pAttachments = &colorBlendAttachmentState
	};

	VkDynamicState dynamicStates[] = {
	    VK_DYNAMIC_STATE_VIEWPORT,
	    VK_DYNAMIC_STATE_SCISSOR,
	    VK_DYNAMIC_STATE_LINE_WIDTH 
	};

	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		
		.dynamicStateCount = AX_STATIC_ARRAY_SIZE(dynamicStates),
		.pDynamicStates = dynamicStates
	};

	VkVertexInputBindingDescription bindingDesc = {
		.binding = 0,
		.stride = sizeof(SVertex),
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
	};

	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = { 
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO, 
		
		.vertexBindingDescriptionCount = 1,
		.pVertexBindingDescriptions = &bindingDesc,
		
		.vertexAttributeDescriptionCount = attributeCount,
		.pVertexAttributeDescriptions = attributes
	};

	VkPipelineInputAssemblyStateCreateInfo inputAsmStateCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		.primitiveRestartEnable = VK_FALSE
	};

	// pipeline layout
	{
		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,

			.setLayoutCount = descriptorSetLayoutCount,
			.pSetLayouts = descriptorSetLayouts
		};

		AX_VK_ASSERT(
			vkCreatePipelineLayout(
				context->device.instance,
				&pipelineLayoutCreateInfo,
				context->allocator,
				&(outGraphicsPipeline->layout)
			)
		);
	}

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = { 
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		
		.stageCount = stageCount,
		.pStages = stages,
		
		.pVertexInputState = &vertexInputStateCreateInfo,
		.pInputAssemblyState = &inputAsmStateCreateInfo,
		.pViewportState = &viewportStateCreateInfo,
		.pRasterizationState = &rasterStateCreateInfo,
		.pMultisampleState = &multisampleStateCreateInfo,
		.pDepthStencilState = &depthStencilStateCreateInfo,
		.pColorBlendState = &colorBlendStateCreateInfo,
		.pDynamicState = &dynamicStateCreateInfo,
		.pTessellationState = NULL,

		.layout = outGraphicsPipeline->layout,
		.renderPass = renderPass->instance,
		.subpass = 0,
		
		.basePipelineHandle = VK_NULL_HANDLE,
		.basePipelineIndex = -1
	};

	AX_VK_ASSERT(
		vkCreateGraphicsPipelines(
			context->device.instance, 
			VK_NULL_HANDLE, 
			1, 
			&graphicsPipelineCreateInfo, 
			context->allocator, 
			&(outGraphicsPipeline->instance)
		)
	);

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_DestroyGraphicsPipeline
(const SVulkanContext *context, SVulkanPipeline *outGraphicsPipeline)
{
	if (!context || !outGraphicsPipeline)
		return false;

	vkDestroyPipeline(context->device.instance, outGraphicsPipeline->instance, context->allocator);
	vkDestroyPipelineLayout(context->device.instance, outGraphicsPipeline->layout, context->allocator);

	AX_HAL_Memory_Memzero(outGraphicsPipeline, sizeof(SVulkanPipeline));
	
	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_BindGraphicsPipeline
(const SVulkanCommandBuffer *commandBuffer, const VkPipelineBindPoint bindPoint, SVulkanPipeline *outGraphicsPipeline)
{
	if (!commandBuffer || !outGraphicsPipeline)
		return false;

	vkCmdBindPipeline(commandBuffer->instance, bindPoint, outGraphicsPipeline->instance);

	return true;
}




