#include "Arctix/Renderer/Backend/Vulkan/VkBuffer.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Misc/String/AXString.h"
#include "Arctix/Core/Platform/Window/Window.h"
#include "Arctix/Core/Containers/Array/Array.h"
#include "Arctix/Core/Containers/String/String.h"
#include "Arctix/Core/Logging/Logging.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"

#include "Arctix/Renderer/Backend/Vulkan/VkCommandBuffer.h"
#include "Arctix/Renderer/Backend/Vulkan/VkHelper.h"


static SVulkanContext *bufferContext = NULL;


AX_API
Bool
AX_Renderer_Backend_Vulkan_Buffer_Startup
(
	const SVulkanContext *context,
	const ByteSize size,
	const VkBufferUsageFlagBits usageFlags,
	const UInt32 memoryPropertyFlags,
	const Bool bindOnCreate,
	SVulkanBuffer *outBuffer
)
{
	if (!context)
		return false;
	
	bufferContext = AX_CAST(SVulkanContext *, context);

	SVulkanBuffer buffer = {
		.totalSize = size,
		.usageFlags = usageFlags,
		.memoryPropertyFlags = memoryPropertyFlags
	};

	VkBufferCreateInfo bufferCreateInfo = { 
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.size = buffer.totalSize,
		.usage = buffer.usageFlags
	};

	AX_VK_ASSERT(
		vkCreateBuffer(
			bufferContext->device.instance, 
			&bufferCreateInfo, 
			bufferContext->allocator,
			&(buffer.instance)
		)
	);

	VkMemoryRequirements memReq;
	vkGetBufferMemoryRequirements(
		bufferContext->device.instance, buffer.instance, &memReq
	);
	
	Int32 memIndex = AX_Renderer_Backend_Vulkan_Helper_SearchMemoryIndex(
		bufferContext,
		memoryPropertyFlags,
		memReq.memoryTypeBits
	);
	AX_ASSERT_MESSAGE(memIndex != -1, "Failed to locate memory index to create Vulkan buffer!");

	buffer.memoryIndex = memIndex;

	VkMemoryAllocateInfo allocateInfo = {
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memReq.size,
		.memoryTypeIndex = buffer.memoryIndex
	};
	
	AX_VK_ASSERT(
		vkAllocateMemory(
			bufferContext->device.instance,
			&allocateInfo,
			bufferContext->allocator,
			&(buffer.memory)
		)
	);

	if (bindOnCreate)
		if (!AX_Renderer_Backend_Vulkan_Buffer_Bind(0, &buffer))
			return false;

	AX_HAL_Memory_Memcpy(outBuffer, &buffer, sizeof(SVulkanBuffer));

	AX_LOG_DEBUG("Engine", "Vulkan buffer startup completed.");
	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Buffer_Shutdown
(SVulkanBuffer *outBuffer)
{
	if (!outBuffer)
		return false;

	vkFreeMemory(bufferContext->device.instance, outBuffer->memory, bufferContext->allocator);
	vkDestroyBuffer(bufferContext->device.instance, outBuffer->instance, bufferContext->allocator);

	AX_HAL_Memory_Memzero(outBuffer, sizeof(SVulkanBuffer));

	AX_LOG_DEBUG("Engine", "Vulkan buffer shutdown completed.");
	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Buffer_Resize
(
	const ByteSize newSize,
	const VkQueue queue,
	const VkCommandPool commandPool,
	SVulkanBuffer *outBuffer
)
{
	if (!outBuffer)
		return false;

	SVulkanBuffer newBuffer = {
		.totalSize = newSize,
		.usageFlags = outBuffer->usageFlags,
		.memoryPropertyFlags = outBuffer->memoryPropertyFlags
	};

	VkBufferCreateInfo newBufferCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,

		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.size = newBuffer.totalSize,
		.usage = newBuffer.usageFlags
	};

	AX_VK_ASSERT(
		vkCreateBuffer(
			bufferContext->device.instance,
			&newBufferCreateInfo,
			bufferContext->allocator,
			&(newBuffer.instance)
		)
	);

	VkMemoryRequirements memReq;
	vkGetBufferMemoryRequirements(
		bufferContext->device.instance, newBuffer.instance, &memReq
	);

	Int32 memIndex = AX_Renderer_Backend_Vulkan_Helper_SearchMemoryIndex(
		bufferContext,
		newBuffer.memoryPropertyFlags,
		memReq.memoryTypeBits
	);
	AX_ASSERT_MESSAGE(memIndex != -1, "Failed to locate memory index to create Vulkan buffer!");

	newBuffer.memoryIndex = memIndex;

	VkMemoryAllocateInfo allocateInfo = {
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memReq.size,
		.memoryTypeIndex = newBuffer.memoryIndex
	};

	AX_VK_ASSERT(
		vkAllocateMemory(
			bufferContext->device.instance,
			&allocateInfo,
			bufferContext->allocator,
			&(newBuffer.memory)
		)
	);

	AX_VK_ASSERT(
		vkBindBufferMemory(
			bufferContext->device.instance, newBuffer.instance, newBuffer.memory, 0
		)
	);

	if (!AX_Renderer_Backend_Vulkan_Buffer_Copy(
		commandPool,
		queue,
		VK_NULL_HANDLE,
		newBuffer.totalSize,
		0,
		&(outBuffer->instance),
		0,
		&(newBuffer.instance)
	))
		return false;

	AX_VK_ASSERT(vkDeviceWaitIdle(bufferContext->device.instance));

	vkFreeMemory(bufferContext->device.instance, outBuffer->memory, bufferContext->allocator);
	vkDestroyBuffer(bufferContext->device.instance, outBuffer->instance, bufferContext->allocator);

	ByteSize oldSize = outBuffer->totalSize;
	AX_HAL_Memory_Memcpy(outBuffer, &newBuffer, sizeof(SVulkanBuffer));

	AX_LOG_DEBUG("Engine", "Vulkan buffer resized (old: %llu bytes, new: %llu bytes).", oldSize, outBuffer->totalSize);
	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Buffer_Bind
(
	const UInt64 offset,
	SVulkanBuffer *outBuffer
)
{
	if (!outBuffer)
		return false;

	AX_VK_ASSERT(
		vkBindBufferMemory(
			bufferContext->device.instance, outBuffer->instance, outBuffer->memory, offset
		)
	);

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Buffer_LockMemory
(
	const UInt64 offset,
	const ByteSize size,
	const UInt32 flags,
	const SVulkanBuffer *buffer,
	VoidPtr outData
)
{
	if (!buffer)
		return false;

	AX_VK_ASSERT(
		vkMapMemory(
			bufferContext->device.instance, buffer->memory, offset, size, flags, &outData
		)
	);

	AX_LOG_DEBUG("Engine", "Vulkan buffer memory locked.");
	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Buffer_UnlockMemory
(const SVulkanBuffer *buffer)
{
	if (!buffer)
		return false;

	vkUnmapMemory(bufferContext->device.instance, buffer->memory);

	AX_LOG_DEBUG("Engine", "Vulkan buffer memory unlocked.");
	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Buffer_LoadData
(
	const UInt64 offset,
	const ByteSize size,
	const UInt32 flags,
	const VoidPtr data,
	const SVulkanBuffer *buffer
)
{
	if (!buffer)
		return false;

	VoidPtr dataPtr;
	AX_VK_ASSERT(
		vkMapMemory(
			bufferContext->device.instance, buffer->memory, offset, size, flags, &dataPtr
		)
	);

	AX_HAL_Memory_Memcpy(dataPtr, data, size);
	vkUnmapMemory(bufferContext->device.instance, buffer->memory);
	
	AX_LOG_DEBUG("Engine", "Vulkan buffer data loaded (to address: %p).", dataPtr);
	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Buffer_Copy
(
	const VkCommandPool commandPool,
	const VkQueue queue,
	const VkFence fence,
	const ByteSize size,
	const UInt64 sourceOffset,
	const VkBuffer *source,
	const UInt64 destOffset,
	const VkBuffer *dest
)
{
	if (!source || !dest)
		return false;

	SVulkanCommandBuffer cmdBuf;
	if (!AX_Renderer_Backend_Vulkan_CommandBuffer_AllocateBeginSingleUse(
		bufferContext, 
		commandPool, 
		&cmdBuf
	))
		return false;

	VkBufferCopy copyRegion = {
		.srcOffset = sourceOffset,
		.dstOffset = destOffset,
		
		.size = size
	};

	vkCmdCopyBuffer(cmdBuf.instance, *source, *dest, 1, &copyRegion);

	if (!AX_Renderer_Backend_Vulkan_CommandBuffer_EndSingleUse(
		commandPool,
		queue,
		&cmdBuf
	))
		return false;

	AX_LOG_DEBUG("Engine", "Vulkan buffer copied (src: %p, dest: %p).", source, dest);
	return true;
}









