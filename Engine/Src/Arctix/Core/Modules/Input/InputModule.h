#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Delegates.h"
#include "Arctix/Defines/Core/Input.h"


AX_API AX_INLINE
ByteSize
AX_Module_Input_GetModuleSize
(void);

AX_API
Bool
AX_Module_Input_Startup
(VoidPtr module);

AX_API
Bool
AX_Module_Input_Shutdown
(void);

AX_API
Bool
AX_Module_Input_Update
(void);

AX_API
Bool
AX_Module_Input_RegisterEvent
(EEventCode eventCode, FPDelegate eventCallback);

AX_API
Bool
AX_Module_Input_UnregisterEvent
(EEventCode eventCode, FPDelegate eventCallback);

AX_API
Bool
AX_Module_Input_InvokeEvent
(EEventCode eventCode, VoidPtr data);

Bool
AX_Module_Input_ProcessKey
(const EKeyCode key, const Bool pressed);

Bool
AX_Module_Input_ProcessMouse_Code
(const EMouseCode code, const Bool pressed);

Bool
AX_Module_Input_ProcessMouse_Move
(const Int32 x, const Int32 y);

Bool
AX_Module_Input_ProcessMouse_Scroll
(const Int8 value);

AX_API AX_INLINE
VoidPtr
AX_Module_Input_GetModuleState
(void);

AX_API
Bool
AX_Module_Input_SetModuleState
(VoidPtr newState);


