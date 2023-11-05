#include "Arctix/Renderer/Backend/Vulkan/VkShader.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Misc/String/AXString.h"
#include "Arctix/Core/Platform/Window/Window.h"
#include "Arctix/Core/Containers/Array/Array.h"
#include "Arctix/Core/Containers/String/String.h"
#include "Arctix/Core/Logging/Logging.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"

#include "Arctix/Renderer/Backend/Vulkan/VkBuffer.h"
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
				AX_CAST(VoidPtr, stage.properties.pCode)
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

	// create global descriptor properties
	{
		VkDescriptorSetLayoutBinding globalDescLayoutBinding = {
			.binding = 0,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.pImmutableSamplers = NULL,
			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT
		};

		VkDescriptorSetLayoutCreateInfo globalDescLayoutCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,

			.bindingCount = 1,
			.pBindings = &globalDescLayoutBinding
		};

		AX_VK_ASSERT(
			vkCreateDescriptorSetLayout(
				shaderContext->device.instance,
				&globalDescLayoutCreateInfo,
				shaderContext->allocator,
				&(outShader->globalDescriptorSetLayout)
			)
		);

		VkDescriptorPoolSize globalDescPoolSize = {
			.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,

			.descriptorCount = shaderContext->swapchain.imageCount
		};

		VkDescriptorPoolCreateInfo globalDescPoolCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,

			.poolSizeCount = 1,
			.pPoolSizes = &globalDescPoolSize,

			.maxSets = shaderContext->swapchain.imageCount,
			.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT
		};

		AX_VK_ASSERT(
			vkCreateDescriptorPool(
				shaderContext->device.instance,
				&globalDescPoolCreateInfo,
				shaderContext->allocator,
				&(outShader->globalDescriptorPool)
			)
		);
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
				.offset = AX_CAST(UInt32, offset)
			);

			offset += sizes[i];
		}

		VkDescriptorSetLayout descriptorSetLayouts[] = {
			outShader->globalDescriptorSetLayout
		};

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
			AX_STATIC_ARRAY_SIZE(descriptorSetLayouts),
			descriptorSetLayouts,
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

	// create global uniform buffer
	{
		const VkBufferUsageFlagBits usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		const UInt32 memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		if (!AX_Renderer_Backend_Vulkan_Buffer_Startup(
			shaderContext,
			sizeof(SGlobalUniform) * 3,
			usageFlags,
			memoryPropertyFlags,
			true,
			&(outShader->globalUniformBuffer)
		))
			return false;
	}

	// allocate global descriptor sets
	{
		VkDescriptorSetLayout globalDescSetLayouts[] = {
			outShader->globalDescriptorSetLayout,
			outShader->globalDescriptorSetLayout,
			outShader->globalDescriptorSetLayout
		};

		VkDescriptorSetAllocateInfo allocateInfo = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,

			.descriptorPool = outShader->globalDescriptorPool,

			.descriptorSetCount = AX_STATIC_ARRAY_SIZE(outShader->globalDescriptorSets),
			.pSetLayouts = globalDescSetLayouts
		};

		AX_VK_ASSERT(
			vkAllocateDescriptorSets(
				shaderContext->device.instance, &allocateInfo, outShader->globalDescriptorSets
			)
		);
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

	// deallocate global descriptor sets
	AX_VK_ASSERT(
		vkFreeDescriptorSets(
			shaderContext->device.instance,
			outShader->globalDescriptorPool,
			AX_STATIC_ARRAY_SIZE(outShader->globalDescriptorSets),
			outShader->globalDescriptorSets
		)
	);

	// destroy global uniform buffer
	if (!AX_Renderer_Backend_Vulkan_Buffer_Shutdown(&(outShader->globalUniformBuffer)))
		return false;

	// destroy pipeline
	{
		if (!AX_Renderer_Backend_Vulkan_Helper_DestroyGraphicsPipeline(shaderContext, &(outShader->pipeline)))
			return false;

		AX_LOG_DEBUG("Engine", "Graphics pipeline destroyed.");
	}

	// destroy global descriptor properties
	{
		vkDestroyDescriptorPool(shaderContext->device.instance, outShader->globalDescriptorPool, shaderContext->allocator);
		vkDestroyDescriptorSetLayout(shaderContext->device.instance, outShader->globalDescriptorSetLayout, shaderContext->allocator);
	}

	// destroy modules
	{
		for (UInt32 i = 0; i < AX_STATIC_ARRAY_SIZE(outShader->stages); ++i) {
			vkDestroyShaderModule(
				shaderContext->device.instance,
				outShader->stages[i].instance,
				shaderContext->allocator
			);

			AX_HAL_Memory_Free(AX_CAST(VoidPtr, outShader->stages[i].properties.pCode));
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
AX_Renderer_Backend_Vulkan_Shader_UpdateGlobalState
(SVulkanShader *outShader)
{
	if (!outShader)
		return false;

	const UInt32 imageIndex = shaderContext->imageIndex;
	const VkCommandBuffer commandBuffer = shaderContext->arrCommandBuffers[imageIndex].instance;
	const VkDescriptorSet globalDescriptor = outShader->globalDescriptorSets[imageIndex];

	const ByteSize dataRange = sizeof(SGlobalUniform);
	const UInt64 offset = sizeof(SGlobalUniform) * imageIndex;

	if (!AX_Renderer_Backend_Vulkan_Buffer_LoadData(
		offset,
		dataRange,
		0,
		&(outShader->globalUniform),
		&(outShader->globalUniformBuffer)
	))
		return false;

	VkDescriptorBufferInfo bufferInfo = {
		.buffer = outShader->globalUniformBuffer.instance,
		.offset = offset,
		.range = dataRange
	};

	VkWriteDescriptorSet writeSet = {
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,

		.dstSet = outShader->globalDescriptorSets[imageIndex],
		.dstBinding = 0,
		.dstArrayElement = 0,

		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1,

		.pBufferInfo = &bufferInfo
	};

	vkUpdateDescriptorSets(shaderContext->device.instance, 1, &writeSet, 0, NULL);

	vkCmdBindDescriptorSets(
		commandBuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		outShader->pipeline.layout,
		0,
		1,
		&globalDescriptor,
		0,
		NULL
	);

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Shader_UpdateObject
(const SVulkanShader *shader, const UMat4 model)
{
	if (!shader)
		return false;

	const UInt32 imageIndex = shaderContext->imageIndex;
	const VkCommandBuffer commandBuffer = shaderContext->arrCommandBuffers[imageIndex].instance;

	vkCmdPushConstants(commandBuffer, shader->pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(UMat4), &model);

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





