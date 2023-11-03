#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Renderer/Backend.h"
#include "Arctix/Defines/Renderer/VulkanTypes.h"


AX_API
Bool
AX_Renderer_Backend_Vulkan_OnStartup
(SRenderBackend backend, SRenderBackendConfig config, VoidPtr window);

AX_API
Bool
AX_Renderer_Backend_Vulkan_OnShutdown
(SRenderBackend backend);

AX_API
Bool
AX_Renderer_Backend_Vulkan_OnResized
(SRenderBackend backend, Int32 width, Int32 height);

AX_API
Bool
AX_Renderer_Backend_Vulkan_OnFrameBegin
(SRenderBackend backend, const Float deltaTime);

AX_API
Bool
AX_Renderer_Backend_Vulkan_OnFrameEnd
(SRenderBackend backend, const Float deltaTime);




