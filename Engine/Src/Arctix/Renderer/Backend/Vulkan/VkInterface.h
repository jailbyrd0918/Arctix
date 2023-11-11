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

AX_API
Bool
AX_Renderer_Backend_Vulkan_UpdateGlobalState
(SRenderBackend backend, const UMat4 projection, const UMat4 view, const UVec3 viewPosition, const UVec4 ambientColor, const Int32 mode);

AX_API
Bool
AX_Renderer_Backend_Vulkan_UpdateObject
(SRenderBackend backend, const SGeometryData geometryData);

AX_API
Bool
AX_Renderer_Backend_Vulkan_CreateTexture
(SRenderBackend backend, ReadOnlyString name, const Int32 width, const Int32 height, const UInt8 channelCount, const BytePtr pixels, const Bool hasTransparency, STexture *outTexture);

AX_API
Bool
AX_Renderer_Backend_Vulkan_DestroyTexture
(SRenderBackend backend, STexture *outTexture);






