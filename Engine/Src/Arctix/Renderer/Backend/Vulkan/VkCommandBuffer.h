#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Renderer/Backend.h"
#include "Arctix/Defines/Renderer/VulkanTypes.h"


AX_API
Bool
AX_Renderer_Backend_Vulkan_CommandBuffer_Allocate
(
	const SVulkanContext *context,
	const VkCommandPool commandPool,
	const Bool isPrimary,
	SVulkanCommandBuffer *outCommandBuffer
);

AX_API
Bool
AX_Renderer_Backend_Vulkan_CommandBuffer_Deallocate
(
	const VkCommandPool commandPool,
	SVulkanCommandBuffer *outCommandBuffer
);

AX_API
Bool
AX_Renderer_Backend_Vulkan_CommandBuffer_Begin
(
	SVulkanCommandBuffer *commandBuffer,
	const Bool single,
	const Bool renderpassContinue,
	const Bool simultaneous
);

AX_API
Bool
AX_Renderer_Backend_Vulkan_CommandBuffer_End
(SVulkanCommandBuffer *commandBuffer);

AX_API
Bool
AX_Renderer_Backend_Vulkan_CommandBuffer_Update_Submitted
(SVulkanCommandBuffer *commandBuffer);

AX_API
Bool
AX_Renderer_Backend_Vulkan_CommandBuffer_Reset
(SVulkanCommandBuffer *commandBuffer);


AX_API
Bool
AX_Renderer_Backend_Vulkan_CommandBuffer_AllocateBeginSingleUse
(
	const SVulkanContext *context,
	const VkCommandPool commandPool,
	SVulkanCommandBuffer *outCommandBuffer
);


AX_API
Bool
AX_Renderer_Backend_Vulkan_CommandBuffer_EndSingleUse
(
	const VkCommandPool commandPool,
	const VkQueue queue,
	SVulkanCommandBuffer *outCommandBuffer
);


