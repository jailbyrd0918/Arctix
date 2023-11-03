#include "Arctix/Renderer/Backend/Vulkan/VkSwapchain.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Misc/String/AXString.h"
#include "Arctix/Core/Platform/Window/Window.h"
#include "Arctix/Core/Containers/Array/Array.h"
#include "Arctix/Core/Containers/String/String.h"
#include "Arctix/Core/Logging/Logging.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"

#include "Arctix/Renderer/Backend/Vulkan/VkHelper.h"


static SVulkanContext *swapchainContext = NULL;


Bool
_AX_Renderer_Backend_Vulkan_Swapchain_Create
(const UInt32 width, const UInt32 height, SVulkanSwapchain *outSwapchain)
{
	AX_LOG_DEBUG("Engine", "Creating swapchain...");

	VkExtent2D extent = {
		.width = width,
		.height = height
	};

	// init some properties...
	{
		outSwapchain->maxFramesInFlight = 2;

		outSwapchain->surfaceFormat = AX_Renderer_Backend_Vulkan_Helper_SelectSurfaceFormat(
			swapchainContext->device.supportDetails.surfaceFormats,
			swapchainContext->device.supportDetails.surfaceFormatCount
		);

		outSwapchain->presentMode = AX_Renderer_Backend_Vulkan_Helper_SelectPresentMode(
			swapchainContext->device.supportDetails.presentModes,
			swapchainContext->device.supportDetails.presentModeCount
		);

		outSwapchain->extent = AX_Renderer_Backend_Vulkan_Helper_SelectSwapchainExtent(
			swapchainContext->device.supportDetails.surfaceCapabilities,
			swapchainContext->window
		);
	}

	// create instance
	AX_LOG_DEBUG("Engine", "Creating swapchain instance...");
	{
		UInt32 targetImageCount = swapchainContext->device.supportDetails.surfaceCapabilities.minImageCount + 1;

		if (swapchainContext->device.supportDetails.surfaceCapabilities.maxImageCount != 0)
			targetImageCount = AX_MATH_MIN(
				targetImageCount,
				swapchainContext->device.supportDetails.surfaceCapabilities.maxImageCount
			);

		UInt32 queueFamilyIndices[] = {
			swapchainContext->graphicsQueue.familyIndex,
			swapchainContext->presentQueue.familyIndex
		};

		Bool queueSeparated = (swapchainContext->graphicsQueue.familyIndex != swapchainContext->presentQueue.familyIndex);

		VkSurfaceTransformFlagBitsKHR preTransform =
			AX_BIT_FLAG_IF_SET(swapchainContext->device.supportDetails.surfaceCapabilities.supportedTransforms, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) ?
			VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR :
			swapchainContext->device.supportDetails.surfaceCapabilities.currentTransform;

		VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[] = {
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
		};

		for (uint32_t i = 0; i < AX_STATIC_ARRAY_SIZE(compositeAlphaFlags); i++) {
			if (AX_BIT_FLAG_IF_SET(swapchainContext->device.supportDetails.surfaceCapabilities.supportedCompositeAlpha, compositeAlphaFlags[i])) {
				compositeAlpha = compositeAlphaFlags[i];
				break;
			}
		}

		VkSwapchainCreateInfoKHR swapchainCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,

			.surface = swapchainContext->surface,
			.minImageCount = targetImageCount,

			.imageFormat = swapchainContext->swapchain.surfaceFormat.format,
			.imageColorSpace = swapchainContext->swapchain.surfaceFormat.colorSpace,
			.imageExtent = swapchainContext->swapchain.extent,
			.imageArrayLayers = 1,
			.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			.imageSharingMode = queueSeparated ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,

			.queueFamilyIndexCount = queueSeparated ? 2 : 0,
			.pQueueFamilyIndices = queueSeparated ? queueFamilyIndices : NULL,

			.preTransform = preTransform,
			.compositeAlpha = compositeAlpha,

			.presentMode = swapchainContext->swapchain.presentMode,
			.clipped = VK_TRUE,

			.oldSwapchain = VK_NULL_HANDLE
		};

		AX_VK_ASSERT(
			swapchainContext->deviceCallbacks.CreateSwapchainKHR(
				swapchainContext->device.instance, &swapchainCreateInfo, swapchainContext->allocator, &(outSwapchain->instance)
			)
		);
	}
	AX_LOG_DEBUG("Engine", "Creating swapchain instance...");

	swapchainContext->currentFrame = 0;

	// swapchain images
	{
		AX_VK_ASSERT(
			swapchainContext->deviceCallbacks.GetSwapchainImagesKHR(
				swapchainContext->device.instance, outSwapchain->instance, &(outSwapchain->imageCount), NULL
			)
		);

		AX_RENDERER_ALLOCATE(VkImage, outSwapchain->imageCount, images);
		outSwapchain->images = images;

		AX_VK_ASSERT(
			swapchainContext->deviceCallbacks.GetSwapchainImagesKHR(
				swapchainContext->device.instance, outSwapchain->instance, &(outSwapchain->imageCount), outSwapchain->images
			)
		);

		AX_LOG_DEBUG("Engine", "swapchain image count: %d", outSwapchain->imageCount);
	}

	// swapchain image views
	{
		AX_LOG_DEBUG("Engine", "Creating swapchain image views...");

		AX_RENDERER_ALLOCATE(VkImageView, outSwapchain->imageCount, views);
		outSwapchain->imageViews = views;

		for (UInt32 i = 0; i < outSwapchain->imageCount; ++i) {
			VkImageViewCreateInfo imageViewCreateInfo = {
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,

				.image = outSwapchain->images[i],

				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = outSwapchain->surfaceFormat.format,

				.components = {
					.a = VK_COMPONENT_SWIZZLE_IDENTITY,
					.r = VK_COMPONENT_SWIZZLE_IDENTITY,
					.g = VK_COMPONENT_SWIZZLE_IDENTITY,
					.b = VK_COMPONENT_SWIZZLE_IDENTITY
				},

				.subresourceRange = {
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1
				}
			};

			AX_VK_ASSERT(
				vkCreateImageView(
					swapchainContext->device.instance, &imageViewCreateInfo, swapchainContext->allocator, &(outSwapchain->imageViews[i])
				)
			);
		}

		AX_LOG_DEBUG("Engine", "Swapchain image views created.");
	}

	// depth format
	if (!AX_Renderer_Backend_Vulkan_Helper_SearchDepthFormat(&(swapchainContext->device))) {
		swapchainContext->device.depthFormat = VK_FORMAT_UNDEFINED;
		AX_LOG_FATAL("Engine", "Failed to detect support depth format!");
	}

	// depth image
	if (!AX_Renderer_Backend_Vulkan_Helper_CreateImage(
		swapchainContext,
		VK_IMAGE_TYPE_2D,
		outSwapchain->extent.width,
		outSwapchain->extent.height,
		swapchainContext->device.depthFormat,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		true,
		VK_IMAGE_ASPECT_DEPTH_BIT,
		&(outSwapchain->depthAttachment)
	))
		return false;

	AX_LOG_DEBUG("Engine", "Swapchain created.");
	return true;
}

Bool
_AX_Renderer_Backend_Vulkan_Swapchain_Destroy
(SVulkanSwapchain *outSwapchain)
{
	AX_LOG_DEBUG("Engine", "Destroying swapchain instace...");

	vkDeviceWaitIdle(swapchainContext->device.instance);
	
	if (!AX_Renderer_Backend_Vulkan_Helper_DestroyImage(swapchainContext, &(outSwapchain->depthAttachment)))
		return false;

	for (UInt32 i = 0; i < outSwapchain->imageCount; ++i)
		vkDestroyImageView(swapchainContext->device.instance, outSwapchain->imageViews[i], swapchainContext->allocator);

	swapchainContext->deviceCallbacks.DestroySwapchainKHR(
		swapchainContext->device.instance, 
		outSwapchain->instance, 
		swapchainContext->allocator
	);

	AX_RENDERER_DEALLOCATE(VkImageView, outSwapchain->imageCount, outSwapchain->imageViews);
	AX_RENDERER_DEALLOCATE(VkImage, outSwapchain->imageCount, outSwapchain->images);

	AX_LOG_DEBUG("Engine", "Swapchain instace destroyed.");
	return true;
}


AX_API
Bool
AX_Renderer_Backend_Vulkan_Swapchain_Startup
(const SVulkanContext *context, const UInt32 width, const UInt32 height, SVulkanSwapchain *outSwapchain)
{
	if (!context)
		return false;

	swapchainContext = AX_CAST(SVulkanContext *, context);

	if (!_AX_Renderer_Backend_Vulkan_Swapchain_Create(width, height, outSwapchain))
		return false;

	AX_LOG_DEBUG("Engine", "Vulkan swapchain startup completed.");
	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Swapchain_Shutdown
(SVulkanSwapchain *outSwapchain)
{
	if (!outSwapchain)
		return false;

	if (!_AX_Renderer_Backend_Vulkan_Swapchain_Destroy(outSwapchain))
		return false;

	AX_LOG_DEBUG("Engine", "Vulkan swapchain shutdown completed.");
	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Swapchain_Recreate
(const UInt32 width, const UInt32 height, SVulkanSwapchain *outSwapchain)
{
	if (!outSwapchain)
		return false;

	if (!_AX_Renderer_Backend_Vulkan_Swapchain_Destroy(outSwapchain))
		return false;
	
	if (!_AX_Renderer_Backend_Vulkan_Swapchain_Create(width, height, outSwapchain))
		return false;

	AX_LOG_DEBUG("Engine", "Vulkan swapchain recreation completed.");
	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Swapchain_AcquireNextImage
(SVulkanSwapchain *swapchain, const UInt64 timeout, const VkSemaphore semaphore, const VkFence fence, UInt32 *outImageIndex)
{
	if (!swapchain || !outImageIndex)
		return false;

	VkResult result = swapchainContext->deviceCallbacks.AcquireNextImageKHR(
		swapchainContext->device.instance,
		swapchain->instance,
		timeout,
		semaphore,
		fence,
		outImageIndex
	);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		AX_Renderer_Backend_Vulkan_Swapchain_Recreate(
			AX_Window_GetWidth(swapchainContext->window), 
			AX_Window_GetHeight(swapchainContext->window), 
			swapchain
		);

		return false;
	}
	else if ((result != VK_SUCCESS) && (result != VK_SUBOPTIMAL_KHR)) {
		AX_LOG_FATAL("Engine", "Failed to acquire swapchain image!");
		return false;
	}

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Swapchain_PresentImage
(SVulkanSwapchain *swapchain, const VkQueue presentQueue, const VkSemaphore semaphore, const UInt32 presentImageIndex)
{
	if (!swapchain)
		return false;

	VkPresentInfoKHR presentInfo = {
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &semaphore,
		
		.swapchainCount = 1,
		.pSwapchains = &(swapchain->instance),
		
		.pImageIndices = &presentImageIndex
	};

	VkResult result = swapchainContext->deviceCallbacks.QueuePresentKHR(presentQueue , &presentInfo);
	
	if ((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR)) {
		AX_Renderer_Backend_Vulkan_Swapchain_Recreate(
			AX_Window_GetWidth(swapchainContext->window),
			AX_Window_GetHeight(swapchainContext->window),
			swapchain
		);
	}
	else if (result != VK_SUCCESS) {
		AX_LOG_FATAL("Engine", "Failed to present swapchain image!");
	}

	// update context current frame
	swapchainContext->currentFrame = (swapchainContext->currentFrame + 1) % swapchain->maxFramesInFlight;

	return true;
}

