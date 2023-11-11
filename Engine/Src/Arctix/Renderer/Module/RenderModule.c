#include "Arctix/Renderer/Module/RenderModule.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Math/Matrix/Mat4.h"
#include "Arctix/Core/Math/Vector/Vec3.h"
#include "Arctix/Core/Math/Vector/Vec4.h"
#include "Arctix/Core/Math/Quaternion/Quat.h"
#include "Arctix/Core/Logging/Logging.h"
#include "Arctix/Core/Platform/Window/Window.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"

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

	STexture			defaultTexture;
}
AX_Module_Render_State;

static AX_Module_Render_State *state;


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

Bool
_AX_Module_Render_CreateDefaultTexture
(void)
{
	AX_LOG_TRACE("Engine", "Creating default texture...");

	#define	DIMENSION	256
	#define	CHANNELS	4
	#define	GRID_SIZE	32

	AX_RENDERER_ALLOCATE(UInt8, (DIMENSION * DIMENSION) * CHANNELS, pixels);
	AX_HAL_Memory_Memset(pixels, 0x00FFFF, (DIMENSION * DIMENSION) * CHANNELS);

	// black and purple checkered texture
	for (UInt64 row = 0; row < DIMENSION; ++row) {
		for (UInt64 col = 0; col < DIMENSION; ++col) {
			UInt64 index = (row * DIMENSION) + col;
			UInt64 bitmapIndex = index * CHANNELS;

			if ((((row / GRID_SIZE) + (col / GRID_SIZE)) % 2) == 0) {
				pixels[bitmapIndex + 0] = 128;
				pixels[bitmapIndex + 1] = 0;
				pixels[bitmapIndex + 2] = 128;
				pixels[bitmapIndex + 3] = 255;
			}
			else {
				pixels[bitmapIndex + 0] = 0;
				pixels[bitmapIndex + 1] = 0;
				pixels[bitmapIndex + 2] = 0;
				pixels[bitmapIndex + 3] = 255;
			}
		}
	}

	if (!AX_Module_Render_CreateTexture(
		"default",
		DIMENSION,
		DIMENSION,
		CHANNELS,
		pixels,
		false,
		&(state->defaultTexture)
	))
		return false;

	AX_RENDERER_DEALLOCATE(UInt8, (DIMENSION * DIMENSION) * CHANNELS, pixels);

	AX_LOG_TRACE("Engine", "Default texture created.");
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

	// create default texture
	if (!_AX_Module_Render_CreateDefaultTexture())
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

	// destroy default texture
	if (!AX_Module_Render_DestroyTexture(&(state->defaultTexture)))
		return false;

	if (!(state->backend.onShutdown(&(state->backend))))
		return false;

	AX_HAL_Memory_Memzero(state, sizeof(AX_Module_Render_State));

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
		UVec3 zeroVec = AX_Math_Vec3_Zero();
		UMat4 model = AX_Math_Mat4_MakeTranslation(&zeroVec);

		SGeometryData data = {
			.objectID = 0,
			.model = model,
			.textures[0] = &(state->defaultTexture)
		};

		if (!(state->backend.updateObject(&(state->backend), data)))
			return false;
	}

	if (!_AX_Module_Render_OnFrameEnd(renderData.deltaTime))
		return false;

	return true;
}

AX_API
Bool
AX_Module_Render_CreateTexture
(ReadOnlyString name, const Int32 width, const Int32 height, const UInt8 channelCount, const BytePtr pixels, const Bool hasTransparency, STexture *outTexture)
{
	if (!(state->backend.createTexture(&(state->backend), name, width, height, channelCount, pixels, hasTransparency, outTexture)))
		return false;

	return true;
}

AX_API
Bool
AX_Module_Render_DestroyTexture
(STexture *outTexture)
{
	if (!(state->backend.destroyTexture(&(state->backend), outTexture)))
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




