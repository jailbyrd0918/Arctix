#include "Arctix/Renderer/Backend/DirectX/DxInterface.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"


AX_API
Bool
AX_Renderer_Backend_DirectX_OnStartup
(SRenderBackend backend, SRenderBackendConfig config, VoidPtr window)
{
	return true;
}

AX_API
Bool
AX_Renderer_Backend_DirectX_OnShutdown
(SRenderBackend backend)
{
	return true;
}

AX_API
Bool
AX_Renderer_Backend_DirectX_OnResized
(SRenderBackend backend, Int32 width, Int32 height)
{
	return true;
}

AX_API
Bool
AX_Renderer_Backend_DirectX_OnFrameBegin
(SRenderBackend backend, const Float deltaTime)
{
	return true;
}

AX_API
Bool
AX_Renderer_Backend_DirectX_OnFrameEnd
(SRenderBackend backend, const Float deltaTime)
{
	return true;
}



