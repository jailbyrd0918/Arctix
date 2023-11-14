#include "Arctix/Renderer/Backend/Vulkan/VkDevice.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Misc/String/AXString.h"
#include "Arctix/Core/Platform/Window/Window.h"
#include "Arctix/Core/Containers/Array/Array.h"
#include "Arctix/Core/Containers/String/String.h"
#include "Arctix/Core/Logging/Logging.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"

#include "Arctix/Renderer/Backend/Vulkan/VkHelper.h"


static SVulkanContext *deviceContext = NULL;

typedef
enum AX_Vulkan_GPU_Queue_Support_Flag
{
	AX_VK_GPU_QUEUE_SUPPORT_FLAG_GRAPHICS = AX_BIT_FLAG_FIELD(0),
	AX_VK_GPU_QUEUE_SUPPORT_FLAG_COMPUTE = AX_BIT_FLAG_FIELD(1),
	AX_VK_GPU_QUEUE_SUPPORT_FLAG_TRANSFER = AX_BIT_FLAG_FIELD(2),
	AX_VK_GPU_QUEUE_SUPPORT_FLAG_PRESENT = AX_BIT_FLAG_FIELD(3)
}
SVulkanGpuQueueSupportFlag;

typedef
struct AX_Vulkan_GPU_Queue_Family_Info
{
	UInt32				graphicsFamilyIndex;
	UInt32				presentFamilyIndex;
	UInt32				computeFamilyIndex;
	UInt32				transferFamilyIndex;
}
SVulkanGpuQueueFamilyInfo;

typedef
struct AX_Vulkan_GPU_Requirements
{
	SVulkanGpuQueueSupportFlag	queueSupportFlags;

	Bool				isDiscrete;
	Bool				samplerAnisotropyEnabled;
}
SVulkanGpuRequirements;


Bool
_AX_Renderer_Backend_Vulkan_Device_IsPortabilityRequired
(const VkPhysicalDevice gpu)
{
	UInt32 extCount = 0;
	AX_VK_ASSERT(vkEnumerateDeviceExtensionProperties(gpu, NULL, &extCount, NULL));
	AX_RENDERER_ALLOCATE(VkExtensionProperties, extCount, exts);
	AX_VK_ASSERT(vkEnumerateDeviceExtensionProperties(gpu, NULL, &extCount, exts));

	for (UInt32 i = 0; i < extCount; ++i) {
		if (AX_Misc_String_Strcmp(exts[i].extensionName, "VK_KHR_portability_subset", true) == 0) {
			AX_RENDERER_DEALLOCATE(VkExtensionProperties, extCount, exts);
			return true;
		}
	}

	AX_RENDERER_DEALLOCATE(VkExtensionProperties, extCount, exts);
	return false;
}

Bool
_AX_Renderer_Backend_Vulkan_Device_ValidateRequirements
(
	const VkPhysicalDevice gpu,
	const VkSurfaceKHR surface,
	const VkPhysicalDeviceProperties *properties,
	const VkPhysicalDeviceFeatures *features,
	const SVulkanGpuRequirements *requirements,
	SVulkanGpuQueueFamilyInfo *outQueueFamilyInfo,
	SVulkanSwapchainSupportDetails *outSwapchainSupportDetails
)
{
	if (requirements->isDiscrete) {
		if (properties->deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			AX_LOG_WARN("Engine", "GPU not discrete (which is required) - skipping to next GPU...");
			return false;
		}
		else
			AX_LOG_DEBUG("Engine", "Discrete GPU detected.");
	}

	UInt32 queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyCount, NULL);
	AX_RENDERER_ALLOCATE(VkQueueFamilyProperties, queueFamilyCount, queueFamilies);
	vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyCount, queueFamilies);

	for (UInt32 i = 0; i < queueFamilyCount; ++i) {
		VkQueueFamilyProperties queueFamily = queueFamilies[i];

		if (AX_BIT_FLAG_IF_SET(queueFamily.queueFlags, VK_QUEUE_GRAPHICS_BIT))
			outQueueFamilyInfo->graphicsFamilyIndex = i;

		if (AX_BIT_FLAG_IF_SET(queueFamily.queueFlags, VK_QUEUE_COMPUTE_BIT))
			outQueueFamilyInfo->computeFamilyIndex = i;

		if (AX_BIT_FLAG_IF_SET(queueFamily.queueFlags, VK_QUEUE_TRANSFER_BIT))
			outQueueFamilyInfo->transferFamilyIndex = i;

		VkBool32 presentSupported = VK_FALSE;
		AX_VK_ASSERT(
			deviceContext->instanceCallbacks.GetPhysicalDeviceSurfaceSupportKHR(
				gpu, i, surface, &presentSupported
			)
		);
		if (presentSupported)
			outQueueFamilyInfo->presentFamilyIndex = i;

		Bool foundAll =
			(outQueueFamilyInfo->graphicsFamilyIndex != UINT32_MAX) &&
			(outQueueFamilyInfo->computeFamilyIndex != UINT32_MAX) &&
			(outQueueFamilyInfo->transferFamilyIndex != UINT32_MAX) &&
			(outQueueFamilyInfo->presentFamilyIndex != UINT32_MAX);

		if (foundAll) break;
	}

	AX_LOG_DEBUG(
		"Engine",
		"\nGPU Queues:\n"
		"\tGraphics -> %s (Index: %d)\n"
		"\tCompute  -> %s (Index: %d)\n"
		"\tTransfer -> %s (Index: %d)\n"
		"\tPresent  -> %s (Index: %d)\n",
		(outQueueFamilyInfo->graphicsFamilyIndex != UINT32_MAX) ? "supported" : "unsupported",
		outQueueFamilyInfo->graphicsFamilyIndex,
		(outQueueFamilyInfo->computeFamilyIndex != UINT32_MAX) ? "supported" : "unsupported",
		outQueueFamilyInfo->computeFamilyIndex,
		(outQueueFamilyInfo->transferFamilyIndex != UINT32_MAX) ? "supported" : "unsupported",
		outQueueFamilyInfo->transferFamilyIndex,
		(outQueueFamilyInfo->presentFamilyIndex != UINT32_MAX) ? "supported" : "unsupported",
		outQueueFamilyInfo->presentFamilyIndex
	);

	if (!AX_Renderer_Backend_Vulkan_Helper_QuerySwapchainSupportDetails(
		deviceContext, gpu, surface, outSwapchainSupportDetails
	))
		return false;

	if ((outSwapchainSupportDetails->surfaceFormatCount < 1) || (outSwapchainSupportDetails->presentModeCount < 1)) {
		if (outSwapchainSupportDetails->surfaceFormats)
			AX_RENDERER_DEALLOCATE(VkSurfaceFormatKHR, outSwapchainSupportDetails->surfaceFormatCount, outSwapchainSupportDetails->surfaceFormats);

		if (outSwapchainSupportDetails->presentModes)
			AX_RENDERER_DEALLOCATE(VkPresentModeKHR, outSwapchainSupportDetails->presentModeCount, outSwapchainSupportDetails->presentModes);

		AX_LOG_WARN("Engine", "Required swapchain support details not available (which are required) - skipping to next GPU...");
		return false;
	}

	// device extensions
	{
		UInt32 extCount = 0;
		AX_VK_ASSERT(
			vkEnumerateDeviceExtensionProperties(gpu, NULL, &extCount, NULL)
		);

		AX_RENDERER_ALLOCATE(VkExtensionProperties, extCount, exts);
		AX_VK_ASSERT(
			vkEnumerateDeviceExtensionProperties(gpu, NULL, &extCount, exts)
		);

		RawString *requiredDeviceExtensions = AX_Array_Construct(RawString);
		AX_Array_Push(requiredDeviceExtensions, VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		if (_AX_Renderer_Backend_Vulkan_Device_IsPortabilityRequired(gpu))
			AX_Array_Push(requiredDeviceExtensions, "VK_KHR_portability_subset");

		for (UInt32 i = 0; i < AX_Array_GetLength(requiredDeviceExtensions); ++i) {
			Bool found = false;

			for (UInt32 j = 0; j < extCount; ++j) {
				if (AX_Misc_String_Strcmp(exts[j].extensionName, requiredDeviceExtensions[i], true) == 0) {
					found = true;
					break;
				}
			}

			if (!found) {
				AX_LOG_ERROR("Engine", "Device extension not found: '%s' - skipping to next GPU...", requiredDeviceExtensions[i]);
				AX_RENDERER_DEALLOCATE(VkExtensionProperties, extCount, exts);
				return false;
			}
		}

		AX_Array_Destruct(requiredDeviceExtensions);
		AX_RENDERER_DEALLOCATE(VkExtensionProperties, extCount, exts);
	}

	if ((requirements->samplerAnisotropyEnabled) && !(features->samplerAnisotropy)) {
		AX_LOG_WARN("Engine", "Device does not support samplerAnisotropy - skipping to next GPU...");
		return false;
	}

	AX_RENDERER_DEALLOCATE(VkQueueFamilyProperties, queueFamilyCount, queueFamilies);

	// all requirements are met
	return true;
}

Bool
_AX_Renderer_Backend_Vulkan_Device_SelectGPU
(VkPhysicalDevice *outGPU)
{
	UInt32 gpuCount = 0;
	AX_VK_ASSERT(vkEnumeratePhysicalDevices(deviceContext->instance, &gpuCount, NULL));
	AX_ASSERT(gpuCount > 0);

	AX_RENDERER_ALLOCATE(VkPhysicalDevice, gpuCount, gpus);
	AX_VK_ASSERT(vkEnumeratePhysicalDevices(deviceContext->instance, &gpuCount, gpus));

	for (UInt32 i = 0; i < gpuCount; ++i) {
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(gpus[i], &properties);

		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(gpus[i], &features);

		VkPhysicalDeviceMemoryProperties memory;
		vkGetPhysicalDeviceMemoryProperties(gpus[i], &memory);

		Bool localBitSupported = false;
		for (UInt32 i = 0; i < memory.memoryTypeCount; ++i) {
			Bool supported = 
				AX_BIT_FLAG_IF_SET(memory.memoryTypes[i].propertyFlags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) && 
				AX_BIT_FLAG_IF_SET(memory.memoryTypes[i].propertyFlags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		
			if (supported) {
				localBitSupported = true;
				break;
			}
		}

		SVulkanGpuRequirements req = {
			.queueSupportFlags =
				AX_VK_GPU_QUEUE_SUPPORT_FLAG_GRAPHICS |
				AX_VK_GPU_QUEUE_SUPPORT_FLAG_TRANSFER |
				AX_VK_GPU_QUEUE_SUPPORT_FLAG_PRESENT,

			.isDiscrete = true,
			.samplerAnisotropyEnabled = true
		};

		SVulkanGpuQueueFamilyInfo queueFamilyInfo = { UINT32_MAX };

		Bool reqMet = _AX_Renderer_Backend_Vulkan_Device_ValidateRequirements(
			gpus[i],
			deviceContext->surface,
			&properties,
			&features,
			&req,
			&queueFamilyInfo,
			&(deviceContext->device.supportDetails)
		);

		if (reqMet) {
			*outGPU = gpus[i];

			deviceContext->graphicsQueue.familyIndex = queueFamilyInfo.graphicsFamilyIndex;
			deviceContext->presentQueue.familyIndex = queueFamilyInfo.presentFamilyIndex;
			deviceContext->transferQueue.familyIndex = queueFamilyInfo.transferFamilyIndex;

			deviceContext->device.gpu.id = i;
			deviceContext->device.gpu.features = features;
			deviceContext->device.gpu.properties = properties;
			deviceContext->device.gpu.memory = memory;
			deviceContext->device.gpu.localBitSupported = localBitSupported;

			return true;
		}
	}

	return false;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Device_Startup
(const SVulkanContext *context, SVulkanDevice *outDevice)
{
	typedef
		struct AX_Vulkan_GPU_Queue_Family_Info
	{
		UInt32		graphicsFamilyIndex;
		UInt32		presentFamilyIndex;
		UInt32		computeFamilyIndex;
		UInt32		transferFamilyIndex;
	}
	SVulkanGpuQueueFamilyInfo;

	if (!context)
		return false;

	deviceContext = AX_CAST(SVulkanContext *, context);

	// GPU
	{
		// select
		if (!_AX_Renderer_Backend_Vulkan_Device_SelectGPU(&(outDevice->gpu.instance)))
			return false;

		// report
		AX_LOG_DEBUG(
			"Engine",
			"\nSelected GPU:\n"
			"ID: %d\n"
			"Name: %s\n"
			"Type: %s\n"
			"Driver version: %d.%d.%d\n"
			"Vulkan API version: %d.%d.%d\n",
			context->device.gpu.id,
			outDevice->gpu.properties.deviceName,
			AX_Renderer_Backend_Vulkan_Helper_GetDeviceTypeString(outDevice->gpu.properties.deviceType),
			VK_VERSION_MAJOR(outDevice->gpu.properties.driverVersion),
			VK_VERSION_MINOR(outDevice->gpu.properties.driverVersion),
			VK_VERSION_PATCH(outDevice->gpu.properties.driverVersion),
			VK_VERSION_MAJOR(outDevice->gpu.properties.apiVersion),
			VK_VERSION_MINOR(outDevice->gpu.properties.apiVersion),
			VK_VERSION_PATCH(outDevice->gpu.properties.apiVersion)
		);
	}

	// create device instance
	AX_LOG_DEBUG("Engine", "Creating device instace...");
	{
		Bool presentSharesGraphicsQueue = (deviceContext->graphicsQueue.familyIndex == deviceContext->presentQueue.familyIndex);
		Bool transferSharesGraphicsQueue = (deviceContext->graphicsQueue.familyIndex == deviceContext->transferQueue.familyIndex);

		UInt32 familyIndexCount = 1;
		if (!presentSharesGraphicsQueue) familyIndexCount++;
		if (!transferSharesGraphicsQueue) familyIndexCount++;

		AX_RENDERER_ALLOCATE(UInt32, familyIndexCount, familyIndices);
		{
			UInt32 index = 0;
			familyIndices[index++] = deviceContext->graphicsQueue.familyIndex;
			if (!presentSharesGraphicsQueue) familyIndices[index++] = deviceContext->presentQueue.familyIndex;
			if (!transferSharesGraphicsQueue) familyIndices[index++] = deviceContext->transferQueue.familyIndex;
		}

		AX_RENDERER_ALLOCATE(VkDeviceQueueCreateInfo, familyIndexCount, queueCreateInfos);
		for (UInt32 i = 0; i < familyIndexCount; ++i) {
			Float queuePriority = 1.0f;

			queueCreateInfos[i] = AX_STRUCT(
				VkDeviceQueueCreateInfo,

				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,

				.queueFamilyIndex = familyIndices[i],
				.queueCount = 1,
				.pQueuePriorities = &queuePriority
			);
		}

		VkPhysicalDeviceFeatures requestFeatures = {
			.samplerAnisotropy = VK_TRUE
		};

		RawString extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		VkDeviceCreateInfo deviceCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,

			.queueCreateInfoCount = familyIndexCount,
			.pQueueCreateInfos = queueCreateInfos,

			.pEnabledFeatures = &requestFeatures,

			.enabledExtensionCount = AX_STATIC_ARRAY_SIZE(extensions),
			.ppEnabledExtensionNames = extensions
		};

		AX_VK_ASSERT(
			vkCreateDevice(
				outDevice->gpu.instance, &deviceCreateInfo, context->allocator, &(outDevice->instance)
			)
		);

		AX_RENDERER_DEALLOCATE(VkDeviceQueueCreateInfo, familyIndexCount, queueCreateInfos);
		AX_RENDERER_DEALLOCATE(UInt32, familyIndexCount, familyIndices);
	}
	AX_LOG_DEBUG("Engine", "Device instance created.");

	// get queue instances
	{
		if (!AX_Renderer_Backend_Vulkan_Helper_GetQueue(
			deviceContext,
			deviceContext->graphicsQueue.familyIndex,
			&(deviceContext->graphicsQueue.instance)
		))
			return false;

		if (!AX_Renderer_Backend_Vulkan_Helper_GetQueue(
			deviceContext,
			deviceContext->presentQueue.familyIndex,
			&(deviceContext->presentQueue.instance)
		))
			return false;

		if (!AX_Renderer_Backend_Vulkan_Helper_GetQueue(
			deviceContext,
			deviceContext->transferQueue.familyIndex,
			&(deviceContext->transferQueue.instance)
		))
			return false;
	}

	// create graphics command pool
	AX_LOG_DEBUG("Engine", "Creating device graphics command pool...");
	{
		VkCommandPoolCreateInfo poolCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,

			.queueFamilyIndex = deviceContext->graphicsQueue.familyIndex,
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
		};

		AX_VK_ASSERT(
			vkCreateCommandPool(
				deviceContext->device.instance,
				&poolCreateInfo,
				deviceContext->allocator,
				&(deviceContext->device.graphicsCommandPool)
			)
		);
	}
	AX_LOG_DEBUG("Engine", "Device graphics command pool created.");

	AX_LOG_DEBUG("Engine", "Vulkan device startup completed.");
	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_Device_Shutdown
(SVulkanDevice *outDevice)
{
	if (!outDevice)
		return false;

	// destroy graphics command pool
	vkDestroyCommandPool(
		outDevice->instance,
		outDevice->graphicsCommandPool,
		deviceContext->allocator
	);
	AX_LOG_DEBUG("Engine", "Device graphics command pool destroyed.");

	// destroy device instance
	vkDestroyDevice(outDevice->instance, deviceContext->allocator);
	AX_LOG_DEBUG("Engine", "Device instance destroyed.");

	// release swapchain support details
	{
		AX_RENDERER_DEALLOCATE(VkPresentModeKHR, outDevice->supportDetails.presentModeCount, outDevice->supportDetails.presentModes);
		AX_RENDERER_DEALLOCATE(VkSurfaceFormatKHR, outDevice->supportDetails.surfaceFormatCount, outDevice->supportDetails.surfaceFormats);

		AX_HAL_Memory_Memzero(&(outDevice->supportDetails), sizeof(SVulkanSwapchainSupportDetails));
	}
	AX_LOG_DEBUG("Engine", "Device swapchain support details released.");

	AX_HAL_Memory_Memzero(outDevice, sizeof(SVulkanDevice));

	AX_LOG_DEBUG("Engine", "Vulkan device shutdown completed.");
	return true;
}
