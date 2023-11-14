#include "Arctix/Resources/Module/ResourcesModule.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Memory/Allocators/MemArena.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"
#include "Arctix/Core/Misc/Assertions/Assertions.h"

#include "Arctix/Resources/Texture/TextureModule.h"

#include <SDL_image.h>

typedef
struct AX_Resources_Module_State
{
	SResourcesConfig 
		resourceConfig;

	SMemArena
		moduleAllocator;

	VoidPtr
		textureModule;

}
AX_Resources_Module_State;


static AX_Resources_Module_State *state;


AX_API AX_INLINE
ByteSize
AX_Module_Resources_GetModuleSize
(void)
{
	return sizeof(AX_Resources_Module_State);
}

AX_API AX_INLINE
ByteSize
AX_Module_Resources_GetTotalSize
(void)
{
	return 
		sizeof(AX_Resources_Module_State) +
		AX_Module_Texture_GetModuleSize();
}

AX_API
Bool
AX_Module_Resources_Startup
(VoidPtr module, const SResourcesConfig config, const SMemArena moduleAllocator)
{
	if (!module)
		return false;

	// startup SDL_image
	{
		if (!IMG_Init(IMG_INIT_PNG))
			return false;

		AX_LOG_TRACE("Engine", "SDL_image init completed (PNG mode enabled).");
	}

	state = AX_CAST(AX_Resources_Module_State *, module);
	state->resourceConfig = config;
	state->moduleAllocator = moduleAllocator;

	// module startup
	{
		// texture module
		{
			state->textureModule = AX_Memory_Arena_AllocateBlock(moduleAllocator, AX_Module_Texture_GetModuleSize());
			AX_ASSERT_MESSAGE(AX_Module_Texture_Startup(state->textureModule, &(state->resourceConfig)), "Texture Module Startup - texture module startup failed!");
		}
	}

	return true;
}

AX_API
Bool
AX_Module_Resources_Shutdown
(void)
{
	// texture module
	AX_ASSERT_MESSAGE(AX_Module_Texture_Shutdown(), "Texture Module Shutdown - texture module shutdown failed!");

	// sbutdown SDL_image
	{
		IMG_Quit();
		AX_LOG_TRACE("Engine", "SDL_image shutdown completed.");
	}

	return true;
}


