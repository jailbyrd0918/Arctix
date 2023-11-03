#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Renderer/Backend.h"
#include "Arctix/Defines/Renderer/VulkanTypes.h"


AX_API
Bool
AX_Renderer_Backend_Vulkan_Framebuffer_Startup
(
	const SVulkanContext *context,
	const SVulkanRenderPass *renderPass,
	UInt32 width,
	UInt32 height,
	UInt32 attachmentCount,
	const VkImageView *attachments,
	const UInt32 imageIndex,
	SVulkanFramebuffer *outFramebuffer
);

AX_API
Bool
AX_Renderer_Backend_Vulkan_Framebuffer_Shutdown
(const UInt32 imageIndex, SVulkanFramebuffer *outFramebuffer);
