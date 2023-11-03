#include "Arctix/Core/HAL/Clock/WallClock.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Misc/String/AXString.h"

#include <stdio.h>
#include <stdarg.h>


AX_API
SWallClock
AX_HAL_WallClock_Now
(void)
{
	SWallClock clock;
	clock.timeValue = time(NULL);
	clock.timeInfo = *(localtime(&(clock.timeValue)));

	return clock;
}

AX_API
SWallClock
AX_HAL_WallClock_UtcNow
(void)
{
	SWallClock clock;
	clock.timeValue = time(NULL);
	clock.timeInfo = *(gmtime(&(clock.timeValue)));

	return clock;
}

AX_API AX_INLINE
SWallClock
AX_HAL_WallClock_Today
(void)
{
	SWallClock clock;
	clock.timeValue = time(NULL);
	clock.timeInfo = *(gmtime(&(clock.timeValue)));
	
	// set time to midnight
	{
		clock.timeInfo.tm_hour = 0;
		clock.timeInfo.tm_min = 0;
		clock.timeInfo.tm_sec = 0;
	}

	return clock;
}

AX_API
SWallClock
AX_HAL_WallClock_FromJulian
(Float64 julianDay)
{
	SWallClock clock;
	clock.timeValue = AX_CAST(TimeValue, (julianDay - 2440587.5) * 86400);
	clock.timeInfo = *(gmtime(&(clock.timeValue)));

	return clock;
}

AX_API
SWallClock
AX_HAL_WallClock_FromUnix
(Int64 unixTime)
{
	SWallClock clock;
	clock.timeValue = unixTime;
	clock.timeInfo = *(gmtime(&(clock.timeValue)));

	return clock;
}

AX_API
SWallClock
AX_HAL_WallClock_Parse
(ReadOnlyString dateTimeString)
{
	SWallClock clock = AX_STRUCT(SWallClock, 0);
	SDateTime tmInfo = AX_STRUCT(SDateTime, 0);

	Char dateBuf[26] = { '\0' };
	Char timeBuf[26] = { '\0' };

	// process date part of datetime
	{
		AX_Misc_String_Strcpy(dateBuf, dateTimeString);
		AX_Misc_String_Strcpy(dateBuf, AX_Misc_String_Strtok(dateBuf, " "));

		Int32 year, month, day;
		if (!sscanf(dateBuf, "%d-%d-%d", &year, &month, &day))
			return clock;

		tmInfo.tm_year = year - 1900;
		tmInfo.tm_mon = month - 1;
		tmInfo.tm_mday = day;
	}

	// process time part of datetime
	{
		AX_Misc_String_Strcpy(timeBuf, dateBuf);
		AX_Misc_String_Strcpy(timeBuf, AX_Misc_String_Strtok(NULL, " "));

		Int32 hour, minute, second;
		if (!sscanf(timeBuf, "%d:%d:%d", &hour, &minute, &second))
			return clock;

		tmInfo.tm_hour = hour;
		tmInfo.tm_min = minute;
		tmInfo.tm_sec = second;
	}

	clock.timeValue = mktime(&tmInfo);
	clock.timeInfo = *(localtime(&(clock.timeValue)));
	return clock;
}

AX_API
SDate
AX_HAL_WallClock_GetDate
(SWallClock *clock)
{
	if (!clock)
		return AX_STRUCT(SDate, -1);

	SDate date = {
		.year = clock->timeInfo.tm_year,
		.month = clock->timeInfo.tm_mon,
		.year = clock->timeInfo.tm_mday
	};

	return date;
}

AX_API
Bool
AX_HAL_WallClock_GetDateYMD
(SWallClock *clock, Int32 *year, Int32 *month, Int32 *day)
{
	if (!clock)
		return false;

	if (year != NULL)
		*year = clock->timeInfo.tm_year + 1900;

	if (month != NULL)
		*month = clock->timeInfo.tm_mon + 1;

	if (day != NULL)
		*day = clock->timeInfo.tm_mday;

	return true;
}

AX_API AX_INLINE
Int32
AX_HAL_WallClock_GetYear
(SWallClock *clock)
{
	return (!clock) ? -1 : (clock->timeInfo.tm_year + 1900);
}

AX_API AX_INLINE
Int32
AX_HAL_WallClock_GetMonth
(SWallClock *clock)
{
	return (!clock) ? -1 : (clock->timeInfo.tm_mon + 1);
}

AX_API AX_INLINE
EMonthOfYear
AX_HAL_WallClock_GetMonthOfYear
(SWallClock *clock)
{
	return (!clock) ? -1 : AX_CAST(EMonthOfYear, clock->timeInfo.tm_mon + 1);
}

AX_API AX_INLINE
Int32
AX_HAL_WallClock_GetDay
(SWallClock *clock)
{
	return (!clock) ? -1 : (clock->timeInfo.tm_mday);
}

AX_API AX_INLINE
EDayOfWeek
AX_HAL_WallClock_GetDayOfWeek
(SWallClock *clock)
{
	return (!clock) ? -1 : AX_CAST(EDayOfWeek, clock->timeInfo.tm_wday);
}

AX_API AX_INLINE
Int32
AX_HAL_WallClock_GetDayOfYear
(SWallClock *clock)
{
	return (!clock) ? -1 : (clock->timeInfo.tm_yday + 1);
}

AX_API AX_INLINE
Float64
AX_HAL_WallClock_GetJulianDay
(SWallClock *clock)
{
	return (!clock) ?
		-1.0 :
		AX_CAST(Float64, clock->timeValue / 86400) + 2440587.5;
}

AX_API AX_INLINE
Int32
AX_HAL_WallClock_GetHour12
(SWallClock *clock)
{
	return
		(!clock) ? -1 :
		(clock->timeInfo.tm_hour > 12) ? (clock->timeInfo.tm_hour) % 12 :
		(clock->timeInfo.tm_hour == 0) ? 12 :
		(clock->timeInfo.tm_hour);
}

AX_API AX_INLINE
Int32
AX_HAL_WallClock_GetHour24
(SWallClock *clock)
{
	return (!clock) ? -1 : (clock->timeInfo.tm_hour);
}

AX_API AX_INLINE
Int32
AX_HAL_WallClock_GetMinute
(SWallClock *clock)
{
	return (!clock) ? -1 : (clock->timeInfo.tm_min);
}

AX_API AX_INLINE
Int32
AX_HAL_WallClock_GetSecond
(SWallClock *clock)
{
	return (!clock) ? -1 : (clock->timeInfo.tm_sec);
}

AX_API AX_INLINE
Bool
AX_HAL_WallClock_IsMorning
(SWallClock *clock)
{
	return (clock != NULL) && ((clock->timeInfo.tm_hour >= 0) && (clock->timeInfo.tm_hour < 12));
}

AX_API AX_INLINE
Bool
AX_HAL_WallClock_IsAfternoon
(SWallClock *clock)
{
	return (clock != NULL) && ((clock->timeInfo.tm_hour >= 12) && (clock->timeInfo.tm_hour < 18));
}

AX_API
Bool
AX_HAL_WallClock_IsLeapYear
(SWallClock *clock)
{
	if (!clock)
		return false;

	Int32 year = AX_HAL_WallClock_GetYear(clock);
	return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}

AX_API AX_INLINE
RawString
AX_HAL_WallClock_ToString
(SWallClock *clock)
{
	return (!clock) ? NULL : asctime(&(clock->timeInfo));
}
