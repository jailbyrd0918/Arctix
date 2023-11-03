#include "Arctix/Core/Platform/Window/Window.h"

#include "Arctix/Core/Modules/Window/WindowModule.h"

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
SWindowConfig
AX_Window_GetProperties
(VoidPtr window)
{
	#define	WINDOW	AX_CAST(AX_Module_Window_State *, window)

	return
		(!window) ?
		AX_STRUCT(SWindowConfig, 0) :
		AX_STRUCT(
			SWindowConfig,
			.title = SDL_GetWindowTitle(WINDOW->sdlWindow),
			.x = WINDOW->x,
			.y = WINDOW->y,
			.width = WINDOW->width,
			.height = WINDOW->height
		);
}

AX_API AX_INLINE
ReadOnlyString
AX_Window_GetTitle
(VoidPtr window)
{
	#define	WINDOW	AX_CAST(AX_Module_Window_State *, window)
	return (!window) ? NULL : SDL_GetWindowTitle(WINDOW->sdlWindow);
}

AX_API AX_INLINE
Int32
AX_Window_GetPosX
(VoidPtr window)
{
	#define	WINDOW	AX_CAST(AX_Module_Window_State *, window)
	return (!window) ? 0 : (WINDOW->x);
}

AX_API AX_INLINE
Int32
AX_Window_GetPosY
(VoidPtr window)
{
	#define	WINDOW	AX_CAST(AX_Module_Window_State *, window)
	return (!window) ? 0 : (WINDOW->y);
}

AX_API AX_INLINE
Int32
AX_Window_GetWidth
(VoidPtr window)
{
	#define	WINDOW	AX_CAST(AX_Module_Window_State *, window)
	return (!window) ? 0 : (WINDOW->width);
}

AX_API AX_INLINE
Int32
AX_Window_GetHeight
(VoidPtr window)
{
	#define	WINDOW	AX_CAST(AX_Module_Window_State *, window)
	return (!window) ? 0 : (WINDOW->height);
}

AX_API
Bool
Ax_Window_SetProperties
(VoidPtr window, SWindowConfig properties)
{
	if (!window)
		return false;

	state = AX_CAST(AX_Module_Window_State *, window);

	// update state properties
	{
		state->x = properties.x;
		state->y = properties.y;
		state->width = properties.width;
		state->height = properties.height;
	}

	// update SDL window
	{
		SDL_SetWindowTitle(state->sdlWindow, properties.title);
		SDL_SetWindowPosition(state->sdlWindow, properties.x, properties.y);
		SDL_SetWindowSize(state->sdlWindow, properties.width, properties.height);
	}

	return true;
}

AX_API
Bool
AX_Window_SetTitle
(VoidPtr window, ReadOnlyString newTitle)
{
	if (!window)
		return false;

	state = AX_CAST(AX_Module_Window_State *, window);

	// update SDL window title
	SDL_SetWindowTitle(state->sdlWindow, newTitle);
	return true;
}

AX_API
Bool
AX_Window_SetPosX
(VoidPtr window, Int32 newX)
{
	if (!window)
		return false;

	state = AX_CAST(AX_Module_Window_State *, window);
	state->x = newX;

	// update SDL window position
	SDL_SetWindowPosition(state->sdlWindow, newX, state->y);
	return true;
}

AX_API
Bool
AX_Window_SetPosY
(VoidPtr window, Int32 newY)
{
	if (!window)
		return false;

	state = AX_CAST(AX_Module_Window_State *, window);
	state->y = newY;

	// update SDL window position
	SDL_SetWindowPosition(state->sdlWindow, state->x, newY);
	return true;
}

AX_API
Bool
AX_Window_SetPosition
(VoidPtr window, Int32 newX, Int32 newY)
{
	if (!window)
		return false;

	state = AX_CAST(AX_Module_Window_State *, window);
	state->x = newX;
	state->y = newY;

	// update SDL window position
	SDL_SetWindowPosition(state->sdlWindow, newX, newY);
	return true;
}

AX_API
Bool
AX_Window_SetWidth
(VoidPtr window, Int32 newWidth)
{
	if (!window)
		return false;

	state = AX_CAST(AX_Module_Window_State *, window);
	state->width = newWidth;

	// update SDL window position
	SDL_SetWindowSize(state->sdlWindow, newWidth, state->height);
	return true;
}

AX_API
Bool
AX_Window_SetHeight
(VoidPtr window, Int32 newHeight)
{
	if (!window)
		return false;

	state = AX_CAST(AX_Module_Window_State *, window);
	state->height = newHeight;

	// update SDL window position
	SDL_SetWindowSize(state->sdlWindow, state->width, newHeight);
	return true;
}

AX_API
Bool
AX_Window_SetSize
(VoidPtr window, Int32 newWidth, Int32 newHeight)
{
	if (!window)
		return false;

	state = AX_CAST(AX_Module_Window_State *, window);
	state->width = newWidth;
	state->height = newHeight;

	// update SDL window position
	SDL_SetWindowSize(state->sdlWindow, newWidth, newHeight);
	return true;
}

AX_API AX_INLINE
VoidPtr
AX_Window_GetSDLWindow
(VoidPtr window)
{
	return (!window) ? NULL : AX_CAST(VoidPtr, AX_CAST(AX_Module_Window_State *, window)->sdlWindow);
}


