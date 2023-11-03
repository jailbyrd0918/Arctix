#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Renderer/Backend.h"
#include "Arctix/Defines/Renderer/VulkanTypes.h"


AX_API
Bool
AX_Renderer_Backend_Vulkan_Fence_Startup
(const SVulkanContext *context, const Bool signaled, SVulkanFence *outFence);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Fence_Shutdown
(SVulkanFence *outFence);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Fence_Wait
(SVulkanFence *fence, const UInt64 timeout);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Fence_Reset
(SVulkanFence *fence);

