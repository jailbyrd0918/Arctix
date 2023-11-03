#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Renderer/Backend.h"


AX_API
Bool
AX_Renderer_Backend_DirectX_OnStartup
(SRenderBackend backend, SRenderBackendConfig config, VoidPtr window);

AX_API
Bool
AX_Renderer_Backend_DirectX_OnShutdown
(SRenderBackend backend);

AX_API
Bool
AX_Renderer_Backend_DirectX_OnResized
(SRenderBackend backend, Int32 width, Int32 height);

AX_API
Bool
AX_Renderer_Backend_DirectX_OnFrameBegin
(SRenderBackend backend, const Float deltaTime);

AX_API
Bool
AX_Renderer_Backend_DirectX_OnFrameEnd
(SRenderBackend backend, const Float deltaTime);




