#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/DataTypes.h"
#include "Arctix/Defines/Core/Timer.h"


AX_API AX_INLINE
STimer
AX_Misc_Timer_New
(void);

AX_API
Bool
AX_Misc_Timer_Start
(STimer *timer);

AX_API
Bool
AX_Misc_Timer_Stop
(STimer *timer);

AX_API
Bool
AX_Misc_Timer_Pause
(STimer *timer);

AX_API
Bool
AX_Misc_Timer_Resume
(STimer *timer);

AX_API AX_INLINE
HiResTimePoint
AX_Misc_Timer_GetTimePoint
(STimer *timer);

AX_API AX_INLINE
ETimerStatus
AX_Misc_Timer_GetStatus
(STimer *timer);

AX_API AX_INLINE
Bool
AX_Misc_Timer_IsPaused
(STimer *timer);

AX_API AX_INLINE
Bool
AX_Misc_Timer_IsRunning
(STimer *timer);

AX_API AX_INLINE
Bool
AX_Misc_Timer_IsInactive
(STimer *timer);





