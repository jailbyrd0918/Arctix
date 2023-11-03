#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Renderer/Backend.h"
#include "Arctix/Defines/Renderer/VulkanTypes.h"


AX_API
Bool
AX_Renderer_Backend_Vulkan_Shader_Startup
(
	const SVulkanContext *context,
	ReadOnlyString shaderFileDirectory,
	ReadOnlyString shaderFilename,
	SVulkanShader *outShader
);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Shader_Shutdown
(SVulkanShader *outShader);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Shader_Apply
(SVulkanShader *outShader);
