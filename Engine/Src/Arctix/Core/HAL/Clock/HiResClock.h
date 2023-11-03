#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/DataTypes.h"
#include "Arctix/Defines/Core/Clock.h"


AX_API AX_INLINE
SHiResClock
AX_HAL_HiResClock_Now
(void);

AX_API AX_INLINE
HiResDuration
AX_HAL_HiResClock_Duration_Raw
(SHiResClock *start, SHiResClock *end);

AX_API AX_INLINE
Float64
AX_HAL_HiResClock_Duration_Seconds
(SHiResClock *start, SHiResClock *end);

AX_API AX_INLINE
Float64
AX_HAL_HiResClock_Duration_Milliseconds
(SHiResClock *start, SHiResClock *end);

AX_API AX_INLINE
Float64
AX_HAL_HiResClock_Duration_Nanoseconds
(SHiResClock *start, SHiResClock *end);

