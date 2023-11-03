#pragma once


#include "Arctix/Defines/Core/DataTypes.h"
#include "Arctix/Defines/Core/Math.h"

#include "Arctix/Core/Misc/Assertions/Assertions.h"

#include <vulkan/vulkan.h>

#define	AX_VK_ASSERT(condition)				AX_ASSERT((condition) == VK_SUCCESS)


typedef UInt32						ShaderByte;
typedef ShaderByte *					ShaderCode;

typedef
struct AX_Vulkan_Instance_Callbacks
{
	PFN_vkCreateDebugUtilsMessengerEXT		CreateDebugUtilsMessengerEXT;
	PFN_vkDestroyDebugUtilsMessengerEXT		DestroyDebugUtilsMessengerEXT;
	
	PFN_vkGetPhysicalDeviceSurfaceSupportKHR	GetPhysicalDeviceSurfaceSupportKHR;
	PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR	GetPhysicalDeviceSurfaceCapabilitiesKHR;
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR	GetPhysicalDeviceSurfaceFormatsKHR;
	PFN_vkGetPhysicalDeviceSurfacePresentModesKHR	GetPhysicalDeviceSurfacePresentModesKHR;
}
SVulkanInstanceCallbacks;

typedef
struct AX_Vulkan_Device_Callbacks
{
	PFN_vkCreateSwapchainKHR			CreateSwapchainKHR;
	PFN_vkDestroySwapchainKHR			DestroySwapchainKHR;
	
	PFN_vkGetSwapchainImagesKHR			GetSwapchainImagesKHR;
	PFN_vkAcquireNextImageKHR			AcquireNextImageKHR;
	PFN_vkQueuePresentKHR				QueuePresentKHR;
}
SVulkanDeviceCallbacks;

typedef
struct AX_Vulkan_GPU
{
	VkPhysicalDevice				instance;

	Int32						id;

	VkPhysicalDeviceProperties			properties;
	VkPhysicalDeviceMemoryProperties		memory;
	VkPhysicalDeviceFeatures			features;
}
SVulkanGPU;

typedef
struct AX_Vulkan_Swapchain_Support_Details
{
	VkSurfaceCapabilitiesKHR			surfaceCapabilities;

	UInt32						surfaceFormatCount;
	VkSurfaceFormatKHR *				surfaceFormats;
	
	UInt32						presentModeCount;
	VkPresentModeKHR *				presentModes;
}
SVulkanSwapchainSupportDetails;

typedef
struct AX_Vulkan_Device
{
	VkDevice					instance;

	SVulkanGPU					gpu;
	SVulkanSwapchainSupportDetails			supportDetails;

	VkCommandPool					graphicsCommandPool;
	VkFormat					depthFormat;
}
SVulkanDevice;

typedef
struct AX_Vulkan_Queue
{
	VkQueue						instance;
	Int32						familyIndex;
}
SVulkanQueue;

typedef
struct AX_Vulkan_Image
{
	VkImage						instance;

	VkImageView					view;

	UInt32						width;
	UInt32						height;

	VkDeviceMemory					memory;
}
SVulkanImage;

typedef
enum AX_Vulkan_Render_Pass_State
{
	AX_VK_RENDER_PASS_STATE_READY,
	AX_VK_RENDER_PASS_STATE_RECORDING,
	AX_VK_RENDER_PASS_STATE_IN_RENDER_PASS,
	AX_VK_RENDER_PASS_STATE_RECORDING_END,
	AX_VK_RENDER_PASS_STATE_SUBMITTED,
	AX_VK_RENDER_PASS_STATE_NOT_ALLOCATED_OR_DISCARDED
}
EVulkanRenderPassState;

typedef
struct AX_Vulkan_Render_Pass
{
	VkRenderPass					instance;

	UVec2						position;
	UVec2						size;
	UVec4						color;

	Float						depth;
	UInt32						stencil;

	EVulkanRenderPassState				state;
}
SVulkanRenderPass;

typedef
struct AX_Vulkan_Framebuffer
{
	VkFramebuffer					instance;

	UInt32						attachmentCount;
	VkImageView *					attachments;

	SVulkanRenderPass *				renderPass;
}
SVulkanFramebuffer;

typedef
struct AX_Vulkan_Swapchain
{
	VkSwapchainKHR					instance;

	VkSurfaceFormatKHR				surfaceFormat;
	VkPresentModeKHR				presentMode;

	VkExtent2D					extent;

	UInt32						imageCount;
	VkImage *					images;

	VkImageView *					imageViews;

	SVulkanImage					depthAttachment;

	SVulkanFramebuffer *				framebuffers;
	
	UInt32						maxFramesInFlight;
}
SVulkanSwapchain;

typedef 
enum AX_Vulkan_Command_Buffer_state 
{
	AX_VK_COMMAND_BUFFER_STATE_READY,
	AX_VK_COMMAND_BUFFER_STATE_RECORDING,
	AX_VK_COMMAND_BUFFER_STATE_IN_RENDER_PASS,
	AX_VK_COMMAND_BUFFER_STATE_RECORDING_END,
	AX_VK_COMMAND_BUFFER_STATE_SUBMITTED,
	AX_VK_COMMAND_BUFFER_STATE_NOT_ALLOCATED_OR_DISCARDED
} 
EVulkanCommandBufferState;

typedef
struct AX_Vulkan_Command_Buffer
{
	VkCommandBuffer					instance;

	EVulkanCommandBufferState			state;
}
SVulkanCommandBuffer;

typedef 
struct AX_Vulkan_Fence 
{
	VkFence						instance;
	
	Bool						signaled;
} 
SVulkanFence;

typedef
struct AX_Vulkan_Sync
{
	VkSemaphore *					imageAcquiredSemaphores;
	VkSemaphore *					renderCompleteSemaphores;
	SVulkanFence *					inFlightFences;
}
SVulkanSync;

typedef
struct AX_Vulkan_Shader_Stage
{
	VkShaderModule					instance;

	VkShaderModuleCreateInfo			properties;
	VkPipelineShaderStageCreateInfo			pipelineStageProperties;
}
SVulkanShaderStage;

typedef
struct AX_Vulkan_Pipeline
{
	VkPipeline					instance;

	VkPipelineLayout				layout;
}
SVulkanPipeline;

typedef
struct AX_Vulkan_Shader
{
	SVulkanShaderStage				stages[2]; // 2 stages: vertex and fragment

	SVulkanPipeline					pipeline;
}
SVulkanShader;

typedef
struct AX_Vulkan_Buffer
{
	VkBuffer					instance;
	
	ByteSize					totalSize;
	VkBufferUsageFlagBits				usageFlags;
	
	Bool						locked;
	
	VkDeviceMemory					memory;
	Int32						memoryIndex;
	UInt32						memoryPropertyFlags;
}
SVulkanBuffer;

typedef
struct AX_Vulkan_Context
{
	VkInstance					instance;

	VoidPtr						window;
	VkSurfaceKHR					surface;

	RawString *					arrEnabledExtensions;
	RawString *					arrEnabledLayers;

	UInt64						framebufferSizeIteration;
	UInt64						lastFramebufferSizeIteration;

	#if defined(AX_CONFIG_DEBUG)
	VkDebugUtilsMessengerEXT			debugMessenger;
	#endif

	SVulkanDevice					device;
	SVulkanSwapchain				swapchain;
	SVulkanRenderPass				mainRenderPass;

	SVulkanQueue					graphicsQueue;
	SVulkanQueue					presentQueue;
	SVulkanQueue					transferQueue;

	SVulkanCommandBuffer *				arrCommandBuffers;

	SVulkanSync					sync;

	SVulkanFence **					inFlightImages;

	Bool						recreatingSwapchain;

	SVulkanShader					objectShader;

	SVulkanBuffer					vertexBuffer;
	UInt64						vertexOffset;
	
	SVulkanBuffer					indexBuffer;
	UInt64						indexOffset;

	SVulkanInstanceCallbacks			instanceCallbacks;
	SVulkanDeviceCallbacks				deviceCallbacks;

	UInt32						currentFrame;
	UInt32						imageIndex;

	VkAllocationCallbacks *				allocator;
}
SVulkanContext;

