#include "Arctix/Renderer/Backend/Vulkan/VkCommandBuffer.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Misc/String/AXString.h"
#include "Arctix/Core/Platform/Window/Window.h"
#include "Arctix/Core/Containers/Array/Array.h"
#include "Arctix/Core/Containers/String/String.h"
#include "Arctix/Core/Logging/Logging.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"


static SVulkanContext *commandBufferContext = NULL;


AX_API
Bool
AX_Renderer_Backend_Vulkan_CommandBuffer_Allocate
(
	const SVulkanContext *context,
	const VkCommandPool commandPool,
	const Bool isPrimary,
	SVulkanCommandBuffer *outCommandBuffer
)
{
	if (!context)
		return false;

	commandBufferContext = AX_CAST(SVulkanContext *, context);

	AX_HAL_Memory_Memzero(outCommandBuffer, sizeof(SVulkanCommandBuffer));

	VkCommandBufferAllocateInfo allocateInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,

		.commandPool = commandPool,
		.level = isPrimary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY,
		.commandBufferCount = 1
	};

	outCommandBuffer->state = AX_VK_COMMAND_BUFFER_STATE_NOT_ALLOCATED_OR_DISCARDED;
	AX_VK_ASSERT(
		vkAllocateCommandBuffers(
			commandBufferContext->device.instance,
			&allocateInfo,
			&(outCommandBuffer->instance)
		)
	);
	outCommandBuffer->state = AX_VK_COMMAND_BUFFER_STATE_READY;

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_CommandBuffer_Deallocate
(
	const VkCommandPool commandPool,
	SVulkanCommandBuffer *outCommandBuffer
)
{
	if (!outCommandBuffer)
		return false;

	vkFreeCommandBuffers(
		commandBufferContext->device.instance,
		commandPool,
		1,
		&(outCommandBuffer->instance)
	);

	AX_HAL_Memory_Memzero(outCommandBuffer, sizeof(SVulkanCommandBuffer));
	outCommandBuffer->state = AX_VK_COMMAND_BUFFER_STATE_NOT_ALLOCATED_OR_DISCARDED;

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_CommandBuffer_Begin
(
	SVulkanCommandBuffer *commandBuffer,
	const Bool single,
	const Bool renderpassContinue,
	const Bool simultaneous
)
{
	if (!commandBuffer)
		return false;

	VkCommandBufferUsageFlags usageFlags = 0;
	if (single) AX_BIT_FLAG_SET(usageFlags, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
	if (renderpassContinue) AX_BIT_FLAG_SET(usageFlags, VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT);
	if (simultaneous) AX_BIT_FLAG_SET(usageFlags, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

	VkCommandBufferBeginInfo beginInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = usageFlags
	};

	AX_VK_ASSERT(vkBeginCommandBuffer(commandBuffer->instance, &beginInfo));
	commandBuffer->state = AX_VK_COMMAND_BUFFER_STATE_RECORDING;

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_CommandBuffer_End
(SVulkanCommandBuffer *commandBuffer)
{
	if (!commandBuffer)
		return false;

	AX_VK_ASSERT(vkEndCommandBuffer(commandBuffer->instance));
	commandBuffer->state = AX_VK_COMMAND_BUFFER_STATE_RECORDING_END;

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_CommandBuffer_Update_Submitted
(SVulkanCommandBuffer *commandBuffer)
{
	if (!commandBuffer)
		return false;

	commandBuffer->state = AX_VK_COMMAND_BUFFER_STATE_SUBMITTED;

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_CommandBuffer_Reset
(SVulkanCommandBuffer *commandBuffer)
{
	if (!commandBuffer)
		return false;

	commandBuffer->state = AX_VK_COMMAND_BUFFER_STATE_READY;

	return true;
}


AX_API
Bool
AX_Renderer_Backend_Vulkan_CommandBuffer_AllocateBeginSingleUse
(
	const SVulkanContext *context,
	const VkCommandPool commandPool,
	SVulkanCommandBuffer *outCommandBuffer
)
{
	if (!context)
		return false;

	if (!AX_Renderer_Backend_Vulkan_CommandBuffer_Allocate(
		context,
		commandPool,
		true,
		outCommandBuffer
	))
		return false;

	if (!AX_Renderer_Backend_Vulkan_CommandBuffer_Begin(
		outCommandBuffer,
		true,
		false,
		false
	))
		return false;

	return true;
}


AX_API
Bool
AX_Renderer_Backend_Vulkan_CommandBuffer_EndSingleUse
(
	const VkCommandPool commandPool,
	const VkQueue queue,
	SVulkanCommandBuffer *outCommandBuffer
)
{
	if (!outCommandBuffer)
		return false;

	if (!AX_Renderer_Backend_Vulkan_CommandBuffer_End(outCommandBuffer))
		return false;

	VkSubmitInfo submitInfo = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO, 
		
		.commandBufferCount = 1,
		.pCommandBuffers = &(outCommandBuffer->instance)
	};
	AX_VK_ASSERT(vkQueueSubmit(queue, 1, &submitInfo, 0));

	AX_VK_ASSERT(vkQueueWaitIdle(queue));
	if (!AX_Renderer_Backend_Vulkan_CommandBuffer_Deallocate(commandPool, outCommandBuffer))
		return false;

	return true;
}




