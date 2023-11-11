#include "Arctix/Core/Modules/Window/WindowModule.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Misc/Assertions/Assertions.h"
#include "Arctix/Core/Delegates/Unicast/Unicast.h"
#include "Arctix/Core/Modules/Input/InputModule.h"

#include <SDL2/SDL.h>


typedef
struct AX_Module_Window_State
{
	SDL_Window *	sdlWindow;
	Int32		x, y, width, height;
	Bool		isInitialized;
}
AX_Module_Window_State;

static AX_Module_Window_State *state;


AX_API AX_INLINE
ByteSize
AX_Module_Window_GetModuleSize
(void)
{
	return sizeof(AX_Module_Window_State);
}

void
_AX_Module_Window_HandleResize
(VoidPtr userdata, SDL_Event *event)
{
	switch (event->type) {
		case SDL_WINDOWEVENT:
		{
			switch (event->window.event) {
				case SDL_WINDOWEVENT_RESTORED:
				case SDL_WINDOWEVENT_SIZE_CHANGED:
				{
					Int32 width, height;
					SDL_GetWindowSize(state->sdlWindow, &width, &height);

					SWinResizeData data = {
						.eventCode = AX_EVENTCODE_RESIZED,
						.width = width,
						.height = height
					};

					AX_Module_Input_InvokeEvent(AX_EVENTCODE_RESIZED, &data);
				}
				break;

				case SDL_WINDOWEVENT_MINIMIZED:
				{
					SWinResizeData data = {
						.eventCode = AX_EVENTCODE_RESIZED,
						.width = event->window.data1,
						.height = event->window.data2
					};

					AX_Module_Input_InvokeEvent(AX_EVENTCODE_RESIZED, &data);
				}
				break;

				default:
					break;
			}
		}
		break;

		default:
			break;
	}
}

AX_API
Bool
AX_Module_Window_Startup
(VoidPtr module, ReadOnlyString title, const Int32 x, const Int32 y, const Int32 width, const Int32 height)
{
	if (!module)
		return false;

	state = AX_CAST(AX_Module_Window_State *, module);

	if (state->isInitialized)
		return false;

	// init state properties
	{
		// construct SDL window
		{
			SDL_WindowFlags winFlags = (SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN); // TODO: make this API independent
			state->sdlWindow = SDL_CreateWindow(title, x, y, width, height, winFlags);
			AX_ASSERT_MESSAGE(state->sdlWindow, "failed to construct SDL window!");
		}

		SDL_GetWindowPosition(state->sdlWindow, &(state->x), &(state->y));
		SDL_GetWindowSize(state->sdlWindow, &(state->width), &(state->height));

		// set handle window resize callback
		SDL_SetEventFilter(AX_CAST(SDL_EventFilter, _AX_Module_Window_HandleResize), NULL);

		state->isInitialized = true;
	}

	return (state->isInitialized);
}

AX_API
Bool
AX_Module_Window_Shutdown
(void)
{
	if (!state || !(state->isInitialized))
		return false;

	// deallocate and set properties to zero
	{
		SDL_DestroyWindow(state->sdlWindow);
		state->isInitialized = false;
	}

	return true;
}

Bool
AX_Module_Window_Update
(void)
{
	if (!state || !(state->isInitialized))
		return false;

	SDL_Event event;
	

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
			{
				SInputEventData data = {
					.eventCode = AX_EVENTCODE_APP_QUIT
				};

				AX_Module_Input_InvokeEvent(AX_EVENTCODE_APP_QUIT, &data);
			}
			break;

			case SDL_KEYUP:
			case SDL_KEYDOWN:
			{
				Bool pressed = (event.type == SDL_KEYDOWN);
				EKeyCode key = AX_CAST(EKeyCode, event.key.keysym.scancode);

				AX_Module_Input_ProcessKey(key, pressed);
			}
			break;

			case SDL_MOUSEMOTION:
			{
				Int32	x = event.motion.x,
					y = event.motion.y;

				AX_Module_Input_ProcessMouse_Move(x, y);
			}
			break;

			case SDL_MOUSEWHEEL:
			{
				Int8 value = AX_CAST(Int8, event.wheel.y);
				
				if (value)
					AX_Module_Input_ProcessMouse_Scroll(value);

				SInputMouseScrollData data = {
					.eventCode = AX_EVENTCODE_MOUSE_SCROLLED,
					.value = event.wheel.y
				};

				AX_Module_Input_InvokeEvent(AX_EVENTCODE_MOUSE_SCROLLED, &data);
			}
			break;

			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
			{
				Bool pressed = (event.button.type == SDL_MOUSEBUTTONDOWN);
				EMouseCode code;

				switch (event.button.button) {
					case SDL_BUTTON_LEFT:
						code = AX_MOUSECODE_LEFT;
						break;

					case SDL_BUTTON_MIDDLE:
						code = AX_MOUSECODE_MIDDLE;
						break;

					case SDL_BUTTON_RIGHT:
						code = AX_MOUSECODE_RIGHT;
						break;

					default:
						code = AX_MOUSECODE_INVALID;
						break;
				}

				if (code != AX_MOUSECODE_INVALID)
					AX_Module_Input_ProcessMouse_Code(code, pressed);
			}
			break;

			default:
				break;
		}
	}

	return true;
}


AX_API AX_INLINE
VoidPtr
AX_Module_Window_GetModuleState
(void)
{
	return AX_CAST(VoidPtr, state);
}

AX_API
Bool
AX_Module_Window_SetModuleState
(VoidPtr newState)
{
	if (!state)
		return false;

	AX_HAL_Memory_Memcpy(state, newState, sizeof(AX_Module_Window_State));
	return true;
}

