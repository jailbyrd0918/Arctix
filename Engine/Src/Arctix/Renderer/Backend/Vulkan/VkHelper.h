#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Renderer/Backend.h"
#include "Arctix/Defines/Renderer/VulkanTypes.h"


AX_API
Int32
AX_Renderer_Backend_Vulkan_Helper_SearchMemoryIndex
(const SVulkanContext *context, const VkMemoryPropertyFlags properties, const UInt32 filter);

AX_API
RawString
AX_Renderer_Backend_Vulkan_Helper_GetDeviceTypeString
(const VkPhysicalDeviceType type);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_CreateWindowSurface
(const SVulkanContext *context, VkSurfaceKHR *outSurface);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_QuerySwapchainSupportDetails
(const SVulkanContext *context, const VkPhysicalDevice gpu, const VkSurfaceKHR surface, SVulkanSwapchainSupportDetails *outSupportDetails);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_GetQueue
(const SVulkanContext *context, const UInt32 familyIndex, VkQueue *outQueue);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_SearchDepthFormat
(SVulkanDevice *outDevice);

AX_API
VkSurfaceFormatKHR
AX_Renderer_Backend_Vulkan_Helper_SelectSurfaceFormat
(const VkSurfaceFormatKHR *surfaceFormats, const UInt32 formatCount);

AX_API
VkPresentModeKHR
AX_Renderer_Backend_Vulkan_Helper_SelectPresentMode
(const VkPresentModeKHR *presentModes, const UInt32 modeCount);

AX_API
VkExtent2D
AX_Renderer_Backend_Vulkan_Helper_SelectSwapchainExtent
(const VkSurfaceCapabilitiesKHR capabilities, const VoidPtr window);

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
);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_CreateImageView
(
	const SVulkanContext *context,
	const VkFormat format,
	const VkImageAspectFlags aspectFlags,
	const SVulkanImage *image,
	VkImageView *outImageView
);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_DestroyImage
(const SVulkanContext *context, SVulkanImage *outImage);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_PerformImageLayoutTransition
(const SVulkanContext *context, const SVulkanCommandBuffer *commandBuffer, const SVulkanImage *image, const VkFormat format, const VkImageLayout oldLayout, const VkImageLayout newLayout);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_CopyImageFromBuffer
(const SVulkanContext *context, const SVulkanCommandBuffer *commandBuffer, const SVulkanImage *image, const VkBuffer buffer);

AX_API
ShaderCode
AX_Renderer_Backend_Vulkan_Helper_GetShaderCode
(ReadOnlyString shadersFilePath, ByteSize *outShadersCodeSize);

AX_API AX_INLINE
VkShaderModuleCreateInfo
AX_Renderer_Backend_Vulkan_Helper_GetShaderModuleProperties
(const ByteSize shaderCodeSize, const ShaderCode shaderCode);

AX_API AX_INLINE
VkPipelineShaderStageCreateInfo
AX_Renderer_Backend_Vulkan_Helper_GetShaderStageProperties
(
	const VkShaderStageFlagBits shaderStageFlag,
	const VkShaderModule module
);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_AcquireShaderResources
(const SVulkanContext *context, SVulkanShader *shader, UInt32 *outObjectID);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_ReleaseShaderResources
(const SVulkanContext *context, SVulkanShader *shader, const UInt32 objectID);

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
);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_DestroyGraphicsPipeline
(const SVulkanContext *context, SVulkanPipeline *outGraphicsPipeline);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Helper_BindGraphicsPipeline
(const SVulkanCommandBuffer *commandBuffer, const VkPipelineBindPoint bindPoint, SVulkanPipeline *outGraphicsPipeline);










