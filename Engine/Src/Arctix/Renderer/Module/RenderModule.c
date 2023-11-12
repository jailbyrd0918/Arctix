#include "Arctix/Renderer/Module/RenderModule.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Math/Matrix/Mat4.h"
#include "Arctix/Core/Math/Vector/Vec3.h"
#include "Arctix/Core/Math/Vector/Vec4.h"
#include "Arctix/Core/Math/Quaternion/Quat.h"
#include "Arctix/Core/Logging/Logging.h"
#include "Arctix/Core/Platform/Window/Window.h"
#include "Arctix/Core/Containers/String/String.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"
#include "Arctix/Core/Modules/Input/InputModule.h"

#include "Arctix/Renderer/Backend/RenderBackend.h"

#include <SDL_image.h>


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

// TODO: temp - remove this
static STexture testDiffuse;


Bool
_AX_ResetTexture
(STexture *outTexture)
{
	if (!outTexture)
		return false;

	AX_HAL_Memory_Memzero(outTexture, sizeof(STexture));
	outTexture->generation = AX_INVALID_ID;

	return true;
}

Bool
_AX_LoadTextureFromFile
(ReadOnlyString textureName, STexture *outTexture)
{
	if (!outTexture)
		return false;

	AX_LOG_TRACE("Engine", "Loading texture '%s'...", textureName);

	SString filepathStr = AX_String_ConstructFormatted("Assets/Textures/%s.png", textureName);
	STexture loadTexture;

	// load texture data png
	{
		SDL_Surface *textureSurface = IMG_Load(AX_String_GetString(filepathStr));
		if (!textureSurface) {
			SDL_FreeSurface(textureSurface);
			return false;
		}

		outTexture->width = textureSurface->w;
		outTexture->height = textureSurface->h;
		outTexture->channelCount = textureSurface->format->BytesPerPixel;
		outTexture->hasTransparency = (textureSurface->format->Amask != 0);
		
		BytePtr textureData = textureSurface->pixels;
		if (textureData) {
			UInt32 currentGen = outTexture->generation;
			outTexture->generation = AX_INVALID_ID;

			ByteSize totalSize = outTexture->width * outTexture->height * outTexture->channelCount;
			
			if (!AX_Module_Render_CreateTexture(
				textureName,
				outTexture->width,
				outTexture->height,
				outTexture->channelCount,
				textureData,
				outTexture->hasTransparency,
				&loadTexture
			))
				return false;

			STexture oldTexture = *outTexture;
			*outTexture = loadTexture;

			AX_Module_Render_DestroyTexture(&oldTexture);

			if (currentGen == AX_INVALID_ID)
				outTexture->generation = 0;
			else
				outTexture->generation = currentGen + 1;
		}

		SDL_FreeSurface(textureSurface);
	}

	AX_String_Destruct(filepathStr);

	AX_LOG_TRACE("Engine", "Texture '%s' loaded.", textureName);
	return true;
}

// TODO (end): temmp - remove this


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

	if (!IMG_Init(IMG_INIT_PNG))
		return false;
	AX_LOG_TRACE("Engine", "SDL_image init completed (PNG mode enabled).");

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

	state->backend.defaultTexture = &(state->defaultTexture);

	// invoke render backend startup event
	if (!state->backend.onStartup(&(state->backend), backendConfig, window))
		return false;

	// create default texture
	if (!_AX_Module_Render_CreateDefaultTexture())
		return false;

	// TODO: temp - remove this
	AX_Module_Input_RegisterEvent(AX_EVENTCODE_TESTUNIT_00, AX_Module_Render_Event_OnDebug);

	// TODO: temp remove this
	// reset test diffuse
	if (!_AX_ResetTexture(&testDiffuse))
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

	// TODO: temp - remove this
	{
		AX_Module_Input_UnregisterEvent(AX_EVENTCODE_TESTUNIT_00, AX_Module_Render_Event_OnDebug);

		if (!AX_Module_Render_DestroyTexture(&testDiffuse))
			return false;
	}

	// destroy default texture
	if (!AX_Module_Render_DestroyTexture(&(state->defaultTexture)))
		return false;

	if (!(state->backend.onShutdown(&(state->backend))))
		return false;

	AX_HAL_Memory_Memzero(state, sizeof(AX_Module_Render_State));

	IMG_Quit();

	AX_LOG_TRACE("Engine", "SDL_image shutdown completed.");
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
			.textures[0] = &testDiffuse
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


// TODO: temp - remove this
Bool
AX_Module_Render_Event_OnDebug
(VoidPtr sender, VoidPtr listener, VoidPtr data)
{
	ReadOnlyString textureNames[] = {
		"gravel",
		"concrete",
		"wall"
	};

	static UInt8 choice = 1;

	if (!_AX_LoadTextureFromFile(textureNames[choice], &testDiffuse))
		return false;

	choice = (choice + 1) % AX_STATIC_ARRAY_SIZE(textureNames);
	return true;
}


