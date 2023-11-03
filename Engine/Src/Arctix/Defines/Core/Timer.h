#pragma once

#include "Arctix/Defines/Core/DataTypes.h"
#include "Arctix/Defines/Core/Clock.h"


typedef
enum AX_Timer_Status
{
	AX_TIMER_STATUS_INACTIVE,
	AX_TIMER_STATUS_RUNNING,
	AX_TIMER_STATUS_PAUSED,

	AX_TIMER_STATUS_TOTAL
}
ETimerStatus;

typedef
struct AX_Timer
{
	HiResTimePoint
		startTimePoint,
		pausedTimePoint;

	ETimerStatus
		status;
}
STimer;



