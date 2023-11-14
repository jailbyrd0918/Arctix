#include "Arctix/Core/Modules/ModuleManager.h"

#include "Arctix/Core/Platform/Filesystem/Filesystem.h"
#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Memory/Allocators/MemArena.h"
#include "Arctix/Core/Modules/Input/InputModule.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"
#include "Arctix/Core/Modules/Window/WindowModule.h"
#include "Arctix/Renderer/Module/RenderModule.h"
#include "Arctix/Resources/Module/ResourcesModule.h"

#include "Arctix/Core/Misc/Assertions/Assertions.h"


typedef
struct AX_Module_Manager_State
{
	SMemArena
		moduleAllocator;

	VoidPtr
		memoryModule,
		loggingModule,
		inputModule,
		windowModule,
		renderModule,
		resourcesModule;
}
AX_Module_Manager_State;


static	AX_Module_Manager_State *state;


AX_API
Bool
AX_ModuleManager_StartupModules
(SWindowConfig winConfig, SRenderBackendConfig backendConfig, SResourcesConfig resourcesConfig)
{
	// construct and initialize module manager state
	state = AX_HAL_Memory_Malloc(sizeof(AX_Module_Manager_State));

	ByteSize totalSize =
		AX_Module_Memory_GetModuleSize() +
		AX_Logging_GetModuleSize() +
		AX_Module_Input_GetModuleSize() +
		AX_Module_Window_GetModuleSize() +
		AX_Module_Render_GetModuleSize() +
		AX_Module_Resources_GetTotalSize();

	state->moduleAllocator = AX_Memory_Arena_Construct(totalSize);

	// modules startup
	{
		// memory module
		{
			state->memoryModule = AX_Memory_Arena_AllocateBlock(state->moduleAllocator, AX_Module_Memory_GetModuleSize());
			AX_ASSERT_MESSAGE(AX_Module_Memory_Startup(state->memoryModule), "Startup Module - memory module startup failed!");
		}

		// logging module
		{
			state->loggingModule = AX_Memory_Arena_AllocateBlock(state->moduleAllocator, AX_Logging_GetModuleSize());
			AX_ASSERT_MESSAGE(AX_Logging_Startup(state->loggingModule), "Startup Module - logging module startup failed!");
		}

		// input module
		{
			state->inputModule = AX_Memory_Arena_AllocateBlock(state->moduleAllocator, AX_Module_Input_GetModuleSize());
			AX_ASSERT_MESSAGE(AX_Module_Input_Startup(state->inputModule), "Startup Module - input module startup failed!");
		}

		// window module
		{
			state->windowModule = AX_Memory_Arena_AllocateBlock(state->moduleAllocator, AX_Module_Window_GetModuleSize());

			AX_ASSERT_MESSAGE(
				AX_Module_Window_Startup(state->windowModule, winConfig.title, winConfig.x, winConfig.y, winConfig.width, winConfig.height), 
				"Startup Module - window module startup failed!"
			);
		}

		// render module
		{
			state->renderModule = AX_Memory_Arena_AllocateBlock(state->moduleAllocator, AX_Module_Render_GetModuleSize());
		
			AX_ASSERT_MESSAGE(
				AX_Module_Render_Startup(state->renderModule, backendConfig, AX_Module_Window_GetModuleState()),
				"Startup Module - render module startup failed!"
			);
		}

		// resource module
		{
			state->resourcesModule = AX_Memory_Arena_AllocateBlock(state->moduleAllocator, AX_Module_Resources_GetModuleSize());

			AX_ASSERT_MESSAGE(
				AX_Module_Resources_Startup(state->resourcesModule, resourcesConfig, state->moduleAllocator),
				"Startup Module - resources module startup failed!"
			);
		}
	}

	return true;
}

AX_API
Bool
AX_ModuleManager_UpdateModules
(void)
{
	// window module
	AX_ASSERT_MESSAGE(AX_Module_Window_Update(), "Update Module - window module update failed!");

	return true;
}

AX_API
Bool
AX_ModuleManager_ShutdownModules
(void)
{
	// resources module
	AX_ASSERT_MESSAGE(AX_Module_Resources_Shutdown(), "Shutdown Module - resources module shutdown failed!");

	// render module
	AX_ASSERT_MESSAGE(AX_Module_Render_Shutdown(), "Shutdown Module - render module shutdown failed!");

	// window module
	AX_ASSERT_MESSAGE(AX_Module_Window_Shutdown(), "Shutdown Module - window module shutdown failed!");

	// input module
	AX_ASSERT_MESSAGE(AX_Module_Input_Shutdown(), "Shutdown Module - input module shutdown failed!");

	// logging module
	AX_ASSERT_MESSAGE(AX_Logging_Shutdown(), "Shutdown Module - logging module shutdown failed!");

	// memory module
	AX_ASSERT_MESSAGE(AX_Module_Memory_Shutdown(), "Shutdown Module - memory module shutdown failed!");

	return true;
}


