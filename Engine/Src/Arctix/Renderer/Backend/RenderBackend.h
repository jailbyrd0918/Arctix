#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Renderer/Backend.h"


AX_API
Bool
AX_Renderer_Backend_Construct
(SRenderBackend backend, SRenderBackendConfig config);

AX_API
Bool
AX_Renderer_Backend_Destruct
(SRenderBackend backend);




