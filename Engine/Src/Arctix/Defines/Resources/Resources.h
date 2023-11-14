#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/DataTypes.h"
#include "Arctix/Defines/Core/Memory.h"


typedef
struct AX_Resources_Config
{
	RawString	fileDir;

	UInt32		maxTextureCount;
}
SResourcesConfig;

#define AX_RESOURCES_ALLOCATE(type, count, name)						\
	type *name = AX_Module_Memory_Allocate(							\
		sizeof(type) * count,								\
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_RESOURCES)			\
	);											\
												\
	if (!name) {										\
		AX_Module_Memory_Deallocate(							\
			name,									\
			sizeof(type) * count,							\
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_RESOURCES)		\
		);										\
												\
		return false;									\
	}

#define AX_RESOURCES_DEALLOCATE(type, count, name)						\
	AX_Module_Memory_Deallocate(								\
		name,										\
		sizeof(type) * count,								\
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_RESOURCES)			\
	);

