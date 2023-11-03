#pragma once

#include "Arctix/Defines/Core/Memory.h"
#include "Arctix/Defines/Core/Containers.h"
#include "Arctix/Defines/Core/Filesystem.h"


typedef
enum AX_Logging_LogVerbosity
{
	AX_LOG_VERBOSITY_FATAL,
	AX_LOG_VERBOSITY_ERROR,
	AX_LOG_VERBOSITY_WARNING,
	AX_LOG_VERBOSITY_INFO,
	AX_LOG_VERBOSITY_DEBUG,
	AX_LOG_VERBOSITY_TRACE
}
ELogVerbosity;

typedef
enum AX_Logging_Flag
{
	AX_LOG_FLAG_NONE = AX_BIT_FLAG_FIELD(0),
	AX_LOG_FLAG_TIMESTAMP = AX_BIT_FLAG_FIELD(1),
	AX_LOG_FLAG_FILE_LINE = AX_BIT_FLAG_FIELD(2)
}
ELogFlag;


typedef
struct AX_Logging_Logger
{
	ReadOnlyString	id;
	SFileHandle	fileHandle;
}
SLogger;


