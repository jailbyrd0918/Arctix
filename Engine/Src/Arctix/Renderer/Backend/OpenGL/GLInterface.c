#include "Arctix/Renderer/Backend/OpenGL/GLInterface.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"


AX_API
Bool
AX_Renderer_Backend_OpenGL_OnStartup
(SRenderBackend backend, SRenderBackendConfig config, VoidPtr window)
{
	return true;
}

AX_API
Bool
AX_Renderer_Backend_OpenGL_OnShutdown
(SRenderBackend backend)
{
	return true;
}

AX_API
Bool
AX_Renderer_Backend_OpenGL_OnResized
(SRenderBackend backend, Int32 width, Int32 height)
{
	return true;
}

AX_API
Bool
AX_Renderer_Backend_OpenGL_OnFrameBegin
(SRenderBackend backend, const Float deltaTime)
{
	return true;
}

AX_API
Bool
AX_Renderer_Backend_OpenGL_OnFrameEnd
(SRenderBackend backend, const Float deltaTime)
{
	return true;
}



