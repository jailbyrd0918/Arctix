#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Renderer/Backend.h"
#include "Arctix/Defines/Renderer/VulkanTypes.h"


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
);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Buffer_Shutdown
(SVulkanBuffer *outBuffer);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Buffer_Resize
(
	const ByteSize newSize,
	const VkQueue queue,
	const VkCommandPool commandPool,
	SVulkanBuffer *outBuffer
);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Buffer_Bind
(
	const UInt64 offset,
	SVulkanBuffer *outBuffer
);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Buffer_LockMemory
(
	const UInt64 offset,
	const ByteSize size,
	const UInt32 flags,
	const SVulkanBuffer *buffer,
	VoidPtr outData
);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Buffer_UnlockMemory
(const SVulkanBuffer *buffer);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Buffer_LoadData
(
	const UInt64 offset,
	const ByteSize size,
	const UInt32 flags,
	const VoidPtr data,
	const SVulkanBuffer *buffer
);

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
);



