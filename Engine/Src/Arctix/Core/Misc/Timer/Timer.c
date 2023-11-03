#include "Arctix/Core/Misc/Timer/Timer.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/HAL/Clock/HiResClock.h"


AX_API AX_INLINE
STimer
AX_Misc_Timer_New
(void)
{
	return AX_STRUCT(
		STimer,
		.startTimePoint = 0,
		.pausedTimePoint = 0,
		.status = AX_TIMER_STATUS_INACTIVE
	);
}

AX_API
Bool
AX_Misc_Timer_Start
(STimer *timer)
{
	if (!timer)
		return false;

	timer->status = AX_TIMER_STATUS_RUNNING;
	timer->startTimePoint = AX_HAL_HiResClock_Now().timePoint;
	timer->pausedTimePoint = 0;

	return true;
}

AX_API
Bool
AX_Misc_Timer_Stop
(STimer *timer)
{
	if (!timer)
		return false;

	// set all properties to zero
	// set status to AX_TIMER_STATUS_INACTIVE
	AX_HAL_Memory_Memzero(timer, sizeof(STimer));

	return true;
}

AX_API
Bool
AX_Misc_Timer_Pause
(STimer *timer)
{
	if (!timer)
		return false;

	timer->status = AX_TIMER_STATUS_PAUSED;

	// calculate paused time point
	timer->pausedTimePoint = AX_HAL_HiResClock_Now().timePoint - timer->startTimePoint;

	// reset start time point
	timer->startTimePoint = 0;

	return true;
}

AX_API
Bool
AX_Misc_Timer_Resume
(STimer *timer)
{
	if (!timer)
		return false;

	if ((timer->status != AX_TIMER_STATUS_INACTIVE) && (timer->status == AX_TIMER_STATUS_PAUSED)) {
		timer->status = AX_TIMER_STATUS_RUNNING;

		// calculate start time point
		timer->startTimePoint = AX_HAL_HiResClock_Now().timePoint - timer->pausedTimePoint;

		// reset paused time point
		timer->pausedTimePoint = 0;
	}

	return true;
}

AX_API AX_INLINE
HiResTimePoint
AX_Misc_Timer_GetTimePoint
(STimer *timer)
{
	return
		(!timer) ?
		0 :

		(timer->status == AX_TIMER_STATUS_PAUSED) ?
		(timer->pausedTimePoint) :

		(timer->status == AX_TIMER_STATUS_RUNNING) ?
		(AX_HAL_HiResClock_Now().timePoint - timer->startTimePoint) : 0;
}

AX_API AX_INLINE
ETimerStatus
AX_Misc_Timer_GetStatus
(STimer *timer)
{
	return (!timer) ? AX_TIMER_STATUS_INACTIVE : (timer->status);
}

AX_API AX_INLINE
Bool
AX_Misc_Timer_IsPaused
(STimer *timer)
{
	return (!timer) ? false : (timer->status == AX_TIMER_STATUS_PAUSED);
}

AX_API AX_INLINE
Bool
AX_Misc_Timer_IsRunning
(STimer *timer)
{
	return (!timer) ? false : (timer->status == AX_TIMER_STATUS_RUNNING);
}

AX_API AX_INLINE
Bool
AX_Misc_Timer_IsInactive
(STimer *timer)
{
	return (!timer) ? false : (timer->status == AX_TIMER_STATUS_INACTIVE);
}


