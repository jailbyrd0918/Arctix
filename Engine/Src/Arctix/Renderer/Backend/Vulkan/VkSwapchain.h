#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Renderer/Backend.h"
#include "Arctix/Defines/Renderer/VulkanTypes.h"


AX_API
Bool
AX_Renderer_Backend_Vulkan_Swapchain_Startup
(const SVulkanContext *context, const UInt32 width, const UInt32 height, SVulkanSwapchain *outSwapchain);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Swapchain_Shutdown
(SVulkanSwapchain *outSwapchain);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Swapchain_Recreate
(const UInt32 width, const UInt32 height, SVulkanSwapchain *outSwapchain);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Swapchain_AcquireNextImage
(SVulkanSwapchain *swapchain, const UInt64 timeout, const VkSemaphore semaphore, const VkFence fence, UInt32 *outImageIndex);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Swapchain_PresentImage
(SVulkanSwapchain *swapchain, const VkQueue presentQueue, const VkSemaphore semaphore, const UInt32 presentImageIndex);



