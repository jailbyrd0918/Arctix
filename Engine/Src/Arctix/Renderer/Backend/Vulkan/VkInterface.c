#include "Arctix/Renderer/Backend/Vulkan/VkInterface.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Misc/String/AXString.h"
#include "Arctix/Core/Platform/Window/Window.h"
#include "Arctix/Core/Containers/Array/Array.h"
#include "Arctix/Core/Containers/String/String.h"
#include "Arctix/Core/Logging/Logging.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"
#include "Arctix/Core/Math/Vector/Vec2.h"
#include "Arctix/Core/Math/Vector/Vec4.h"

#include "Arctix/Renderer/Backend/Vulkan/VkHelper.h"
#include "Arctix/Renderer/Backend/Vulkan/VkDevice.h"
#include "Arctix/Renderer/Backend/Vulkan/VkSwapchain.h"
#include "Arctix/Renderer/Backend/Vulkan/VkRenderPass.h"
#include "Arctix/Renderer/Backend/Vulkan/VkFramebuffer.h"
#include "Arctix/Renderer/Backend/Vulkan/VkCommandBuffer.h"
#include "Arctix/Renderer/Backend/Vulkan/VkFence.h"
#include "Arctix/Renderer/Backend/Vulkan/VkShader.h"
#include "Arctix/Renderer/Backend/Vulkan/VkBuffer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#if	defined(AX_CONFIG_DEBUG)
#	define	AX_VK_VALIDATION_LAYERS_ENABLED		1

#else
#	define	AX_VK_VALIDATION_LAYERS_ENABLED		0

#endif


#define AX_VK_LOAD_INSTANCE_CALLBACK(instance, entrypoint)									\
{																\
	context.instanceCallbacks.##entrypoint = (PFN_vk##entrypoint)vkGetInstanceProcAddr(instance, "vk"#entrypoint);		\
	AX_ASSERT_MESSAGE(context.instanceCallbacks.##entrypoint, "Failed to load callback vk"#entrypoint"!");			\
	AX_LOG_DEBUG("Engine", "Callback to vk"#entrypoint" loaded.");								\
}

#define AX_VK_LOAD_DEVICE_CALLBACK(instance, device, entrypoint)								\
{																\
	PFN_vkGetDeviceProcAddr gdpa = (PFN_vkGetDeviceProcAddr)vkGetInstanceProcAddr(instance, "vkGetDeviceProcAddr");		\
	context.deviceCallbacks.##entrypoint = (PFN_vk##entrypoint)gdpa(device, "vk"#entrypoint);				\
	AX_ASSERT_MESSAGE(context.deviceCallbacks.##entrypoint, "Failed to load callback vk"#entrypoint"!");			\
	AX_LOG_DEBUG("Engine", "Callback to vk"#entrypoint" loaded.");								\
}


static SVulkanContext context;
static SDL_Window *contextWindow = NULL;


VKAPI_ATTR VkBool32
_AX_Renderer_Backend_Vulkan_DebugCallback
(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT *callbackData,
	VoidPtr userData
)
{
	switch (messageSeverity) {
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			AX_LOG_ERROR("Engine", callbackData->pMessage);
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			AX_LOG_WARN("Engine", callbackData->pMessage);
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			AX_LOG_INFO("Engine", callbackData->pMessage);
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			AX_LOG_TRACE("Engine", callbackData->pMessage);
			break;

		default:
			break;
	}

	return VK_FALSE;
}

Bool
_AX_Renderer_Backend_Vulkan_GetInstanceExtensions
(void)
{
	if (!contextWindow)
		contextWindow = AX_Window_GetSDLWindow(context.window);

	UInt32 extCount;
	SDL_Vulkan_GetInstanceExtensions(contextWindow, &extCount, NULL);

	context.arrEnabledExtensions = AX_Array_Construct(RawString);
	if (!context.arrEnabledExtensions) {
		AX_Array_Destruct(context.arrEnabledExtensions);
		return false;
	}

	// populate enabled extensions
	{
		AX_RENDERER_ALLOCATE(RawString, extCount, exts);

		SDL_Vulkan_GetInstanceExtensions(contextWindow, &extCount, exts);
		for (UInt32 i = 0; i < extCount; ++i)
			AX_Array_Push(context.arrEnabledExtensions, exts[i]);

		AX_RENDERER_DEALLOCATE(RawString, extCount, exts);
	}

	// include debug utils extension if validation enabled
	#if AX_VK_VALIDATION_LAYERS_ENABLED
	{
		AX_Array_Push(context.arrEnabledExtensions, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	#endif

	// report extensions
	{
		SString report = AX_String_ConstructFormatted(
			"\nDetected %d instance extensions:\n",
			AX_Array_GetLength(context.arrEnabledExtensions)
		);

		for (UInt32 i = 0; i < AX_Array_GetLength(context.arrEnabledExtensions); ++i)
			AX_String_AppendFormatted(report, "%d. %s\n", (i + 1), context.arrEnabledExtensions[i]);

		AX_LOG_DEBUG("Engine", AX_String_GetString(report));
		AX_String_Destruct(report);
	}

	return true;
}

Bool
_AX_Renderer_Backend_Vulkan_SearchValidationLayers
(void)
{
	#if AX_VK_VALIDATION_LAYERS_ENABLED
	{
		AX_LOG_DEBUG("Engine", "Validation layers enabled.");

		UInt32 layerCount = 0;
		RawString requiredLayers[] = { "VK_LAYER_KHRONOS_validation" };

		AX_VK_ASSERT(vkEnumerateInstanceLayerProperties(&layerCount, NULL));

		context.arrEnabledLayers = AX_Array_Construct(RawString);
		if (!context.arrEnabledLayers) {
			AX_Array_Destruct(context.arrEnabledLayers);
			return false;
		}

		{
			AX_RENDERER_ALLOCATE(VkLayerProperties, layerCount, layers);
			AX_VK_ASSERT(vkEnumerateInstanceLayerProperties(&layerCount, layers));

			SString report = AX_String_Construct("\nRequired Layers:\n");

			for (UInt32 i = 0; i < AX_STATIC_ARRAY_SIZE(requiredLayers); ++i) {
				AX_String_AppendFormatted(report, "%d. %s: ", (i + 1), requiredLayers[i]);
				Bool available = false;

				for (UInt32 j = 0; j < layerCount; ++j) {
					available = (AX_Misc_String_Strcmp(requiredLayers[i], layers[j].layerName, true) == 0);

					if (available) {
						AX_String_Append(report, "available\n");
						AX_Array_Push(context.arrEnabledLayers, requiredLayers[i]);
						break;
					}
				}

				if (!available)
					AX_String_Append(report, "unavailable\n");
			}

			AX_LOG_DEBUG("Engine", AX_String_GetString(report));
			AX_String_Destruct(report);

			AX_RENDERER_DEALLOCATE(VkLayerProperties, layerCount, layers);
		}
	}
	#endif

	return true;
}

void
_AX_Renderer_Backend_Vulkan_LoadInstanceCallbacks
(void)
{
	AX_VK_LOAD_INSTANCE_CALLBACK(context.instance, CreateDebugUtilsMessengerEXT);
	AX_VK_LOAD_INSTANCE_CALLBACK(context.instance, DestroyDebugUtilsMessengerEXT);

	AX_VK_LOAD_INSTANCE_CALLBACK(context.instance, GetPhysicalDeviceSurfaceSupportKHR);
	AX_VK_LOAD_INSTANCE_CALLBACK(context.instance, GetPhysicalDeviceSurfaceCapabilitiesKHR);
	AX_VK_LOAD_INSTANCE_CALLBACK(context.instance, GetPhysicalDeviceSurfaceFormatsKHR);
	AX_VK_LOAD_INSTANCE_CALLBACK(context.instance, GetPhysicalDeviceSurfacePresentModesKHR);
}

void
_AX_Renderer_Backend_Vulkan_LoadDeviceCallbacks
(void)
{
	AX_VK_LOAD_DEVICE_CALLBACK(context.instance, context.device.instance, CreateSwapchainKHR);
	AX_VK_LOAD_DEVICE_CALLBACK(context.instance, context.device.instance, DestroySwapchainKHR);
	AX_VK_LOAD_DEVICE_CALLBACK(context.instance, context.device.instance, GetSwapchainImagesKHR);
	AX_VK_LOAD_DEVICE_CALLBACK(context.instance, context.device.instance, AcquireNextImageKHR);
	AX_VK_LOAD_DEVICE_CALLBACK(context.instance, context.device.instance, QueuePresentKHR);
}

Bool
_AX_Renderer_Backend_Vulkan_CreateContextInstance
(SRenderBackendConfig config)
{
	if (!_AX_Renderer_Backend_Vulkan_GetInstanceExtensions())
		return false;

	if (!_AX_Renderer_Backend_Vulkan_SearchValidationLayers())
		return false;

	AX_LOG_DEBUG("Engine", "Creating Vulkan context instance...");
	{
		VkApplicationInfo appInfo = {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,

			.apiVersion = VK_API_VERSION_1_2,

			.pApplicationName = config.appName,
			.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),

			.pEngineName = "Arctix Engine",
			.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0)
		};

		VkInstanceCreateInfo createInfo = {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,

			.pApplicationInfo = &appInfo,

			.enabledExtensionCount = AX_Array_GetLength(context.arrEnabledExtensions),
			.ppEnabledExtensionNames = context.arrEnabledExtensions,

			.enabledLayerCount = AX_Array_GetLength(context.arrEnabledLayers),
			.ppEnabledLayerNames = context.arrEnabledLayers
		};

		AX_VK_ASSERT(vkCreateInstance(&createInfo, context.allocator, &(context.instance)));
	}
	AX_LOG_DEBUG("Engine", "Vulkan context instance created.");

	return true;
}

void
_AX_Renderer_Backend_Vulkan_SetupDebugMessenger
(void)
{
	#if AX_VK_VALIDATION_LAYERS_ENABLED
	{
		AX_LOG_DEBUG("Engine", "Setting up Vulkan debug messenger...");

		/*VkDebugUtilsMessageSeverityFlagsEXT msgSeverity =
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;*/

		VkDebugUtilsMessageSeverityFlagsEXT msgSeverity =
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;

		VkDebugUtilsMessageTypeFlagsEXT msgType =
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;

		VkDebugUtilsMessengerCreateInfoEXT createInfo = {
			.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,

			.messageSeverity = msgSeverity,
			.messageType = msgType,

			.pfnUserCallback = _AX_Renderer_Backend_Vulkan_DebugCallback,
			.pUserData = &context
		};

		AX_VK_ASSERT(
			context.instanceCallbacks.CreateDebugUtilsMessengerEXT(
				context.instance,
				&createInfo,
				context.allocator,
				&(context.debugMessenger)
			)
		);

		AX_LOG_DEBUG("Engine", "Vulkan debug messenger setup completed.");
	}
	#endif
}

Bool
_AX_Renderer_Backend_Vulkan_CreateWindowSurface
(void)
{
	AX_LOG_DEBUG("Engine", "Creating Vulkan window surface...");

	if (!AX_Renderer_Backend_Vulkan_Helper_CreateWindowSurface(&context, &(context.surface)))
		return false;

	AX_LOG_DEBUG("Engine", "Vulkan window surface created.");
	return true;
}

Bool
_AX_Renderer_Backend_Vulkan_RegenerateFramebuffers
(void)
{
	for (UInt32 i = 0; i < context.swapchain.imageCount; ++i) {
		VkImageView attachments[] = {
		    context.swapchain.imageViews[i],
		    context.swapchain.depthAttachment.view
		};

		if (!AX_Renderer_Backend_Vulkan_Framebuffer_Startup(
			&context,
			&(context.mainRenderPass),
			AX_Window_GetWidth(context.window),
			AX_Window_GetHeight(context.window),
			AX_STATIC_ARRAY_SIZE(attachments),
			attachments,
			i,
			&(context.swapchain.framebuffers[i])
		))
			return false;
	}

	return true;
}

Bool
_AX_Renderer_Backend_Vulkan_CreateCommandBuffer
(void)
{
	context.arrCommandBuffers = AX_Array_ConstructReserved(SVulkanCommandBuffer, context.swapchain.imageCount);
	for (UInt32 i = 0; i < context.swapchain.imageCount; ++i) {
		if (context.arrCommandBuffers[i].instance) {
			if (!AX_Renderer_Backend_Vulkan_CommandBuffer_Deallocate(
				context.device.graphicsCommandPool,
				&(context.arrCommandBuffers[i])
			))
				return false;
		}

		AX_HAL_Memory_Memzero(&(context.arrCommandBuffers[i]), sizeof(SVulkanCommandBuffer));

		if (!AX_Renderer_Backend_Vulkan_CommandBuffer_Allocate(
			&context,
			context.device.graphicsCommandPool,
			true,
			&(context.arrCommandBuffers[i])
		))
			return false;
	}

	AX_LOG_DEBUG("Engine", "Graphics command buffers allocated.");
	return true;
}

Bool
_AX_Renderer_Backend_Vulkan_CreateSync
(void)
{
	AX_LOG_DEBUG("Engine", "Creating Vulkan sync object...");

	AX_RENDERER_ALLOCATE(VkSemaphore, context.swapchain.maxFramesInFlight, imageAcquiredSemaphores);
	context.sync.imageAcquiredSemaphores = imageAcquiredSemaphores;

	AX_RENDERER_ALLOCATE(VkSemaphore, context.swapchain.maxFramesInFlight, renderCompleteSemaphores);
	context.sync.renderCompleteSemaphores = renderCompleteSemaphores;

	AX_RENDERER_ALLOCATE(SVulkanFence, context.swapchain.maxFramesInFlight, inFlightFences);
	context.sync.inFlightFences = inFlightFences;

	for (UInt32 i = 0; i < context.swapchain.maxFramesInFlight; ++i) {
		VkSemaphoreCreateInfo semaphoreCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
		};

		VkFenceCreateInfo fenceCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,

			.flags = VK_FENCE_CREATE_SIGNALED_BIT
		};

		AX_VK_ASSERT(
			vkCreateSemaphore(
				context.device.instance, &semaphoreCreateInfo, context.allocator, &(context.sync.imageAcquiredSemaphores[i])
			)
		);

		AX_VK_ASSERT(
			vkCreateSemaphore(
				context.device.instance, &semaphoreCreateInfo, context.allocator, &(context.sync.renderCompleteSemaphores[i])
			)
		);

		if (!AX_Renderer_Backend_Vulkan_Fence_Startup(&context, true, &(context.sync.inFlightFences[i])))
			return false;
	}

	AX_LOG_DEBUG("Engine", "Vulkan sync object created.");
	return true;
}

Bool
_AX_Renderer_Backend_Vulkan_DestroySync
(void)
{
	AX_LOG_DEBUG("Engine", "Destroying Vulkan sync object...");

	for (UInt32 i = 0; i < context.swapchain.maxFramesInFlight; ++i) {
		vkDestroySemaphore(context.device.instance, context.sync.imageAcquiredSemaphores[i], context.allocator);
		vkDestroySemaphore(context.device.instance, context.sync.renderCompleteSemaphores[i], context.allocator);

		if (!AX_Renderer_Backend_Vulkan_Fence_Shutdown(&(context.sync.inFlightFences[i])))
			return false;
	}

	AX_RENDERER_DEALLOCATE(VkSemaphore, context.swapchain.maxFramesInFlight, context.sync.imageAcquiredSemaphores);
	AX_RENDERER_DEALLOCATE(VkSemaphore, context.swapchain.maxFramesInFlight, context.sync.renderCompleteSemaphores);
	AX_RENDERER_DEALLOCATE(SVulkanFence, context.swapchain.maxFramesInFlight, context.sync.inFlightFences);

	AX_LOG_DEBUG("Engine", "Vulkan sync object destroyed.");
	return true;
}

Bool
_AX_Renderer_Backend_Vulkan_RecreateSwapchain
(void)
{
	if (context.recreatingSwapchain) {
		AX_LOG_DEBUG("Engine", "Swapchain recreation in progress...");
		return false;
	}

	if ((AX_Window_GetWidth(context.window) == 0) || (AX_Window_GetHeight(context.window) == 0))
		return false;

	context.recreatingSwapchain = true;

	// wait for all other operations to complete
	AX_VK_ASSERT(vkDeviceWaitIdle(context.device.instance));

	if (!AX_Renderer_Backend_Vulkan_Helper_QuerySwapchainSupportDetails(
		&context,
		context.device.gpu.instance,
		context.surface,
		&(context.device.supportDetails)
	))
		return false;

	if (!AX_Renderer_Backend_Vulkan_Swapchain_Recreate(
		AX_Window_GetWidth(context.window),
		AX_Window_GetHeight(context.window),
		&(context.swapchain)
	))
		return false;

	context.mainRenderPass.size = AX_Math_Vec2_Construct(
		AX_CAST(Float, AX_Window_GetWidth(context.window)),
		AX_CAST(Float, AX_Window_GetHeight(context.window))
	);

	// update framebuffer size iteration
	context.lastFramebufferSizeIteration = context.framebufferSizeIteration;

	for (UInt32 i = 0; i < context.swapchain.imageCount; ++i)
		if (!AX_Renderer_Backend_Vulkan_CommandBuffer_Deallocate(context.device.graphicsCommandPool, &(context.arrCommandBuffers[i])))
			return false;

	for (UInt32 i = 0; i < context.swapchain.imageCount; ++i)
		if (!AX_Renderer_Backend_Vulkan_Framebuffer_Shutdown(i, &(context.swapchain.framebuffers[i])))
			return false;

	if (!_AX_Renderer_Backend_Vulkan_RegenerateFramebuffers())
		return false;

	if (!_AX_Renderer_Backend_Vulkan_CreateCommandBuffer())
		return false;

	context.recreatingSwapchain = false;
	return true;
}

Bool
_AX_Renderer_Backend_Vulkan_CreateBuffers
(const ByteSize vertexBufferSize, const ByteSize indexBufferSize)
{
	VkMemoryPropertyFlagBits memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	// vertex buffer
	{
		VkBufferUsageFlagBits vertexBufferUsageFlags =
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
			VK_BUFFER_USAGE_TRANSFER_DST_BIT |
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		if (!AX_Renderer_Backend_Vulkan_Buffer_Startup(
			&context,
			vertexBufferSize,
			vertexBufferUsageFlags,
			memoryPropertyFlags,
			true,
			&(context.vertexBuffer)
		))
			return false;

		context.vertexOffset = 0;

		AX_LOG_DEBUG("Engine", "Vertex buffer created.");
	}

	// index buffer
	{
		VkBufferUsageFlagBits indexBufferUsageFlags =
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
			VK_BUFFER_USAGE_TRANSFER_DST_BIT |
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		if (!AX_Renderer_Backend_Vulkan_Buffer_Startup(
			&context,
			indexBufferSize,
			indexBufferUsageFlags,
			memoryPropertyFlags,
			true,
			&(context.indexBuffer)
		))
			return false;

		context.indexOffset = 0;
		
		AX_LOG_DEBUG("Engine", "Index buffer created.");
	}

	return true;
}

Bool
_AX_Renderer_Backend_Vulkan_DestroyBuffers
(void)
{
	// vertex buffer
	{
		if (!AX_Renderer_Backend_Vulkan_Buffer_Shutdown(&(context.vertexBuffer)))
			return false;

		AX_LOG_DEBUG("Engine", "Vertex buffer destroyed.");
	}

	// index buffer
	{
		if (!AX_Renderer_Backend_Vulkan_Buffer_Shutdown(&(context.indexBuffer)))
			return false;

		AX_LOG_DEBUG("Engine", "Index buffer destroyed.");
	}

	return true;
}

Bool
_AX_Renderer_Backend_Vulkan_UploadData
(
	const VkCommandPool commandPool,
	const VkFence fence,
	const VkQueue queue,
	const UInt64 offset,
	const UInt64 size,
	const VoidPtr data,
	SVulkanBuffer *outBuffer
)
{
	if (!data)
		return false;

	VkBufferUsageFlags usageFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	SVulkanBuffer stagingBuffer = { 0 };

	// create a staging buffer as upload target - considered source of transfer
	// upload the data into it
	// copy the data from staging buffer to local buffer
	// destroy the staging buffer
	{
		if (!AX_Renderer_Backend_Vulkan_Buffer_Startup(
			&context,
			size,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			usageFlags,
			true,
			&stagingBuffer
		))
			return false;

		if (!AX_Renderer_Backend_Vulkan_Buffer_LoadData(
			0, size, 0, data, &stagingBuffer
		))
			return false;

		if (!AX_Renderer_Backend_Vulkan_Buffer_Copy(
			commandPool,
			queue,
			fence,
			size,
			0,
			&(stagingBuffer.instance),
			0,
			&(outBuffer->instance)
		))
			return false;

		if (!AX_Renderer_Backend_Vulkan_Buffer_Shutdown(&stagingBuffer))
			return false;
	}

	return true;
}


AX_API
Bool
AX_Renderer_Backend_Vulkan_OnStartup
(SRenderBackend backend, SRenderBackendConfig config, VoidPtr window)
{
	if (!backend || !window)
		return false;

	AX_HAL_Memory_Memzero(&context, sizeof(SVulkanContext));

	// init window
	{
		context.window = window;
		contextWindow = AX_Window_GetSDLWindow(context.window);
	}

	// setup
	{
		// context instance
		if (!_AX_Renderer_Backend_Vulkan_CreateContextInstance(config))
			return false;

		// instance callbacks
		_AX_Renderer_Backend_Vulkan_LoadInstanceCallbacks();

		// window surface
		if (!_AX_Renderer_Backend_Vulkan_CreateWindowSurface())
			return false;

		// debug messenger
		_AX_Renderer_Backend_Vulkan_SetupDebugMessenger();

		// device
		if (!AX_Renderer_Backend_Vulkan_Device_Startup(&context, &(context.device)))
			return false;

		// device callbacks
		_AX_Renderer_Backend_Vulkan_LoadDeviceCallbacks();

		// swapchain
		if (!AX_Renderer_Backend_Vulkan_Swapchain_Startup(
			&context,
			AX_Window_GetWidth(context.window),
			AX_Window_GetHeight(context.window),
			&(context.swapchain)
		))
			return false;

		// render pass
		if (!AX_Renderer_Backend_Vulkan_RenderPass_Startup(
			&context,
			AX_Math_Vec2_Zero(),
			AX_Math_Vec2_Zero(),
			AX_Math_Vec4_Construct(0.0f, 0.0f, 0.2f, 0.1f),
			1.0f,
			0,
			&(context.mainRenderPass)
		))
			return false;

		// swapchain framebuffers
		{
			AX_RENDERER_ALLOCATE(SVulkanFramebuffer, context.swapchain.imageCount, framebuffers);
			context.swapchain.framebuffers = framebuffers;

			if (!_AX_Renderer_Backend_Vulkan_RegenerateFramebuffers())
				return false;
		}

		// command buffers
		if (!_AX_Renderer_Backend_Vulkan_CreateCommandBuffer())
			return false;

		// sync object
		if (!_AX_Renderer_Backend_Vulkan_CreateSync())
			return false;

		// images in flight
		{
			AX_RENDERER_ALLOCATE(SVulkanFence *, context.swapchain.imageCount, inFlightImages);
			context.inFlightImages = inFlightImages;

			for (UInt32 i = 0; i < context.swapchain.imageCount; ++i)
				context.inFlightImages[i] = NULL;

			AX_LOG_DEBUG("Engine", "Images in flight allocated.");
		}

		// object shader
		if (!AX_Renderer_Backend_Vulkan_Shader_Startup(
			&context,
			"Shaders",
			"BasicShader",
			&(context.objectShader)
		))
			return false;

		// buffers
		{
			const ByteSize vertexBufferSize = sizeof(SVertex) * (1024 * 1024);
			const ByteSize indexBufferSize = sizeof(UInt32) * (1024 * 1024);

			if (!_AX_Renderer_Backend_Vulkan_CreateBuffers(
				vertexBufferSize,
				indexBufferSize
			))
				return false;
		}

		// TODO: temp - remove this
		{
			const Float f = 1.0f;

			SVertex vertices[] = {
				[0] = {	
					.position = {
						.x = -0.5f * f,
						.y = -0.5f * f
					}
				},
				
				[1] = {	
					.position = {
						.x = 0.5f * f,
						.y = -0.5f * f
					}
				},
				
				[2] = {	
					.position = {
						.x = 0.5f * f,
						.y = 0.5f * f
					}
				},
				
				[3] = {	
					.position = {
						.x = -0.5f * f,
						.y = 0.5f * f
					}
				}
			};

			UInt32 indices[] = { 0, 1, 2, 0, 2, 3 };

			if (!_AX_Renderer_Backend_Vulkan_UploadData(
				context.device.graphicsCommandPool,
				0,
				context.graphicsQueue.instance,
				0,
				sizeof(SVertex) * AX_STATIC_ARRAY_SIZE(vertices),
				vertices,
				&(context.vertexBuffer)
			))
				return false;

			if (!_AX_Renderer_Backend_Vulkan_UploadData(
				context.device.graphicsCommandPool,
				0,
				context.graphicsQueue.instance,
				0,
				sizeof(UInt32) * AX_STATIC_ARRAY_SIZE(indices),
				indices,
				&(context.indexBuffer)
			))
				return false;
		}
	}

	AX_LOG_DEBUG("Engine", "Vulkan backend startup completed.");
	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_OnShutdown
(SRenderBackend backend)
{
	if (!backend)
		return false;

	// shutdown
	{
		// wait for all operations to complete
		AX_VK_ASSERT(vkDeviceWaitIdle(context.device.instance));

		// buffers
		if (!_AX_Renderer_Backend_Vulkan_DestroyBuffers())
			return false;

		// object shader
		if (!AX_Renderer_Backend_Vulkan_Shader_Shutdown(&(context.objectShader)))
			return false;

		// images in flight
		{
			AX_RENDERER_DEALLOCATE(SVulkanFence, context.swapchain.imageCount, context.inFlightImages);
			AX_LOG_DEBUG("Engine", "Images in flight deallocated.");
		}

		// sync object
		if (!_AX_Renderer_Backend_Vulkan_DestroySync())
			return false;

		// command buffers
		{
			for (UInt32 i = 0; i < context.swapchain.imageCount; ++i) {
				if (context.arrCommandBuffers[i].instance) {
					if (!AX_Renderer_Backend_Vulkan_CommandBuffer_Deallocate(
						context.device.graphicsCommandPool,
						&(context.arrCommandBuffers[i])
					))
						return false;
				}
			}

			AX_LOG_DEBUG("Engine", "Graphics command buffers deallocated.");
		}

		// swapchain framebuffers
		{
			for (UInt32 i = 0; i < context.swapchain.imageCount; ++i) {
				if (!AX_Renderer_Backend_Vulkan_Framebuffer_Shutdown(i, &(context.swapchain.framebuffers[i])))
					return false;
			}

			AX_RENDERER_DEALLOCATE(SVulkanFramebuffer, context.swapchain.imageCount, context.swapchain.framebuffers);

			AX_LOG_DEBUG("Engine", "Swapchain framebuffers destroyed.");
		}

		// render pass
		if (!AX_Renderer_Backend_Vulkan_RenderPass_Shutdown(&(context.mainRenderPass)))
			return false;

		// swapchain
		if (!AX_Renderer_Backend_Vulkan_Swapchain_Shutdown(&(context.swapchain)))
			return false;

		// device
		if (!AX_Renderer_Backend_Vulkan_Device_Shutdown(&(context.device)))
			return false;

		// debug messenger
		#if AX_VK_VALIDATION_LAYERS_ENABLED
		{
			context.instanceCallbacks.DestroyDebugUtilsMessengerEXT(
				context.instance,
				context.debugMessenger,
				context.allocator
			);

			AX_LOG_DEBUG("Engine", "Vulkan debug messenger destroyed.");
		}
		#endif

		// Vulkan surface
		{
			vkDestroySurfaceKHR(context.instance, context.surface, context.allocator);
			AX_LOG_DEBUG("Engine", "Vulkan surface destroyed.");
		}

		// context instance
		{
			vkDestroyInstance(context.instance, context.allocator);
			AX_LOG_DEBUG("Engine", "Vulkan context instance destroyed.");
		}

		// extensions and layers lists
		{
			AX_Array_Destruct(context.arrEnabledExtensions);
			AX_LOG_DEBUG("Engine", "Enabled extensions list deallocated.");

			AX_Array_Destruct(context.arrEnabledLayers);
			AX_LOG_DEBUG("Engine", "Enabled layers deallocated.");
		}
	}

	AX_HAL_Memory_Memzero(&context, sizeof(SVulkanContext));

	AX_LOG_DEBUG("Engine", "Vulkan backend shutdown completed.");
	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_OnResized
(SRenderBackend backend, Int32 width, Int32 height)
{
	if (!backend)
		return false;

	context.framebufferSizeIteration++;
	AX_LOG_DEBUG(
		"Engine",
		__FUNCTION__" called - resized to %d, %d (iteration: %d)",
		width,
		height,
		context.framebufferSizeIteration
	);

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_OnFrameBegin
(SRenderBackend backend, const Float deltaTime)
{
	if (!backend)
		return false;

	if (context.recreatingSwapchain) {
		AX_LOG_DEBUG("Engine", "Swapchain recreation in progress...");
		return false;
	}

	if (context.framebufferSizeIteration != context.lastFramebufferSizeIteration) {
		if (!_AX_Renderer_Backend_Vulkan_RecreateSwapchain())
			return false;

		AX_LOG_DEBUG("Engine", "Swapchain reecreated.");
		return false;
	}

	if (!AX_Renderer_Backend_Vulkan_Fence_Wait(&(context.sync.inFlightFences[context.currentFrame]), UINT64_MAX))
		return false;

	if (!AX_Renderer_Backend_Vulkan_Swapchain_AcquireNextImage(
		&(context.swapchain),
		UINT64_MAX,
		context.sync.imageAcquiredSemaphores[context.currentFrame],
		0,
		&(context.imageIndex)
	))
		return false;

	SVulkanCommandBuffer *commandBuffer = &(context.arrCommandBuffers[context.imageIndex]);

	// record commands
	{
		if (!AX_Renderer_Backend_Vulkan_CommandBuffer_Reset(commandBuffer))
			return false;

		if (!AX_Renderer_Backend_Vulkan_CommandBuffer_Begin(commandBuffer, false, false, false))
			return false;
	}

	// dynamic state
	{
		const UInt32 framebufferWidth = AX_Window_GetWidth(context.window);
		const UInt32 framebufferHeight = AX_Window_GetHeight(context.window);

		VkViewport viewport = {
			.x = 0.0f,
			.y = AX_CAST(Float, framebufferHeight),
			.width = AX_CAST(Float, framebufferWidth),
			.height = AX_CAST(Float, framebufferHeight) * -1.0f,
			.minDepth = 0.0f,
			.maxDepth = 1.0f
		};

		VkRect2D scissor = {
			.offset = { 0, 0 },
			.extent = { framebufferWidth, framebufferHeight }
		};

		vkCmdSetViewport(commandBuffer->instance, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer->instance, 0, 1, &scissor);

		context.mainRenderPass.size.w = AX_CAST(Float, framebufferWidth);
		context.mainRenderPass.size.h = AX_CAST(Float, framebufferHeight);

		if (!AX_Renderer_Backend_Vulkan_RenderPass_Begin(
			commandBuffer,
			&(context.mainRenderPass),
			context.swapchain.framebuffers[context.imageIndex].instance
		))
			return false;
	}

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_OnFrameEnd
(SRenderBackend backend, const Float deltaTime)
{
	if (!backend)
		return false;

	SVulkanCommandBuffer *commandBuffer = &(context.arrCommandBuffers[context.imageIndex]);

	if (!AX_Renderer_Backend_Vulkan_RenderPass_End(commandBuffer, &(context.mainRenderPass)))
		return false;

	if (!AX_Renderer_Backend_Vulkan_CommandBuffer_End(commandBuffer))
		return false;

	if (context.inFlightImages[context.imageIndex] != NULL) {
		if (!AX_Renderer_Backend_Vulkan_Fence_Wait(
			context.inFlightImages[context.imageIndex],
			UINT64_MAX
		))
			return false;
	}

	context.inFlightImages[context.imageIndex] = &(context.sync.inFlightFences[context.currentFrame]);

	if (!AX_Renderer_Backend_Vulkan_Fence_Reset(&(context.sync.inFlightFences[context.currentFrame])))
		return false;

	VkPipelineStageFlags flags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkSubmitInfo submitInfo = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,

		.commandBufferCount = 1,
		.pCommandBuffers = &(commandBuffer->instance),

		.signalSemaphoreCount = 1,
		.pSignalSemaphores = &(context.sync.renderCompleteSemaphores[context.currentFrame]),

		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &(context.sync.imageAcquiredSemaphores[context.currentFrame]),

		.pWaitDstStageMask = flags
	};

	VkResult result = vkQueueSubmit(
		context.graphicsQueue.instance,
		1,
		&submitInfo,
		context.sync.inFlightFences[context.currentFrame].instance
	);

	if (result != VK_SUCCESS) {
		AX_LOG_ERROR("Engine", "Failed to submit queue!");
		return false;
	}

	if (!AX_Renderer_Backend_Vulkan_CommandBuffer_Update_Submitted(commandBuffer))
		return false;

	if (!AX_Renderer_Backend_Vulkan_Swapchain_PresentImage(
		&(context.swapchain),
		context.presentQueue.instance,
		context.sync.renderCompleteSemaphores[context.currentFrame],
		context.imageIndex
	))
		return false;

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_UpdateGlobalState
(SRenderBackend backend, const UMat4 projection, const UMat4 view, const UVec3 viewPosition, const UVec4 ambientColor, const Int32 mode)
{
	if (!backend)
		return false;
	
	SVulkanCommandBuffer *commandBuffer = &(context.arrCommandBuffers[context.imageIndex]);

	if(!AX_Renderer_Backend_Vulkan_Shader_Apply(&(context.objectShader)))
		return false;

	context.objectShader.globalUniform.projection = projection;
	context.objectShader.globalUniform.view = view;

	if (!AX_Renderer_Backend_Vulkan_Shader_UpdateGlobalState(&(context.objectShader)))
		return false;

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Vulkan_UpdateObject
(SRenderBackend backend, const UMat4 model)
{
	if (!backend)
		return false;
	
	SVulkanCommandBuffer *commandBuffer = &(context.arrCommandBuffers[context.imageIndex]);
	
	if (!AX_Renderer_Backend_Vulkan_Shader_UpdateObject(&(context.objectShader), model))
		return false;

	// TODO: temp - remove this
	if (!AX_Renderer_Backend_Vulkan_Shader_Apply(&(context.objectShader)))
		return false;

	// bind vertex buffer
	{
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer->instance, 0, 1, &(context.vertexBuffer.instance), offsets);
	}

	// bind index buffer
	vkCmdBindIndexBuffer(commandBuffer->instance, context.indexBuffer.instance, 0, VK_INDEX_TYPE_UINT32);

	// issue drawing
	vkCmdDrawIndexed(commandBuffer->instance, 6, 1, 0, 0, 0);

	return true;
}


