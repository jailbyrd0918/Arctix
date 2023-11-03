#include "Arctix/Renderer/Backend/Vulkan/VkShader.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Misc/String/AXString.h"
#include "Arctix/Core/Platform/Window/Window.h"
#include "Arctix/Core/Containers/Array/Array.h"
#include "Arctix/Core/Containers/String/String.h"
#include "Arctix/Core/Logging/Logging.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"

#include "Arctix/Renderer/Backend/Vulkan/VkHelper.h"


static SVulkanContext *shaderContext = NULL;

static ReadOnlyString stageNames[] = {
	"Vertex",
	"Fragment"
};


AX_API
Bool
AX_Renderer_Backend_Vulkan_Shader_Startup
(
	const SVulkanContext *context,
	ReadOnlyString shaderFileDirectory,
	ReadOnlyString shaderFilename,
	SVulkanShader *outShader
)
{
	if (!context)
		return false;

	shaderContext = AX_CAST(SVulkanContext *, context);

	// create shader module
	{
		VkShaderStageFlagBits stageFlags[] = {
			VK_SHADER_STAGE_VERTEX_BIT,
			VK_SHADER_STAGE_FRAGMENT_BIT
		};

		ReadOnlyString stageTypes[] = {
			"vert",
			"frag"
		};

		for (UInt32 i = 0; i < AX_STATIC_ARRAY_SIZE(outShader->stages); ++i) {
			SVulkanShaderStage stage = { 0 };
			SString filepath = AX_String_ConstructFormatted(
				"%s/%s.%s.spv",
				shaderFileDirectory,
				shaderFilename,
				stageTypes[i]
			);

			stage.properties.pCode = AX_Renderer_Backend_Vulkan_Helper_GetShaderCode(
				AX_String_GetString(filepath),
				&(stage.properties.codeSize)
			);

			stage.properties = AX_Renderer_Backend_Vulkan_Helper_GetShaderModuleProperties(
				stage.properties.codeSize,
				stage.properties.pCode
			);

			AX_VK_ASSERT(
				vkCreateShaderModule(
					shaderContext->device.instance,
					&(stage.properties),
					shaderContext->allocator,
					&(stage.instance)
				)
			);

			stage.pipelineStageProperties = AX_Renderer_Backend_Vulkan_Helper_GetShaderStageProperties(
				stageFlags[i],
				stage.instance
			);

			AX_HAL_Memory_Memcpy(&(outShader->stages[i]), &stage, sizeof(SVulkanShaderStage));
			AX_String_Destruct(filepath);

			AX_LOG_DEBUG("Engine", "Shader module (for stage '%s') created.", stageNames[i]);
		}
	}

	// create pipeline
	{
		const Float framebufferWidth = AX_CAST(Float, AX_Window_GetWidth(shaderContext->window));
		const Float framebufferHeight = AX_CAST(Float, AX_Window_GetHeight(shaderContext->window));

		VkViewport viewport = {
			.x = 0,
			.y = framebufferHeight,
			
			.width = framebufferWidth,
			.height = framebufferHeight * -1.0f,
			
			.minDepth = 0,
			.maxDepth = 1
		};

		VkRect2D scissor = {
			.offset = {
				.x = 0,
				.y = 0
			},
			.extent = {
				.width = AX_CAST(UInt32, framebufferWidth),
				.height = AX_CAST(UInt32, framebufferHeight)
			}
		};

		UInt64 offset = 0;
		#define	ATTRIBUTE_COUNT	1

		VkFormat formats[] = {
			VK_FORMAT_R32G32B32_SFLOAT
		};
		
		ByteSize sizes[] = {
		    sizeof(UVec3)
		};

		VkVertexInputAttributeDescription attributeDescs[ATTRIBUTE_COUNT];
		for (UInt32 i = 0; i < ATTRIBUTE_COUNT; ++i) {
			attributeDescs[i] = AX_STRUCT(
				VkVertexInputAttributeDescription,

				.binding = 0,
				.location = i,
				.format = formats[i],
				.offset = offset
			);

			offset += sizes[i];
		}

		VkPipelineShaderStageCreateInfo stageCreateInfos[AX_STATIC_ARRAY_SIZE(outShader->stages)] = { 0 };
		for (UInt32 i = 0; i < AX_STATIC_ARRAY_SIZE(outShader->stages); ++i) {
			AX_HAL_Memory_Memcpy(
				&(stageCreateInfos[i]), 
				&(outShader->stages[i].pipelineStageProperties), 
				sizeof(VkPipelineShaderStageCreateInfo)
			);
		}

		if (!AX_Renderer_Backend_Vulkan_Helper_CreateGraphicsPipeline(
			shaderContext,
			&(shaderContext->mainRenderPass),
			ATTRIBUTE_COUNT,
			attributeDescs,
			0,
			NULL,
			AX_STATIC_ARRAY_SIZE(stageCreateInfos),
			stageCreateInfos,
			viewport,
			scissor,
			false,
			&(outShader->pipeline)
		))
			return false;

		AX_LOG_DEBUG("Engine", "Graphics pipeline created.");
	}

	AX_LOG_DEBUG("Engine", "Vulkan shader startup completed.");
	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Shader_Shutdown
(SVulkanShader *outShader)
{
	if (!outShader)
		return false;
	
	// destroy pipeline
	{
		if (!AX_Renderer_Backend_Vulkan_Helper_DestroyGraphicsPipeline(shaderContext, &(outShader->pipeline)))
			return false;

		AX_LOG_DEBUG("Engine", "Graphics pipeline destroyed.");
	}

	// destroy modules
	{
		for (UInt32 i = 0; i < AX_STATIC_ARRAY_SIZE(outShader->stages); ++i) {
			vkDestroyShaderModule(
				shaderContext->device.instance,
				outShader->stages[i].instance,
				shaderContext->allocator
			);

			AX_HAL_Memory_Free(outShader->stages[i].properties.pCode);
			AX_HAL_Memory_Memzero(&(outShader->stages[i]), sizeof(SVulkanShaderStage));

			AX_LOG_DEBUG("Engine", "Shader module (for stage '%s') destroyed.", stageNames[i]);
		}
	}

	AX_HAL_Memory_Memzero(outShader, sizeof(SVulkanShader));

	AX_LOG_DEBUG("Engine", "Vulkan shader shutdown completed.");
	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Shader_Apply
(SVulkanShader *outShader)
{
	if (!outShader)
		return false;

	UInt32 imageIndex = shaderContext->imageIndex;
	
	if (!AX_Renderer_Backend_Vulkan_Helper_BindGraphicsPipeline(
		&(shaderContext->arrCommandBuffers[shaderContext->imageIndex]),
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		&(outShader->pipeline)
	))
		return false;

	return true;
}




