#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/DataTypes.h"
#include "Arctix/Defines/Core/Clock.h"


AX_API
SWallClock
AX_HAL_WallClock_Now
(void);

AX_API
SWallClock
AX_HAL_WallClock_UtcNow
(void);

AX_API AX_INLINE
SWallClock
AX_HAL_WallClock_Today
(void);

AX_API
SWallClock
AX_HAL_WallClock_FromJulian
(Float64 julianDay);

AX_API
SWallClock
AX_HAL_WallClock_FromUnix
(Int64 unixTime);

AX_API
SWallClock
AX_HAL_WallClock_Parse
(ReadOnlyString dateTimeString);

AX_API
SDate
AX_HAL_WallClock_GetDate
(SWallClock *clock);

AX_API
Bool
AX_HAL_WallClock_GetDateYMD
(SWallClock *clock, Int32 *year, Int32 *month, Int32 *day);

AX_API AX_INLINE
Int32
AX_HAL_WallClock_GetYear
(SWallClock *clock);

AX_API AX_INLINE
Int32
AX_HAL_WallClock_GetMonth
(SWallClock *clock);

AX_API AX_INLINE
EMonthOfYear
AX_HAL_WallClock_GetMonthOfYear
(SWallClock *clock);

AX_API AX_INLINE
Int32
AX_HAL_WallClock_GetDay
(SWallClock *clock);

AX_API AX_INLINE
EDayOfWeek
AX_HAL_WallClock_GetDayOfWeek
(SWallClock *clock);

AX_API AX_INLINE
Int32
AX_HAL_WallClock_GetDayOfYear
(SWallClock *clock);

AX_API AX_INLINE
Float64
AX_HAL_WallClock_GetJulianDay
(SWallClock *clock);

AX_API AX_INLINE
Int32
AX_HAL_WallClock_GetHour12
(SWallClock *clock);

AX_API AX_INLINE
Int32
AX_HAL_WallClock_GetHour24
(SWallClock *clock);

AX_API AX_INLINE
Int32
AX_HAL_WallClock_GetMinute
(SWallClock *clock);

AX_API AX_INLINE
Int32
AX_HAL_WallClock_GetSecond
(SWallClock *clock);

AX_API AX_INLINE
Bool
AX_HAL_WallClock_IsMorning
(SWallClock *clock);

AX_API AX_INLINE
Bool
AX_HAL_WallClock_IsAfternoon
(SWallClock *clock);

AX_API
Bool
AX_HAL_WallClock_IsLeapYear
(SWallClock *clock);

AX_API AX_INLINE
RawString
AX_HAL_WallClock_ToString
(SWallClock *clock);


