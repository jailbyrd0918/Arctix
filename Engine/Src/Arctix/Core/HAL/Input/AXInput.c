#include "Arctix/Core/HAL/Input/AXInput.h"

#include "Arctix/Core/Modules/Input/InputModule.h"


typedef
struct AX_HAL_Input_Keyboard_State
{
	Bool	keys[AX_KEYCODES_TOTAL];
}
SKeyboardState;

typedef
struct AX_HAL_Input_Mouse_State
{
	Int32	x,
		y;

	Int8	scrollValue;

	Bool	values[AX_MOUSECODES_TOTAL];
}
SMouseState;

typedef
struct AX_HAL_Input_Module_State
{
	SKeyboardState
		currKeyboardState,
		prevKeyboardState;

	SMouseState
		currMouseState,
		prevMouseState;

	Bool	isInitialized;

	SUnicastDelegate
		eventDelegates[AX_EVENTCODES_TOTAL];

	SUnicastDelegateHandle
		eventHandles[AX_EVENTCODES_TOTAL];
}
AX_HAL_Input_Module_State;

#define	MODULE_STATE	\
	AX_CAST(AX_HAL_Input_Module_State *, AX_Module_Input_GetModuleState())


// -- keyboard -- //

AX_API AX_INLINE
Bool
AX_HAL_Input_IsKeyDown
(const EKeyCode key)
{
	return (MODULE_STATE->currKeyboardState.keys[key]);
}

AX_API AX_INLINE
Bool
AX_HAL_Input_IsKeyUp
(const EKeyCode key)
{
	return !(MODULE_STATE->currKeyboardState.keys[key]);
}

AX_API AX_INLINE
Bool
AX_HAL_Input_WasKeyDown
(const EKeyCode key)
{
	return (MODULE_STATE->prevKeyboardState.keys[key]);
}

AX_API AX_INLINE
Bool
AX_HAL_Input_WasKeyUp
(const EKeyCode key)
{
	return !(MODULE_STATE->prevKeyboardState.keys[key]);
}

AX_API AX_INLINE
Bool
AX_HAL_Input_IsKeyPressed
(const EKeyCode key)
{
	return (AX_HAL_Input_IsKeyUp(key) && AX_HAL_Input_WasKeyDown(key));
}


// -- mouse -- //

AX_API AX_INLINE
Bool
AX_HAL_Input_IsMouseButtonDown
(const EMouseCode code)
{
	return (MODULE_STATE->currMouseState.values[code]);
}

AX_API AX_INLINE
Bool
AX_HAL_Input_IsMouseButtonUp
(const EMouseCode code)
{
	return !(MODULE_STATE->currMouseState.values[code]);
}

AX_API AX_INLINE
Bool
AX_HAL_Input_WasMouseButtonDown
(const EMouseCode code)
{
	return (MODULE_STATE->prevMouseState.values[code]);
}

AX_API AX_INLINE
Bool
AX_HAL_Input_WasMouseButtonUp
(const EMouseCode code)
{
	return !(MODULE_STATE->prevMouseState.values[code]);
}

AX_API AX_INLINE
Bool
AX_HAL_Input_IsMousePressed
(const EMouseCode code)
{
	return (AX_HAL_Input_IsMouseButtonUp(code) && AX_HAL_Input_WasMouseButtonDown(code));
}

AX_API AX_INLINE
Int32
AX_HAL_Input_GetMouseX
(void)
{
	return (MODULE_STATE->currMouseState.x);
}

AX_API AX_INLINE
Int32
AX_HAL_Input_GetMouseY
(void)
{
	return (MODULE_STATE->currMouseState.y);
}

AX_API AX_INLINE
Int32
AX_HAL_Input_GetPreviousMouseX
(void)
{
	return (MODULE_STATE->prevMouseState.x);
}

AX_API AX_INLINE
Int32
AX_HAL_Input_GetPreviousMouseY
(void)
{
	return (MODULE_STATE->prevMouseState.y);
}

AX_API AX_INLINE
Int8
AX_HAL_Input_GetMouseScrollValue
(void)
{
	return (MODULE_STATE->currMouseState.scrollValue);
}

AX_API AX_INLINE
Int8
AX_HAL_Input_GetPreviousMouseScrollValue
(void)
{
	return (MODULE_STATE->prevMouseState.scrollValue);
}


