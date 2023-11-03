#pragma once

#include "Arctix/Core/Logging/Logging.h"


#if	!defined(AX_DEBUG_BREAK)

#	if	defined(_MSC_VER)
#		include <intrin.h>
#		define	AX_DEBUG_BREAK	__debugbreak()

#	elif	defined(__clang__) || defined(__gcc__)
#		define	AX_DEBUG_BREAK	__builtin_trap()

#	endif

#endif

#if	defined(AX_ENABLE_ASSERTIONS)

#	define	AX_ASSERT(condition)											\
		{													\
			if (!(condition)) {										\
				AX_LOG_FATAL("Invalid Condition '%s'", #condition);					\
				AX_DEBUG_BREAK;										\
			}												\
		}

#	define	AX_ASSERT_MESSAGE(condition, message)									\
		{													\
			if (!(condition)) {										\
				AX_LOG_FATAL("Invalid Condition '%s', Message: %s", #condition, message);		\
				AX_DEBUG_BREAK;										\
			}												\
		}

#else	

#	define	AX_ASSERT(condition)			condition
#	define	AX_ASSERT_MESSAGE(condition, message)	condition


#endif


