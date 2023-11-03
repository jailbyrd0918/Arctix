#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Renderer/Backend.h"


AX_API
Bool
AX_Renderer_Backend_OpenGL_OnStartup
(SRenderBackend backend, SRenderBackendConfig config, VoidPtr window);

AX_API
Bool
AX_Renderer_Backend_OpenGL_OnShutdown
(SRenderBackend backend);

AX_API
Bool
AX_Renderer_Backend_OpenGL_OnResized
(SRenderBackend backend, Int32 width, Int32 height);

AX_API
Bool
AX_Renderer_Backend_OpenGL_OnFrameBegin
(SRenderBackend backend, const Float deltaTime);

AX_API
Bool
AX_Renderer_Backend_OpenGL_OnFrameEnd
(SRenderBackend backend, const Float deltaTime);




