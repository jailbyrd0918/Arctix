#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/DataTypes.h"
#include "Arctix/Defines/Core/Logging.h"

#include "Arctix/Core/Platform/Filesystem/Filesystem.h"


ByteSize
AX_Logging_GetModuleSize
(void);

Bool
AX_Logging_Startup
(VoidPtr module);

Bool
AX_Logging_Shutdown
(void);

AX_API
Bool
AX_Logging_AddLogger
(RawString loggerID, ReadOnlyString filepath);

AX_API
Bool
AX_Logging_RemoveLogger
(RawString loggerID);

AX_API
Bool
AX_Logging_ToggleFlag
(ELogFlag logFlag);

AX_API
Bool
AX_Logging_Print
(RawString loggerID, ELogVerbosity verbosity, ReadOnlyString atFile, UInt32 atLine, ReadOnlyString message, ...);


#define	AX_LOG_FATAL(logID, message, ...)	\
	AX_Logging_Print(logID, AX_LOG_VERBOSITY_FATAL, AX_Filesystem_ExtractFilename(__FILE__), __LINE__, message, __VA_ARGS__)

#define	AX_LOG_ERROR(logID, message, ...)	\
	AX_Logging_Print(logID, AX_LOG_VERBOSITY_ERROR, AX_Filesystem_ExtractFilename(__FILE__), __LINE__, message, __VA_ARGS__)

#define	AX_LOG_WARN(logID, message, ...)	\
	AX_Logging_Print(logID, AX_LOG_VERBOSITY_WARNING, AX_Filesystem_ExtractFilename(__FILE__), __LINE__, message, __VA_ARGS__)

#define	AX_LOG_INFO(logID, message, ...)	\
	AX_Logging_Print(logID, AX_LOG_VERBOSITY_INFO, AX_Filesystem_ExtractFilename(__FILE__), __LINE__, message, __VA_ARGS__)

#if	defined(AX_CONFIG_DEBUG)
#	define	AX_LOG_DEBUG(logID, message, ...)	\
		AX_Logging_Print(logID, AX_LOG_VERBOSITY_DEBUG, AX_Filesystem_ExtractFilename(__FILE__), __LINE__, message, __VA_ARGS__)

#	define	AX_LOG_TRACE(logID, message, ...)	\
		AX_Logging_Print(logID, AX_LOG_VERBOSITY_TRACE, AX_Filesystem_ExtractFilename(__FILE__), __LINE__, message, __VA_ARGS__)

#else
#	define	AX_LOG_DEBUG(logID, message, ...)
#	define	AX_LOG_TRACE(logID, message, ...)

#endif




