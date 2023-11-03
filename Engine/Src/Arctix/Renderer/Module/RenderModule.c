#include "Arctix/Renderer/Module/RenderModule.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Renderer/Backend/RenderBackend.h"


typedef
struct AX_Module_Render_State
{
	AX_Render_Backend
		backend;

	Bool
		isInitialized;
}
AX_Module_Render_State;

struct	AX_Module_Render_State *	state;


Bool
_AX_Module_Render_OnFrameBegin
(const Float deltaTime)
{
	if (!state || !(state->isInitialized))
		return false;

	if (!(state->backend.onFrameBegin(&(state->backend), deltaTime)))
		return false;

	return true;
}

Bool
_AX_Module_Render_OnFrameEnd
(const Float deltaTime)
{
	if (!state || !(state->isInitialized))
		return false;

	if (!(state->backend.onFrameEnd(&(state->backend), deltaTime)))
		return false;

	state->backend.frameCount++;
	return true;
}


AX_API AX_INLINE
ByteSize
AX_Module_Render_GetModuleSize
(void)
{
	return sizeof(AX_Module_Render_State);
}

AX_API
Bool
AX_Module_Render_Startup
(VoidPtr module, SRenderBackendConfig backendConfig, VoidPtr window)
{
	if (!module)
		return false;

	state = AX_CAST(AX_Module_Render_State *, module);

	if (state->isInitialized == true)
		return false;

	// init state properties
	{
		if (!AX_Renderer_Backend_Construct(&(state->backend), backendConfig))
			return false;

		state->isInitialized = true;
	}

	// invoke render backend startup event
	if (!state->backend.onStartup(&(state->backend), backendConfig, window))
		return false;

	return state->isInitialized;
}

AX_API
Bool
AX_Module_Render_Shutdown
(void)
{
	if (!state || !(state->isInitialized))
		return false;

	if (!(state->backend.onShutdown(&(state->backend))))
		return false;

	return true;
}

AX_API
Bool
AX_Module_Render_OnResized
(Int32 width, Int32 height)
{
	if (!state || !(state->isInitialized))
		return false;

	if (!(state->backend.onResized(&(state->backend), width, height)))
		return false;

	return true;
}

AX_API
Bool
AX_Module_Render_RenderFrame
(SRenderData renderData)
{
	if (!state || !(state->isInitialized))
		return false;

	if (!_AX_Module_Render_OnFrameBegin(renderData.deltaTime))
		return false;

	if (!_AX_Module_Render_OnFrameEnd(renderData.deltaTime))
		return false;

	return true;
}

AX_API AX_INLINE
VoidPtr
AX_Module_Render_GetModuleState
(void)
{
	return AX_CAST(VoidPtr, state);
}

AX_API
Bool
AX_Module_Render_SetModuleState
(VoidPtr newState)
{
	if (!newState || !state || !(state->isInitialized))
		return false;

	AX_HAL_Memory_Memcpy(state, newState, sizeof(AX_Module_Render_State));
	return true;
}




