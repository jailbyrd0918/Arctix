#pragma once

#include "Arctix/Defines/Core/Math.h"
#include "Arctix/Defines/Core/DataTypes.h"


typedef
enum AX_Render_Backend_Mode
{
	AX_RENDER_BACKEND_VULKAN,
	AX_RENDER_BACKEND_OPENGL,
	AX_RENDER_BACKEND_DIRECTX
}
ERenderBackendMode;

typedef
struct AX_Render_Backend_Configuration
{
	ERenderBackendMode		mode;
	ReadOnlyString			appName;
}
SRenderBackendConfig;

typedef	struct AX_Render_Backend	AX_Render_Backend;
typedef	AX_Render_Backend *		SRenderBackend;

typedef
struct AX_Render_Vertex
{
	UVec3				position;
}
SVertex;

typedef
struct AX_Render_Global_Uniform
{
	UMat4				projection;
	UMat4				view;
	
	UMat4				reserved0;
	UMat4				reserved1;
}
SGlobalUniform;

struct AX_Render_Backend
{
	UInt64				frameCount;
	SRenderBackendConfig		properties;

	Bool(*onStartup)(SRenderBackend backend, SRenderBackendConfig config, VoidPtr window);
	Bool(*onShutdown)(SRenderBackend backend);
	Bool(*onResized)(SRenderBackend backend, Int32 width, Int32 height);
	Bool(*onFrameBegin)(SRenderBackend backend, const Float deltaTime);
	Bool(*onFrameEnd)(SRenderBackend backend, const Float deltaTime);
	Bool(*updateGlobalState)(SRenderBackend backend, const UMat4 projection, const UMat4 view, const UVec3 viewPosition, const UVec4 ambientColor, const Int32 mode);
	Bool(*updateObject)(SRenderBackend backend, const UMat4 model);
};

#define AX_RENDERER_ALLOCATE(type, count, name)							\
	type *name = AX_Module_Memory_Allocate(							\
		sizeof(type) * count,								\
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_RENDERER)			\
	);											\
												\
	if (!name) {										\
		AX_Module_Memory_Deallocate(							\
			name,									\
			sizeof(type) * count,							\
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_RENDERER)		\
		);										\
												\
		return false;									\
	}

#define AX_RENDERER_DEALLOCATE(type, count, name)						\
	AX_Module_Memory_Deallocate(								\
		name,										\
		sizeof(type) * count,								\
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_RENDERER)			\
	);

