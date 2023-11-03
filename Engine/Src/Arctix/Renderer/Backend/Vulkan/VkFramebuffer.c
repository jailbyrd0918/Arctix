#include "Arctix/Renderer/Backend/Vulkan/VkFramebuffer.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Misc/String/AXString.h"
#include "Arctix/Core/Platform/Window/Window.h"
#include "Arctix/Core/Containers/Array/Array.h"
#include "Arctix/Core/Containers/String/String.h"
#include "Arctix/Core/Logging/Logging.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"


static SVulkanContext *framebufferContext = NULL;


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
)
{
	if (!context || !renderPass)
		return false;

	framebufferContext = AX_CAST(SVulkanContext *, context);

	outFramebuffer->attachmentCount = attachmentCount;
	
	AX_RENDERER_ALLOCATE(VkImageView, outFramebuffer->attachmentCount, attachmentList);
	outFramebuffer->attachments = attachmentList;

	for (UInt32 i = 0; i < outFramebuffer->attachmentCount; ++i)
		outFramebuffer->attachments[i] = attachments[i];

	outFramebuffer->renderPass = AX_CAST(SVulkanRenderPass *, renderPass);

	VkFramebufferCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		
		.attachmentCount = outFramebuffer->attachmentCount,
		.pAttachments = outFramebuffer->attachments,
		
		.width = width,
		.height = height,
		
		.renderPass = renderPass->instance,
		.layers = 1
	};

	AX_VK_ASSERT(
		vkCreateFramebuffer(
			framebufferContext->device.instance,
			&createInfo,
			framebufferContext->allocator,
			&(outFramebuffer->instance)
		)
	);

	AX_LOG_DEBUG("Engine", "Vulkan framebuffer (for image index %d) startup completed.", imageIndex);
	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Framebuffer_Shutdown
(const UInt32 imageIndex, SVulkanFramebuffer *outFramebuffer)
{
	if (!outFramebuffer)
		return false;

	vkDestroyFramebuffer(
		framebufferContext->device.instance, outFramebuffer->instance, framebufferContext->allocator
	);
	
	if (outFramebuffer->attachments)
		AX_RENDERER_DEALLOCATE(VkImageView, outFramebuffer->attachmentCount, outFramebuffer->attachments);

	AX_HAL_Memory_Memzero(outFramebuffer, sizeof(SVulkanFramebuffer));

	AX_LOG_DEBUG("Engine", "Vulkan framebuffer (for image index %d) shutdown completed.", imageIndex);
	return true;
}

