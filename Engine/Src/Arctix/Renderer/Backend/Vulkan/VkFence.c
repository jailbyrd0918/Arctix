#include "Arctix/Renderer/Backend/Vulkan/VkFence.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Misc/String/AXString.h"
#include "Arctix/Core/Platform/Window/Window.h"
#include "Arctix/Core/Containers/Array/Array.h"
#include "Arctix/Core/Containers/String/String.h"
#include "Arctix/Core/Logging/Logging.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"


static SVulkanContext *fenceContext = NULL;

AX_API
Bool
AX_Renderer_Backend_Vulkan_Fence_Startup
(const SVulkanContext *context, const Bool signaled, SVulkanFence *outFence)
{
	if (!context)
		return false;

	fenceContext = AX_CAST(SVulkanContext *, context);

	outFence->signaled = signaled;

	VkFenceCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.flags = (outFence->signaled) ? VK_FENCE_CREATE_SIGNALED_BIT : 0
	};

	AX_VK_ASSERT(
		vkCreateFence(
			fenceContext->device.instance,
			&createInfo,
			fenceContext->allocator,
			&(outFence->instance)
		)
	);

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Fence_Shutdown
(SVulkanFence *outFence)
{
	if (!outFence)
		return false;

	vkDestroyFence(fenceContext->device.instance, outFence->instance, fenceContext->allocator);
	AX_HAL_Memory_Memzero(outFence, sizeof(SVulkanFence));

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Fence_Wait
(SVulkanFence *fence, const UInt64 timeout)
{
	if (!fence)
		return false;

	if (fence->signaled)
		return true;

	VkResult result = vkWaitForFences(fenceContext->device.instance, 1, &(fence->instance), true, timeout);
	switch (result) {
		case VK_SUCCESS:
			fence->signaled = true;
			return true;

		case VK_TIMEOUT:
			AX_LOG_WARN("Engine", __FUNCTION__" - Timed out");
			break;

		case VK_ERROR_DEVICE_LOST:
			AX_LOG_ERROR("Engine", __FUNCTION__" - VK_ERROR_DEVICE_LOST.");
			break;

		case VK_ERROR_OUT_OF_HOST_MEMORY:
			AX_LOG_ERROR("Engine", __FUNCTION__" - VK_ERROR_OUT_OF_HOST_MEMORY.");
			break;

		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			AX_LOG_ERROR("Engine", __FUNCTION__" - VK_ERROR_OUT_OF_DEVICE_MEMORY.");
			break;

		default:
			AX_LOG_ERROR("Engine", __FUNCTION__" - An unknown error has occurred.");
			break;
	}

	return false;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Fence_Reset
(SVulkanFence *fence)
{
	if (!fence)
		return false;

	if (!fence->signaled)
		return false;

	AX_VK_ASSERT(vkResetFences(fenceContext->device.instance, 1, &(fence->instance)));
	fence->signaled = false;

	return true;
}

