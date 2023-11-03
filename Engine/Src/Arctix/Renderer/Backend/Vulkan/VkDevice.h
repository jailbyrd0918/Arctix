#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Renderer/Backend.h"
#include "Arctix/Defines/Renderer/VulkanTypes.h"


AX_API
Bool
AX_Renderer_Backend_Vulkan_Device_Startup
(const SVulkanContext *context, SVulkanDevice *outDevice);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Device_Shutdown
(SVulkanDevice *outDevice);


