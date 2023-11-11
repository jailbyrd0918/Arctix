#include "Arctix/Renderer/Backend/Vulkan/VkShader.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Misc/String/AXString.h"
#include "Arctix/Core/Platform/Window/Window.h"
#include "Arctix/Core/Containers/Array/Array.h"
#include "Arctix/Core/Containers/String/String.h"
#include "Arctix/Core/Math/Helper/MathHelper.h"
#include "Arctix/Core/Math/Vector/Vec4.h"
#include "Arctix/Core/Logging/Logging.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"

#include "Arctix/Renderer/Backend/Vulkan/VkHelper.h"
#include "Arctix/Renderer/Backend/Vulkan/VkBuffer.h"


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

	// create object descriptor properties
	{
		VkDescriptorSetLayoutBinding bindings[] = {
			[0] = {
				.binding = 0,
				.descriptorCount = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
			},	
			
			[1] = {
				.binding = 1,
				.descriptorCount = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
			}
		};

		VkDescriptorSetLayoutCreateInfo layoutCreateInfo = { 
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = AX_STATIC_ARRAY_SIZE(bindings),
			.pBindings = bindings,
		};

		AX_VK_ASSERT(
			vkCreateDescriptorSetLayout(
				context->device.instance, &layoutCreateInfo, context->allocator, &(outShader->objectDescriptorSetLayout)
			)
		);

		const UInt32 localSamplerCount = 1;
		VkDescriptorPoolSize objectPoolSizes[] = {
			[0] = {
				.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = 1024 // TODO: make 'max object count' configurable
			},
			[1] = {
				.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.descriptorCount = localSamplerCount * 1024 // TODO: make 'max object count' configurable
			}
		};

		VkDescriptorPoolCreateInfo objectDescPoolCreateInfo = { 
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO, 
			
			.poolSizeCount = AX_STATIC_ARRAY_SIZE(objectPoolSizes),
			.pPoolSizes = objectPoolSizes,
			
			.maxSets = 1024, // TODO: make 'max object count' configurable
			.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT
		};

		AX_VK_ASSERT(
			vkCreateDescriptorPool(
				context->device.instance, &objectDescPoolCreateInfo, context->allocator, &(outShader->objectDescriptorPool)
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
		#define	ATTRIBUTE_COUNT	2

		VkFormat formats[] = {
			VK_FORMAT_R32G32B32_SFLOAT,
			VK_FORMAT_R32G32_SFLOAT
		};

		ByteSize sizes[] = {
		    sizeof(UVec3),
		    sizeof(UVec2)
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
			outShader->globalDescriptorSetLayout,
			outShader->objectDescriptorSetLayout
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
	
	// create object uniform buffer
	{
		const VkBufferUsageFlagBits usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		const UInt32 memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		if (!AX_Renderer_Backend_Vulkan_Buffer_Startup(
			shaderContext,
			sizeof(SObjectUniform),
			usageFlags,
			memoryPropertyFlags,
			true,
			&(outShader->objectUniformBuffer)
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

	// destroy object uniform buffer
	if (!AX_Renderer_Backend_Vulkan_Buffer_Shutdown(&(outShader->objectUniformBuffer)))
		return false;

	// destroy global uniform buffer
	if (!AX_Renderer_Backend_Vulkan_Buffer_Shutdown(&(outShader->globalUniformBuffer)))
		return false;

	// destroy pipeline
	{
		if (!AX_Renderer_Backend_Vulkan_Helper_DestroyGraphicsPipeline(shaderContext, &(outShader->pipeline)))
			return false;

		AX_LOG_DEBUG("Engine", "Graphics pipeline destroyed.");
	}

	// destroy object descriptor properties
	{
		vkDestroyDescriptorPool(shaderContext->device.instance, outShader->objectDescriptorPool, shaderContext->allocator);
		vkDestroyDescriptorSetLayout(shaderContext->device.instance, outShader->objectDescriptorSetLayout, shaderContext->allocator);
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
(SVulkanShader *outShader, const Float deltaTime)
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
(SVulkanShader *shader, const SGeometryData geometryData, const Float deltaTime)
{
	if (!shader)
		return false;

	const UInt32 imageIndex = shaderContext->imageIndex;
	const VkCommandBuffer commandBuffer = shaderContext->arrCommandBuffers[imageIndex].instance;

	vkCmdPushConstants(commandBuffer, shader->pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(UMat4), &(geometryData.model));

	SVulkanShaderState *objectState = &(shader->objectStates[geometryData.objectID]);
	VkDescriptorSet objectDescriptorSet = objectState->descriptorSets[imageIndex];
	VkWriteDescriptorSet writeSet[2] = { 0 };
	UInt32 descriptorCount = 0;
	UInt32 descriptorIndex = 0;
	UInt32 size = sizeof(SObjectUniform);
	UInt64 offset = sizeof(SObjectUniform) * geometryData.objectID;
	SObjectUniform objectUniform;

	// static Float accumulator = 0.0f;
	// accumulator += deltaTime;
	// Float sinVal = (AX_Math_Sin(accumulator) + 1.0f) / 2.0f; // scale from (-1, 1) to (0, 1)
	
	objectUniform.diffuseColor = AX_Math_Vec4_Construct(1.0f, 1.0f, 1.0f, 1.0f);

	if (!AX_Renderer_Backend_Vulkan_Buffer_LoadData(
		offset, 
		size,
		0, 
		&objectUniform,
		&(shader->objectUniformBuffer)
	))
		return false;

	if (objectState->descriptorStates[descriptorIndex].generations[imageIndex] == AX_INVALID_ID) {
		VkDescriptorBufferInfo bufferInfo = {
			.buffer = shader->objectUniformBuffer.instance,
			.offset = offset,
			.range = size
		};

		VkWriteDescriptorSet descriptorSet = { 
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			
			.dstSet = objectDescriptorSet,
			.dstBinding = descriptorIndex,
			
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = 1,
			
			.pBufferInfo = &bufferInfo
		};

		writeSet[descriptorCount++] = descriptorSet;
		objectState->descriptorStates[descriptorIndex].generations[imageIndex] = 1;
	}

	++descriptorIndex;

	#define	SAMPLER_COUNT	1
	VkDescriptorImageInfo imageInfos[SAMPLER_COUNT];
	
	for (UInt32 samplerIndex = 0; samplerIndex < SAMPLER_COUNT; ++samplerIndex) {
		STexture *texture = geometryData.textures[samplerIndex];
		UInt32 *descriptorGen = &(objectState->descriptorStates[descriptorIndex].generations[imageIndex]);

		if ((texture != NULL) && ((*descriptorGen != texture->generation) || (*descriptorGen == AX_INVALID_ID))) {
			SVulkanTextureData *textureData = AX_CAST(SVulkanTextureData *, texture->data);

			imageInfos[samplerIndex].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfos[samplerIndex].imageView = textureData->image.view;
			imageInfos[samplerIndex].sampler = textureData->sampler;

			VkWriteDescriptorSet descriptor = { 
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				
				.dstSet = objectDescriptorSet,
				.dstBinding = descriptorIndex,
				
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.descriptorCount = 1,
				
				.pImageInfo = &(imageInfos[samplerIndex])
			};

			writeSet[descriptorCount++] = descriptor;

			if (texture->generation != AX_INVALID_ID)
				*descriptorGen = texture->generation;

			++descriptorIndex;
		}
	}

	if (descriptorCount > 0)
		vkUpdateDescriptorSets(shaderContext->device.instance, descriptorCount, writeSet, 0, NULL);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->pipeline.layout, 1, 1, &objectDescriptorSet, 0, NULL);

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

AX_API
Bool
AX_Renderer_Backend_Vulkan_Shader_AcquireResources
(SVulkanShader *shader, UInt32 *outObjectID)
{
	if (!AX_Renderer_Backend_Vulkan_Helper_AcquireShaderResources(shaderContext, shader, outObjectID))
		return false;

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Shader_ReleaseResources
(SVulkanShader *shader, const UInt32 objectID)
{
	if (!AX_Renderer_Backend_Vulkan_Helper_ReleaseShaderResources(shaderContext, shader, objectID))
		return false;

	return true;
}



