#include "Arctix/Renderer/Backend/Vulkan/VkRenderPass.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Misc/String/AXString.h"
#include "Arctix/Core/Platform/Window/Window.h"
#include "Arctix/Core/Containers/Array/Array.h"
#include "Arctix/Core/Containers/String/String.h"
#include "Arctix/Core/Logging/Logging.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"


static SVulkanContext *renderPassContext = NULL;


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
)
{
	if (!context)
		return false;

	outRenderPass->position = position;
	outRenderPass->size = size;
	outRenderPass->color = color;
	outRenderPass->depth = depth;
	outRenderPass->stencil = stencil;

	if (!renderPassContext)
		renderPassContext = AX_CAST(SVulkanContext *, context);

	AX_LOG_DEBUG("Engine", "Creating render pass instance...");
	{
		VkAttachmentDescription colorAttachment = {
			.format = context->swapchain.surfaceFormat.format,
			.samples = VK_SAMPLE_COUNT_1_BIT,

			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,

			.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,

			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
		};

		VkAttachmentDescription depthStencilAttachment = {
			.format = context->device.depthFormat,
			.samples = VK_SAMPLE_COUNT_1_BIT,

			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,

			.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,

			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
		};

		VkAttachmentDescription attachments[] = { colorAttachment, depthStencilAttachment };

		VkAttachmentReference colorAttachmentRef = {
			.attachment = 0,
			.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		};

		VkAttachmentReference depthStencilAttachmentRef = {
			.attachment = 1,
			.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
		};

		VkSubpassDescription subpass = {
			.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,

			.colorAttachmentCount = 1,
			.pColorAttachments = &colorAttachmentRef,

			.pDepthStencilAttachment = &depthStencilAttachmentRef,

			.inputAttachmentCount = 0,
			.pInputAttachments = NULL,

			.pResolveAttachments = NULL,

			.preserveAttachmentCount = 0,
			.pPreserveAttachments = NULL
		};

		VkSubpassDependency dependency = {
			.srcSubpass = VK_SUBPASS_EXTERNAL,
			.dstSubpass = 0,

			.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			.srcAccessMask = 0,

			.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,

			.dependencyFlags = 0
		};

		VkRenderPassCreateInfo createInfo = {
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,

			.attachmentCount = AX_STATIC_ARRAY_SIZE(attachments),
			.pAttachments = attachments,

			.subpassCount = 1,
			.pSubpasses = &subpass,

			.dependencyCount = 1,
			.pDependencies = &dependency
		};

		AX_VK_ASSERT(
			vkCreateRenderPass(
				context->device.instance, &createInfo, context->allocator, &(outRenderPass->instance)
			)
		);
	}
	AX_LOG_DEBUG("Engine", "Render pass instance created.");

	AX_LOG_DEBUG("Engine", "Vulkan render pass startup completed.");
	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_RenderPass_Shutdown
(SVulkanRenderPass *outRenderPass)
{
	if (!outRenderPass)
		return false;

	vkDestroyRenderPass(
		renderPassContext->device.instance, outRenderPass->instance, renderPassContext->allocator
	);
	AX_LOG_DEBUG("Engine", "Render pass instance destroyed.");

	AX_HAL_Memory_Memzero(outRenderPass, sizeof(SVulkanRenderPass));

	AX_LOG_DEBUG("Engine", "Vulkan render pass shutdown completed.");
	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_RenderPass_Begin
(SVulkanCommandBuffer *commandBuffer, const SVulkanRenderPass *renderPass, const VkFramebuffer framebuffer)
{
	if (!commandBuffer || !renderPass)
		return false;

	VkClearValue clearValues[] = {
		[0] = {
			.color.float32 = {
				renderPass->color.r,
				renderPass->color.g,
				renderPass->color.b,
				renderPass->color.a
			}
		},

		[1] = {
			.depthStencil = {
				.depth = renderPass->depth,
				.stencil = renderPass->stencil
			}
		}
	};

	VkRenderPassBeginInfo beginInfo = { 
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO, 
		
		.renderPass = renderPass->instance,
		.framebuffer = framebuffer,
		
		.renderArea = {
			.offset = {
				.x = AX_CAST(UInt32, renderPass->position.x),
				.y = AX_CAST(UInt32, renderPass->position.y)
			},
			.extent = {
				.width = AX_CAST(UInt32, renderPass->size.x),
				.height = AX_CAST(UInt32, renderPass->size.y)
			}
		},

		.clearValueCount = AX_STATIC_ARRAY_SIZE(clearValues),
		.pClearValues = clearValues
	};

	vkCmdBeginRenderPass(commandBuffer->instance, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
	commandBuffer->state = AX_VK_COMMAND_BUFFER_STATE_IN_RENDER_PASS;

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_RenderPass_End
(SVulkanCommandBuffer *commandBuffer, const SVulkanRenderPass *renderPass)
{
	if (!commandBuffer || !renderPass)
		return false;

	vkCmdEndRenderPass(commandBuffer->instance);
	commandBuffer->state = AX_VK_COMMAND_BUFFER_STATE_RECORDING;
	
	return true;
}


