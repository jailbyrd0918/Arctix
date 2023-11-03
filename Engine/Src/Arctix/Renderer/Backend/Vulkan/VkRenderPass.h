#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Renderer/Backend.h"
#include "Arctix/Defines/Renderer/VulkanTypes.h"


AX_API
Bool
AX_Renderer_Backend_Vulkan_RenderPass_Startup
(
	const SVulkanContext *context,
	const UVec2 position,
	const UVec2 size,
	const UVec4 color,
	const Float depth,
	const UInt32 stencil,
	SVulkanRenderPass *outRenderPass
);

AX_API
Bool
AX_Renderer_Backend_Vulkan_RenderPass_Shutdown
(SVulkanRenderPass *outRenderPass);

AX_API
Bool
AX_Renderer_Backend_Vulkan_RenderPass_Begin
(SVulkanCommandBuffer *commandBuffer, const SVulkanRenderPass *renderPass, const VkFramebuffer framebuffer);

AX_API
Bool
AX_Renderer_Backend_Vulkan_RenderPass_End
(SVulkanCommandBuffer *commandBuffer, const SVulkanRenderPass *renderPass);




