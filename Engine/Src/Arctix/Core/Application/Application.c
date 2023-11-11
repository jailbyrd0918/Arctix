#include "Arctix/Core/Application/Application.h"

#include "Arctix/Core/Misc/Console/Console.h"
#include "Arctix/Core/Misc/Assertions/Assertions.h"
#include "Arctix/Core/Misc/String/AXString.h"
#include "Arctix/Core/Math/RNG/RNG.h"
#include "Arctix/Core/Logging/Logging.h"
#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/HAL/Clock/WallClock.h"
#include "Arctix/Core/HAL/Clock/HiResClock.h"
#include "Arctix/Core/HAL/Input/AXInput.h"
#include "Arctix/Core/Platform/Window/Window.h"
#include "Arctix/Core/Containers/String/String.h"
#include "Arctix/Core/Modules/ModuleManager.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"
#include "Arctix/Core/Modules/Input/InputModule.h"
#include "Arctix/Core/Modules/Window/WindowModule.h"

#include "Arctix/Gameplay/Game/Game.h"

#include "Arctix/Renderer/Module/RenderModule.h"

#include <SDL2/SDL.h>


typedef
struct AX_Application_State
{
	SGame	game;

	UInt64	prevTick,
		frameRate;
	Float64	frameRateUpdateFreqMs;
	Float	deltaTime;

	VoidPtr	window;

	Bool	active;
	Bool	suspended;

	Bool	isInitialized;
}
AX_Application_State;

static AX_Application_State *appState;


Bool
_AX_Application_OnEvent
(VoidPtr sender, VoidPtr listener, VoidPtr data)
{
	SInputEventData code = *(AX_CAST(SInputEventData *, data));

	switch (code.eventCode) {
		case AX_EVENTCODE_APP_QUIT:
		{
			AX_LOG_INFO("Engine", "EVENTCODE_APP_QUIT invoked - shutting down...");
			appState->active = false;
			return true;
		}
		break;

		default:
			break;
	}

	return false;
}

Bool
_AX_Application_OnKey
(VoidPtr sender, VoidPtr listener, VoidPtr data)
{
	SInputEventData code = *(AX_CAST(SInputEventData *, data));
	SInputKeyData keyData = *(AX_CAST(SInputKeyData *, data));

	switch (code.eventCode) {
		case AX_EVENTCODE_KEY_PRESSED:
		{
			AX_LOG_INFO("Engine", "key '%c' pressed!", AX_Input_Get_KeyCode(keyData.key));
			return true;
		}
		break;

		case AX_EVENTCODE_KEY_RELEASED:
		{
			AX_LOG_INFO("Engine", "key '%c' released!", AX_Input_Get_KeyCode(keyData.key));
			return true;
		}
		break;

		default:
			break;
	}

	return false;
}

Bool
_AX_Application_OnResized
(VoidPtr sender, VoidPtr listener, VoidPtr data)
{
	SWinResizeData keyData = *(AX_CAST(SWinResizeData *, data));

	Int32 width = AX_CAST(Int32, keyData.width);
	Int32 height = AX_CAST(Int32, keyData.height);

	Int32 currWidth = AX_Window_GetWidth(appState->window);
	Int32 currHeight = AX_Window_GetHeight(appState->window);

	if ((width != currWidth) || (height != currHeight)) {
		AX_Window_SetSize(appState->window, width, height);
		AX_LOG_INFO("Engine", "Window resized to %d, %d", width, height);

		// in case of minimization
		if ((width == 0) || (height == 0)) {
			AX_LOG_INFO("Engine", "Window minimized - suspending application...");
			appState->suspended = true;
		}
		else {
			if (appState->suspended) {
				AX_LOG_INFO("Engine", "Window restored - resuming application...");
				appState->suspended = false;
			}

			// invoke render on resized event
			AX_Module_Render_OnResized(width, height);
		}
	}

	return true;
}

Bool
_AX_Application_OnMouseScroll
(VoidPtr sender, VoidPtr listener, VoidPtr data)
{
	SInputMouseScrollData scrollData = *(AX_CAST(SInputMouseScrollData *, data));

	Int8 scrollValue = AX_CAST(Int8, scrollData.value);

	if (!AX_Gameplay_Game_OnScroll(&(appState->game), scrollValue))
		return false;

	return true;
}

void
_AX_Application_ReportStatus
(ReadOnlyString statusMsg)
{
	AX_Misc_Console_SetForegroundColorRGB(101, 191, 104);
	AX_Misc_Console_WriteLine("Application: %s", statusMsg);
	AX_Misc_Console_Reset();
}

void
_AX_Application_UpdateDeltaTime
(void)
{
	UInt64 frameDuration = 1000 / appState->frameRate;

	// delay until delay time exceeds frame duration
	UInt64 delayTime = frameDuration - (SDL_GetTicks64() - appState->prevTick);
	if (delayTime <= frameDuration)
		SDL_Delay(AX_CAST(UInt32, delayTime));

	// update delta time and refresh prev tick
	{
		appState->deltaTime = (SDL_GetTicks64() - appState->prevTick) / 1000.0f;
		appState->prevTick = SDL_GetTicks64();
	}
}


AX_API
Bool
AX_Application_PreConstruct
(SGame *game)
{
	AX_Misc_Console_Startup();
	AX_Math_RNG_Initialize(AX_HAL_WallClock_Now().timeValue);

	SDL_Init(SDL_INIT_EVERYTHING);

	appState = AX_Module_Memory_Allocate(
		sizeof(AX_Application_State),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_APPLICATION)
	);

	// init app state properties
	{
		AX_HAL_Memory_Memcpy(&(appState->game), game, sizeof(SGame));
		game->appState = appState;

		appState->prevTick = 0;
		appState->frameRate = game->gameConfig.maxFrameRate;
		appState->deltaTime = 0.f;
		appState->frameRateUpdateFreqMs = 1000;

		appState->active = true;
		appState->suspended = false;
		appState->isInitialized = true;
	}

	AX_ModuleManager_StartupModules(
		appState->game.gameConfig.winConfig,
		appState->game.gameConfig.backendConfig
	);

	// init window module state
	appState->window = AX_Module_Window_GetModuleState();

	// register events
	{
		AX_Module_Input_RegisterEvent(AX_EVENTCODE_APP_QUIT, _AX_Application_OnEvent);
		AX_Module_Input_RegisterEvent(AX_EVENTCODE_KEY_PRESSED, _AX_Application_OnKey);
		AX_Module_Input_RegisterEvent(AX_EVENTCODE_KEY_RELEASED, _AX_Application_OnKey);
		AX_Module_Input_RegisterEvent(AX_EVENTCODE_RESIZED, _AX_Application_OnResized);
		AX_Module_Input_RegisterEvent(AX_EVENTCODE_MOUSE_SCROLLED, _AX_Application_OnMouseScroll);
	}

	_AX_Application_ReportStatus("PreConstruct stage completed - proceeding to Construct stage...");
	return true;
}

AX_API
Bool
AX_Application_Construct
(void)
{
	// invoke game start event
	appState->game.gameConfig.onStart(&(appState->game));

	_AX_Application_ReportStatus("Construct stage completed - proceeding to Update stage...");
	return true;
}

#include <time.h>
AX_API
Bool
AX_Application_Update
(void)
{
	if (!appState || !(appState->isInitialized))
		return false;

	_AX_Application_ReportStatus("starting loop...");

	SHiResClock lastUpdateClock = AX_STRUCT(SHiResClock, 0);
	ReadOnlyString baseWindowTitle = appState->game.gameConfig.winConfig.title;

	do {
		if (!AX_Module_Input_Update())
			return false;

		AX_ModuleManager_UpdateModules();

		if (!(appState->suspended)) {
			// invoke game update event
			appState->game.gameConfig.onUpdate(&(appState->game), appState->deltaTime);

			// invoke backend render frame event
			SRenderData renderData = { .deltaTime = appState->deltaTime };
			AX_Module_Render_RenderFrame(renderData);
		}

		_AX_Application_UpdateDeltaTime();

		// update FPS counter on window title bar (every 1000 ms)
		{
			SHiResClock currUpdateClock = AX_HAL_HiResClock_Now();
			Float64 elaspedMs = AX_HAL_HiResClock_Duration_Milliseconds(&lastUpdateClock, &currUpdateClock);
			if (elaspedMs >= appState->frameRateUpdateFreqMs) {
				Char title[64];
				AX_Misc_String_Format(title, 64, "%s | FPS: %u", baseWindowTitle, AX_CAST(UInt32, 1.0f / appState->deltaTime));
				AX_Window_SetTitle(appState->window, title);
				lastUpdateClock = currUpdateClock;
			}
		}

	} while (appState->active);

	_AX_Application_ReportStatus("loop terminated - proceeding to Destruct stage...");
	return true;
}

AX_API
Bool
AX_Application_Destruct
(void)
{
	if (!appState || !(appState->isInitialized))
		return false;

	AX_Module_Memory_Deallocate(
		appState,
		sizeof(AX_Application_State),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_APPLICATION)
	);

	appState->isInitialized = false;

	// unregister events
	{
		AX_Module_Input_UnregisterEvent(AX_EVENTCODE_APP_QUIT, _AX_Application_OnEvent);
		AX_Module_Input_UnregisterEvent(AX_EVENTCODE_KEY_PRESSED, _AX_Application_OnKey);
		AX_Module_Input_UnregisterEvent(AX_EVENTCODE_KEY_RELEASED, _AX_Application_OnKey);
		AX_Module_Input_UnregisterEvent(AX_EVENTCODE_RESIZED, _AX_Application_OnResized);
		AX_Module_Input_UnregisterEvent(AX_EVENTCODE_MOUSE_SCROLLED, _AX_Application_OnMouseScroll);
	}

	AX_ModuleManager_ShutdownModules();

	_AX_Application_ReportStatus("Destruct stage completed - shutting down console and exiting...");

	AX_Misc_Console_Shutdown();
	return true;
}




