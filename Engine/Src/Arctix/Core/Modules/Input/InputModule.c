#include "Arctix/Core/Modules/Input/InputModule.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"
#include "Arctix/Core/Delegates/Unicast/Unicast.h"
#include "Arctix/Core/Logging/Logging.h"


typedef
struct AX_Input_Keyboard_State
{
	Bool	keys[AX_KEYCODES_TOTAL];
}
SKeyboardState;

typedef
struct AX_Input_Mouse_State
{
	Int32	x, 
		y;

	Int8	scrollValue;

	Bool	values[AX_MOUSECODES_TOTAL];
}
SMouseState;

typedef
struct AX_Input_Module_State
{
	SKeyboardState
		currKeyboardState,
		prevKeyboardState;

	SMouseState
		currMouseState,
		prevMouseState;

	SUnicastDelegate
		eventDelegates[AX_EVENTCODES_TOTAL];

	SUnicastDelegateHandle
		eventHandles[AX_EVENTCODES_TOTAL];

	Bool	isInitialized;
}
AX_Input_Module_State;

static AX_Input_Module_State *	state;


AX_API AX_INLINE
ByteSize
AX_Module_Input_GetModuleSize
(void)
{
	return sizeof(AX_Input_Module_State);
}

AX_API
Bool
AX_Module_Input_Startup
(VoidPtr module)
{
	if (!module)
		return false;

	state = AX_CAST(AX_Input_Module_State *, module);
	
	if (state->isInitialized)
		return false;

	AX_HAL_Memory_Memzero(&(state->currMouseState), sizeof(SMouseState));
	AX_HAL_Memory_Memzero(&(state->prevMouseState), sizeof(SMouseState));
	AX_HAL_Memory_Memzero(&(state->currKeyboardState), sizeof(SKeyboardState));
	AX_HAL_Memory_Memzero(&(state->prevKeyboardState), sizeof(SKeyboardState));
	AX_HAL_Memory_Memzero(&(state->eventDelegates), sizeof(state->eventDelegates));

	for (UInt32 i = 0; i < AX_EVENTCODES_TOTAL; ++i)
		state->eventHandles[i] = AX_UnicastDelegate_Handle_Construct();

	state->isInitialized = true;
	return state->isInitialized;
}

AX_API
Bool
AX_Module_Input_Shutdown
(void)
{
	if (!state || !(state->isInitialized))
		return false;

	for (UInt32 i = 0; i < AX_EVENTCODES_TOTAL; ++i) {
		SUnicastDelegate delegate = state->eventDelegates[i];
		if (delegate != NULL)
			AX_UnicastDelegate_Destruct(delegate);

		SUnicastDelegateHandle handle = state->eventHandles[i];
		AX_UnicastDelegate_Handle_UnbindDelegate(handle, delegate);
		AX_UnicastDelegate_Handle_Destruct(handle);
	}

	// set all properties to zero
	AX_HAL_Memory_Memzero(state, sizeof(AX_Input_Module_State));

	return true;
}

AX_API
Bool
AX_Module_Input_Update
(void)
{
	if (!state || !(state->isInitialized))
		return false;

	// copy current states to previous states
	{
		AX_HAL_Memory_Memcpy(&(state->prevKeyboardState), &(state->currKeyboardState), sizeof(SKeyboardState));
		AX_HAL_Memory_Memcpy(&(state->prevMouseState), &(state->currMouseState), sizeof(SMouseState));
	}

	return true;
}

AX_API
Bool
AX_Module_Input_RegisterEvent
(EEventCode eventCode, FPDelegate eventCallback)
{
	if (!state || !(state->isInitialized))
		return false;

	if (!(state->eventDelegates[eventCode]))
		state->eventDelegates[eventCode] = AX_UnicastDelegate_Construct(state->eventHandles[eventCode], eventCallback);
	
	AX_UnicastDelegate_Handle_BindDelegate(state->eventHandles[eventCode], state->eventDelegates[eventCode]);

	return true;
}

AX_API
Bool
AX_Module_Input_UnregisterEvent
(EEventCode eventCode, FPDelegate eventCallback)
{
	if (!state || !(state->isInitialized))
		return false;

	SUnicastDelegate delegate = state->eventDelegates[eventCode];
	if (!delegate)
		return false;
	
	AX_UnicastDelegate_Destruct(state->eventDelegates[eventCode]);
	state->eventDelegates[eventCode] = NULL;
	return true;
}

AX_API
Bool
AX_Module_Input_InvokeEvent
(EEventCode eventCode, VoidPtr data)
{
	if (!state || !(state->isInitialized))
		return false;

	SUnicastDelegateHandle handle = state->eventHandles[eventCode];
	if (!handle)
		return false;

	AX_UnicastDelegate_Handle_Invoke(NULL, handle, data);
	return true;
}

Bool
AX_Module_Input_ProcessKey
(const EKeyCode key, const Bool pressed)
{
	if (!state || !(state->isInitialized))
		return false;

	if (state->currKeyboardState.keys[key] != pressed) {
		// update current keyboard state
		state->currKeyboardState.keys[key] = pressed;

		// immediately invoke input event
		{
			SInputKeyData data = {
				.eventCode = pressed ? AX_EVENTCODE_KEY_PRESSED : AX_EVENTCODE_KEY_RELEASED,
				.key = key
			};

			SUnicastDelegate del = state->eventDelegates[data.eventCode];
			if (!del)
				return false;

			SUnicastDelegateHandle handle = state->eventHandles[data.eventCode];
			AX_UnicastDelegate_Handle_Invoke(NULL, handle, &data);
		}
	}

	return true;
}

Bool
AX_Module_Input_ProcessMouse_Code
(const EMouseCode code, const Bool pressed)
{
	if (!state || !(state->isInitialized))
		return false;

	if (state->currMouseState.values[code] != pressed) {
		// update current mouse state
		state->currMouseState.values[code] = pressed;

		// immediately invoke input event
		{
			SInputMouseData data = {
				.eventCode = pressed ? AX_EVENTCODE_MOUSE_BTN_PRESSED : AX_EVENTCODE_MOUSE_BTN_RELEASED,
				.mouseCode = code
			};

			SUnicastDelegate del = state->eventDelegates[data.eventCode];
			if (!del)
				return false;

			SUnicastDelegateHandle handle = state->eventHandles[data.eventCode];
			AX_UnicastDelegate_Handle_Invoke(NULL, handle, &data);
		}
	}

	return true;
}

Bool
AX_Module_Input_ProcessMouse_Move
(const Int32 x, const Int32 y)
{
	if (!state || !(state->isInitialized))
		return false;

	if ((state->currMouseState.x != x) || (state->currMouseState.y != y)) {
		// update current mouse positions state
		{
			state->currMouseState.x = x;
			state->currMouseState.y = y;
		}

		// immediately invoke input event
		{
			SInputMouseMoveData data = {
				.eventCode = AX_EVENTCODE_MOUSE_MOVED,
				.x = x,
				.y = y
			};

			SUnicastDelegate del = state->eventDelegates[data.eventCode];
			if (!del)
				return false;

			SUnicastDelegateHandle handle = state->eventHandles[data.eventCode];
			AX_UnicastDelegate_Handle_Invoke(NULL, handle, &data);
		}
	}

	return true;
}

Bool
AX_Module_Input_ProcessMouse_Scroll
(const Int8 value)
{
	if (!state || !(state->isInitialized))
		return false;

	// update mouse scroll state
	state->currMouseState.scrollValue = value;

	// immediately invoke input event
	{
		SInputMouseScrollData data = {
			.eventCode = AX_EVENTCODE_MOUSE_SCROLLED,
			.value = value
		};

		SUnicastDelegate del = state->eventDelegates[data.eventCode];
		if (!del)
			return false;

		SUnicastDelegateHandle handle = state->eventHandles[data.eventCode];
		AX_UnicastDelegate_Handle_Invoke(NULL, handle, &data);
	}

	return true;
}

AX_API AX_INLINE
VoidPtr
AX_Module_Input_GetModuleState
(void)
{
	return state;
}

AX_API 
Bool
AX_Module_Input_SetModuleState
(VoidPtr newState)
{
	if (!state || !(state->isInitialized))
		return false;

	return (AX_HAL_Memory_Memcpy(state, newState, sizeof(AX_Input_Module_State)) != NULL);
}



