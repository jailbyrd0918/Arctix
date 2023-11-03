#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Delegates.h"
#include "Arctix/Defines/Core/Input.h"


// -- keyboard -- //

AX_API AX_INLINE
Bool
AX_HAL_Input_IsKeyDown
(const EKeyCode key);

AX_API AX_INLINE
Bool
AX_HAL_Input_IsKeyUp
(const EKeyCode key);

AX_API AX_INLINE
Bool
AX_HAL_Input_WasKeyDown
(const EKeyCode key);

AX_API AX_INLINE
Bool
AX_HAL_Input_WasKeyUp
(const EKeyCode key);

AX_API AX_INLINE
Bool
AX_HAL_Input_IsKeyPressed
(const EKeyCode key);


// -- mouse -- //

AX_API AX_INLINE
Bool
AX_HAL_Input_IsMouseButtonDown
(const EMouseCode code);

AX_API AX_INLINE
Bool
AX_HAL_Input_IsMouseButtonUp
(const EMouseCode code);

AX_API AX_INLINE
Bool
AX_HAL_Input_WasMouseButtonDown
(const EMouseCode code);

AX_API AX_INLINE
Bool
AX_HAL_Input_WasMouseButtonUp
(const EMouseCode code);

AX_API AX_INLINE
Bool
AX_HAL_Input_IsMousePressed
(const EMouseCode code);

AX_API AX_INLINE
Int32
AX_HAL_Input_GetMouseX
(void);

AX_API AX_INLINE
Int32
AX_HAL_Input_GetMouseY
(void);

AX_API AX_INLINE
Int32
AX_HAL_Input_GetPreviousMouseX
(void);

AX_API AX_INLINE
Int32
AX_HAL_Input_GetPreviousMouseY
(void);

AX_API AX_INLINE
Int8
AX_HAL_Input_GetMouseScrollValue
(void);

AX_API AX_INLINE
Int8
AX_HAL_Input_GetPreviousMouseScrollValue
(void);





