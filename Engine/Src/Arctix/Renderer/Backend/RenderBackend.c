#include "Arctix/Renderer/Backend/RenderBackend.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"

#include "Arctix/Renderer/Backend/Vulkan/VkInterface.h"
#include "Arctix/Renderer/Backend/OpenGL/GLInterface.h"
#include "Arctix/Renderer/Backend/DirectX/DxInterface.h"


AX_API
Bool
AX_Renderer_Backend_Construct
(SRenderBackend backend, SRenderBackendConfig config)
{
	if (!backend)
		return false;

	backend->frameCount = 0;
	AX_HAL_Memory_Memcpy(&(backend->properties), &config, sizeof(SRenderBackendConfig));

	switch (config.mode) {
		case AX_RENDER_BACKEND_VULKAN:
		{
			backend->onStartup = AX_Renderer_Backend_Vulkan_OnStartup;
			backend->onShutdown = AX_Renderer_Backend_Vulkan_OnShutdown;
			backend->onResized = AX_Renderer_Backend_Vulkan_OnResized;
			backend->onFrameBegin = AX_Renderer_Backend_Vulkan_OnFrameBegin;
			backend->onFrameEnd = AX_Renderer_Backend_Vulkan_OnFrameEnd;
			backend->updateGlobalState = AX_Renderer_Backend_Vulkan_UpdateGlobalState;
			backend->updateObject = AX_Renderer_Backend_Vulkan_UpdateObject;
			backend->createTexture = AX_Renderer_Backend_Vulkan_CreateTexture;
			backend->destroyTexture = AX_Renderer_Backend_Vulkan_DestroyTexture;
		}
		break;

		case AX_RENDER_BACKEND_OPENGL:
		{
			backend->onStartup = AX_Renderer_Backend_OpenGL_OnStartup;
			backend->onShutdown = AX_Renderer_Backend_OpenGL_OnShutdown;
			backend->onResized = AX_Renderer_Backend_OpenGL_OnResized;
			backend->onFrameBegin = AX_Renderer_Backend_OpenGL_OnFrameBegin;
			backend->onFrameEnd = AX_Renderer_Backend_OpenGL_OnFrameEnd;
		}
		break;

		case AX_RENDER_BACKEND_DIRECTX:
		{
			backend->onStartup = AX_Renderer_Backend_DirectX_OnStartup;
			backend->onShutdown = AX_Renderer_Backend_DirectX_OnShutdown;
			backend->onResized = AX_Renderer_Backend_DirectX_OnResized;
			backend->onFrameBegin = AX_Renderer_Backend_DirectX_OnFrameBegin;
			backend->onFrameEnd = AX_Renderer_Backend_DirectX_OnFrameEnd;
		}
		break;

		default:
			// not supposed to reach here
			return false;
	}

	return true;
}

AX_API
Bool
AX_Renderer_Backend_Destruct
(SRenderBackend backend)
{
	if (!backend)
		return false;

	// set the entire backend to zero
	AX_HAL_Memory_Memzero(backend, sizeof(AX_Render_Backend));

	return true;
}

