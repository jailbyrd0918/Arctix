#include "Arctix/Renderer/Module/RenderModule.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Math/Matrix/Mat4.h"
#include "Arctix/Core/Math/Vector/Vec3.h"
#include "Arctix/Core/Math/Vector/Vec4.h"
#include "Arctix/Core/Math/Quaternion/Quat.h"
#include "Arctix/Core/Platform/Window/Window.h"

#include "Arctix/Renderer/Backend/RenderBackend.h"


typedef
struct AX_Module_Render_State
{
	AX_Render_Backend		backend;

	Bool				isInitialized;

	UMat4				projection;
	UMat4				view;

	Float				nearZ;
	Float				farZ;
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

		state->nearZ = 0.1f;
		state->farZ = 1000.0f;

		state->projection = AX_Math_Mat4_Perspective(
			AX_MATH_DEG_TO_RAD(45.0f), 
			AX_CAST(Float, AX_Window_GetWidth(window)) / AX_CAST(Float, AX_Window_GetHeight(window)),
			state->nearZ, 
			state->farZ
		);

		UVec3 position = AX_Math_Vec3_Construct(0, 0, -30);
		state->view = AX_Math_Mat4_MakeTranslation(&position);
		AX_Math_Mat4_Inverse(&(state->view));

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

	state->projection = AX_Math_Mat4_Perspective(
		AX_MATH_DEG_TO_RAD(45.0f),
		AX_CAST(Float, width) / AX_CAST(Float, height),
		state->nearZ,
		state->farZ
	);

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
	
	// update global state
	{
		if (!(state->backend.updateGlobalState(
			&(state->backend), 
			state->projection, 
			state->view, 
			AX_Math_Vec3_Zero(), 
			AX_Math_Vec4_Zero(), 
			0
		)))
			return false;
	}

	// update object
	{
		static Float angle = 0.0f;
		static Float aval = 0.1f;

		angle += aval;
		UQuat rotation = AX_Math_Quat_ConvertFromAxisAngle(AX_Math_Vec3_Forward(), angle, false);
		UMat4 model = AX_Math_Quat_ConvertToRotationMat(rotation);
		AX_Math_Mat4_Inverse(&model);
		
		if (!(state->backend.updateObject(&(state->backend), model)))
			return false;
	}

	if (!_AX_Module_Render_OnFrameEnd(renderData.deltaTime))
		return false;

	return true;
}

AX_API AX_INLINE
void
AX_Module_Render_SetProjection
(UMat4 projection)
{
	state->projection = projection;
}

AX_API AX_INLINE
void
AX_Module_Render_SetView
(UMat4 view)
{
	state->view = view;
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




