#include "Arctix/Core/Logging/Logging.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/HAL/Clock/WallClock.h"
#include "Arctix/Core/Misc/String/AXString.h"
#include "Arctix/Core/Misc/Console/Console.h"

#include "Arctix/Core/Containers/Map/Map.h"

#include <stdio.h>
#include <stdarg.h>


#define	OUTPUT_BUFFER_MAX_SIZE		4'096

typedef
struct AX_Logging_Module_State
{
	SMap		loggerMap;
	ELogFlag	logFlags;

	Bool		isInitialized;
}
AX_Logging_Module_State;

static AX_Logging_Module_State *	state;


Bool
_AX_Logging_Append_To_Log_File
(SFileHandle *handle, RawString message)
{
	if ((!handle) || !(handle->stream))
		return false;

	ByteSize msgLength = AX_Misc_String_Strlen(message);
	if (!AX_Filesystem_WriteDataToFile(handle, message, msgLength))
		return false;

	return true;
}


ByteSize
AX_Logging_GetModuleSize
(void)
{
	return sizeof(AX_Logging_Module_State);
}

Bool
AX_Logging_Startup
(VoidPtr module)
{
	if (!module)
		return false;

	state = AX_CAST(AX_Logging_Module_State *, module);

	if (state->isInitialized)
		return false;

	// init state properties
	{
		// set up logger map
		state->loggerMap = AX_Map_Construct(SLogger);
		if (!state->loggerMap) {
			AX_Map_Destruct(state->loggerMap);
			return false;
		}

		// have no flags checked
		state->logFlags = AX_LOG_FLAG_NONE;

		// set initialized
		state->isInitialized = true;
	}

	// add engine logger
	if (!AX_Logging_AddLogger("Engine", "Log/EngineLog.txt"))
		return false;

	return state->isInitialized;
}

Bool
AX_Logging_Shutdown
(void)
{
	if (!state || !state->isInitialized)
		return false;

	if (!state->loggerMap)
		return false;

	AX_Map_Destruct(state->loggerMap);
	state->isInitialized = false;

	return true;
}

AX_API
Bool
AX_Logging_AddLogger
(RawString loggerID, ReadOnlyString filepath)
{
	if (!state || !state->isInitialized || !state->loggerMap)
		return false;

	SLogger newLogger = {
		.id = loggerID,
		.fileHandle = NULL
	};

	if (!AX_Filesystem_OpenFile(filepath, AX_FILE_IO_MODE_WRITE, AX_FILE_MODE_TEXT, &(newLogger.fileHandle))) {
		// in case the directory does not exist
		{
			// create it ...

			ReadOnlyString dirSep =
				AX_Misc_String_Strrchr(AX_CAST(RawString, filepath), '\\') ?
				AX_Misc_String_Strrchr(AX_CAST(RawString, filepath), '\\') :

				AX_Misc_String_Strrchr(AX_CAST(RawString, filepath), '/') ?
				AX_Misc_String_Strrchr(AX_CAST(RawString, filepath), '/') : filepath;

			if (!dirSep)
				return false;

			ByteSize index = AX_CAST(PtrDiff, dirSep - filepath);

			Char dirName[OUTPUT_BUFFER_MAX_SIZE] = { '\0' };
			AX_Misc_String_Strncpy(dirName, filepath, index);
			dirName[index] = '\0';

			if (!AX_Filesystem_CreateDirectory(dirName))
				return false;
		}

		// try again - create new file in the newly created directory
		if (!AX_Filesystem_OpenFile(filepath, AX_FILE_IO_MODE_WRITE, AX_FILE_MODE_TEXT, &(newLogger.fileHandle)))
			return false;
	}

	return AX_Map_Insert(state->loggerMap, loggerID, newLogger);
}

AX_API
Bool
AX_Logging_RemoveLogger
(RawString loggerID)
{
	if (!state || !state->isInitialized)
		return false;

	if (AX_Map_IsEmpty(state->loggerMap))
		return false;

	return AX_Map_Remove(state->loggerMap, loggerID);
}

AX_API
Bool
AX_Logging_ToggleFlag
(ELogFlag logFlag)
{
	if (!state || !state->isInitialized)
		return false;

	AX_BIT_FLAG_TOGGLE(state->logFlags, logFlag);
	return true;
}

AX_API
Bool
AX_Logging_Print
(RawString loggerID, ELogVerbosity verbosity, ReadOnlyString atFile, UInt32 atLine, ReadOnlyString message, ...)
{
	if (!state || !state->isInitialized)
		return false;

	if (AX_Map_IsEmpty(state->loggerMap))
		return false;

	if (!AX_Map_Contains(state->loggerMap, loggerID))
		return false;

	SLogger logger = AX_Map_GetValue(state->loggerMap, SLogger, loggerID);

	ReadOnlyString msgVerbosity;
	switch (verbosity) {
		case AX_LOG_VERBOSITY_FATAL:	msgVerbosity = "FATAL";		break;
		case AX_LOG_VERBOSITY_ERROR:	msgVerbosity = "ERROR";		break;
		case AX_LOG_VERBOSITY_WARNING:	msgVerbosity = "WARNING";	break;
		case AX_LOG_VERBOSITY_INFO:	msgVerbosity = "INFO";		break;
		case AX_LOG_VERBOSITY_DEBUG:	msgVerbosity = "DEBUG";		break;
		case AX_LOG_VERBOSITY_TRACE:	msgVerbosity = "TRACE";		break;
		default:							break;
	}

	Char
		msgContent[OUTPUT_BUFFER_MAX_SIZE],
		msgTime[OUTPUT_BUFFER_MAX_SIZE],
		msgToFile[OUTPUT_BUFFER_MAX_SIZE];

	// construct message format
	{
		VaList argList;
		va_start(argList, message);
		AX_Misc_String_VaArgs(msgContent, OUTPUT_BUFFER_MAX_SIZE, message, argList);
		va_end(argList);
	}

	// format message time
	{
		SWallClock clock = AX_HAL_WallClock_Now();
		strftime(msgTime, OUTPUT_BUFFER_MAX_SIZE, "%F %T", &(clock.timeInfo));
	}

	// print logger ID
	{
		AX_Misc_Console_SetForegroundColorRGB(102, 255, 51);
		AX_Misc_Console_SetBackgroundColorRGB(0, 0, 153);
		AX_Misc_Console_Write(" %s ", loggerID);
		AX_Misc_Console_Reset();
	}

	// print verbosity
	switch (verbosity) {
		case AX_LOG_VERBOSITY_FATAL:
		{
			AX_Misc_Console_SetForegroundColorRGB(255, 153, 153);
			AX_Misc_Console_SetBackgroundColorRGB(255, 0, 0);
			AX_Misc_Console_Write(" %s ", msgVerbosity);
			AX_Misc_Console_Reset();
		}
		break;

		case AX_LOG_VERBOSITY_ERROR:
		{
			AX_Misc_Console_SetForegroundColorRGB(0, 255, 255);
			AX_Misc_Console_SetBackgroundColorRGB(255, 0, 0);
			AX_Misc_Console_Write(" %s ", msgVerbosity);
			AX_Misc_Console_Reset();
		}
		break;

		case AX_LOG_VERBOSITY_WARNING:
		{
			AX_Misc_Console_SetForegroundColorRGB(0, 64, 255);
			AX_Misc_Console_SetBackgroundColorRGB(229, 229, 16);
			AX_Misc_Console_Write(" %s ", msgVerbosity);
			AX_Misc_Console_Reset();
		}
		break;

		case AX_LOG_VERBOSITY_INFO:
		{
			AX_Misc_Console_SetForegroundColorRGB(0, 255, 255);
			AX_Misc_Console_SetBackgroundColorRGB(0, 153, 255);
			AX_Misc_Console_Write(" %s ", msgVerbosity);
			AX_Misc_Console_Reset();
		}
		break;

		case AX_LOG_VERBOSITY_DEBUG:
		{
			AX_Misc_Console_SetForegroundColorRGB(0, 255, 255);
			AX_Misc_Console_SetBackgroundColorRGB(0, 0, 204);
			AX_Misc_Console_Write(" %s ", msgVerbosity);
			AX_Misc_Console_Reset();
		}
		break;
		
		case AX_LOG_VERBOSITY_TRACE:
		{
			AX_Misc_Console_SetForegroundColorRGB(0, 255, 255);
			AX_Misc_Console_SetBackgroundColorRGB(51, 102, 153);
			AX_Misc_Console_Write(" %s ", msgVerbosity);
			AX_Misc_Console_Reset();
		}
		break;

		default:
			break;
	}

	// print checked log flags
	{
		if (AX_BIT_FLAG_IF_SET(state->logFlags, AX_LOG_FLAG_TIMESTAMP))
			AX_Misc_Console_Write(" [%s]", msgTime);

		if (AX_BIT_FLAG_IF_SET(state->logFlags, AX_LOG_FLAG_FILE_LINE))
			AX_Misc_Console_Write(" (file: %s, line: %d)", atFile, atLine);
	}

	AX_Misc_Console_WriteLine(" %s", msgContent);

	// write to log file under associate logger ID
	{
		AX_Misc_String_Format(
			msgToFile,
			OUTPUT_BUFFER_MAX_SIZE,
			"[%s] (file: %s, line: %d) %s: %s \n",
			msgTime, atFile, atLine, msgVerbosity, msgContent
		);

		return _AX_Logging_Append_To_Log_File(&logger.fileHandle, msgToFile);
	}
}

