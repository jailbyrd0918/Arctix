#pragma once

#include <time.h>


// -- wall clock -- //

typedef struct tm	SDateTime;
typedef time_t		TimeValue;

typedef
enum AX_Clock_MonthOfYear
{
	AX_MONTH_JANUARY = 1,
	AX_MONTH_FEBRUARY,
	AX_MONTH_MARCH,
	AX_MONTH_APRIL,
	AX_MONTH_MAY,
	AX_MONTH_JUNE,
	AX_MONTH_JULY,
	AX_MONTH_AUGUST,
	AX_MONTH_SEPTEMBER,
	AX_MONTH_OCTOBER,
	AX_MONTH_NOVEMBER,
	AX_MONTH_DECEMBER
}
EMonthOfYear;

typedef
enum AX_Clock_DayOfWeek
{
	AX_DAY_MONDAY = 0,
	AX_DAY_TUESDAY,
	AX_DAY_WEDNESDAY,
	AX_DAY_THURSDAY,
	AX_DAY_FRIDAY,
	AX_DAY_SATURDAY,
	AX_DAY_SUNDAY
}
EDayOfWeek;

typedef
struct AX_Clock_WallClock
{
	SDateTime	timeInfo;
	TimeValue	timeValue;
}
SWallClock;

typedef
struct AX_Clock_Date
{
	Int32		year;
	Int32		month;
	Int32		day;
}
SDate;


// -- high resolution clock -- //

typedef	UInt64		HiResRep;

typedef HiResRep	HiResDuration;
typedef HiResRep	HiResTimePoint;

typedef
struct AX_Clock_High_Resolution_Clock
{
	HiResRep	counter;
	HiResTimePoint	timePoint;
}
SHiResClock;



