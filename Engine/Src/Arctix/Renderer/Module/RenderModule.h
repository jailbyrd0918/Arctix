#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Renderer/Frontend.h"
#include "Arctix/Defines/Renderer/Backend.h"
#include "Arctix/Defines/Resources/Texture.h"


AX_API AX_INLINE
ByteSize
AX_Module_Render_GetModuleSize
(void);

AX_API
Bool
AX_Module_Render_Startup
(VoidPtr module, SRenderBackendConfig backendConfig, VoidPtr window);

AX_API
Bool
AX_Module_Render_Shutdown
(void);

AX_API
Bool
AX_Module_Render_OnResized
(Int32 width, Int32 height);

AX_API
Bool
AX_Module_Render_RenderFrame
(SRenderData renderData);

AX_API
Bool
AX_Module_Render_CreateTexture
(ReadOnlyString name, const Int32 width, const Int32 height, const UInt8 channelCount, const BytePtr pixel, const Bool hasTransparency, STexture *outTexture);

AX_API
Bool
AX_Module_Render_DestroyTexture
(STexture *outTexture);

AX_API AX_INLINE
void
AX_Module_Render_SetProjection
(UMat4 projection);

AX_API AX_INLINE
void
AX_Module_Render_SetView
(UMat4 view);


AX_API AX_INLINE
VoidPtr
AX_Module_Render_GetModuleState
(void);

AX_API
Bool
AX_Module_Render_SetModuleState
(VoidPtr newState);


// TODO: temp - remove this
Bool
AX_Module_Render_Event_OnDebug
(VoidPtr sender, VoidPtr listener, VoidPtr data);
