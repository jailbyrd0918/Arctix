#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Renderer/Backend.h"
#include "Arctix/Defines/Renderer/VulkanTypes.h"
#include "Arctix/Defines/Resources/Texture.h"


AX_API
Bool
AX_Renderer_Backend_Vulkan_Shader_Startup
(
	const SVulkanContext *context,
	ReadOnlyString shaderFileDirectory,
	ReadOnlyString shaderFilename,
	STexture *defaultTexture, 
	SVulkanShader *outShader
);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Shader_Shutdown
(SVulkanShader *outShader);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Shader_UpdateGlobalState
(SVulkanShader *outShader, const Float deltaTime);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Shader_UpdateObject
(SVulkanShader *shader, const SGeometryData geometryData, const Float deltaTime);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Shader_Apply
(SVulkanShader *outShader);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Shader_AcquireResources
(SVulkanShader *shader, UInt32 *outObjectID);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Shader_ReleaseResources
(SVulkanShader *shader, const UInt32 objectID);


