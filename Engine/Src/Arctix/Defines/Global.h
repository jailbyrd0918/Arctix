#pragma once


// -- import/export DLL -- //

#if	defined(AX_EXPORT_DLL)
#	if	defined(_MSC_VER)
#		define	AX_API	__declspec(dllexport)

#	else
#		define	AX_API	__attribute__((visibility("default")))

#	endif

#else
#	if	defined(_MSC_VER)
#		define	AX_API	__declspec(dllimport)

#	else
#		define	AX_API

#	endif

#endif


// -- explicit casting -- //

#define	AX_CAST(type, value)	((type)(value))


// -- struct assignment -- //

#define	AX_STRUCT(type, ...)	((type){__VA_ARGS__})


// -- force-inlining -- //

#if	defined(_MSC_VER)
#	define	AX_INLINE	__forceinline
#	define	AX_NOINLINE	__declspec(noinline)

#elif	defined(__clang__) || defined(__gcc__)
#	define	AX_INLINE	__attribute__((always_inline)) inline
#	define	AX_NOINLINE	__attribute__((noinline))

#else
#	define	AX_INLINE	static inline
#	define	AX_NOINLINE

#endif


// -- disable warnings -- // 

#if	defined(_MSC_VER)

// deprecated functions 
#	pragma	warning	(disable : 4996)
#	define	_CRT_SECURE_NO_WARNINGS

#endif


// -- static array size -- //

#define	AX_STATIC_ARRAY_SIZE(array)	(sizeof(array) / sizeof(*(array)))


// -- invalid ID -- //

#if	defined(UINT32_MAX)
#	define	AX_INVALID_ID		(UINT32_MAX)

#else
#	define	AX_INVALID_ID		(4294967295U)

#endif

