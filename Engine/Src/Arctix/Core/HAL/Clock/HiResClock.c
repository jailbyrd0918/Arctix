#include "Arctix/Core/HAL/Clock/HiResClock.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/HAL/Clock/WallClock.h"
#include "Arctix/Core/Misc/String/AXString.h"

#include <stdio.h>
#include <stdarg.h>
#include <SDL2/SDL.h>


AX_API AX_INLINE
SHiResClock
AX_HAL_HiResClock_Now
(void)
{
	return AX_STRUCT(
		SHiResClock,
		.counter = SDL_GetPerformanceCounter(),
		.timePoint = SDL_GetPerformanceCounter()
	);
}

AX_API AX_INLINE
HiResDuration
AX_HAL_HiResClock_Duration_Raw
(SHiResClock *start, SHiResClock *end)
{
	return (!start || !end) ? 0 : AX_CAST(HiResDuration, end->timePoint - start->timePoint);
}

AX_API AX_INLINE
Float64
AX_HAL_HiResClock_Duration_Seconds
(SHiResClock *start, SHiResClock *end)
{
	#define DURATION_IN_SECS(duration)	(AX_CAST(Float64, duration) / SDL_GetPerformanceFrequency())
	return DURATION_IN_SECS(AX_HAL_HiResClock_Duration_Raw(start, end));
}

AX_API AX_INLINE
Float64
AX_HAL_HiResClock_Duration_Milliseconds
(SHiResClock *start, SHiResClock *end)
{
	#define SECS_TO_MILLISECS(sec)		(sec * 1000)
	return  SECS_TO_MILLISECS(AX_HAL_HiResClock_Duration_Seconds(start, end));
}

AX_API AX_INLINE
Float64
AX_HAL_HiResClock_Duration_Nanoseconds
(SHiResClock *start, SHiResClock *end)
{
	#define MILLISECS_TO_NANOSECS(ms)	(ms * 1'000'000)
	return  MILLISECS_TO_NANOSECS(AX_HAL_HiResClock_Duration_Milliseconds(start, end));
}



