#pragma once

#include <stdint.h>


// -- signed types -- //

typedef		signed char		Int8;
typedef		signed short		Int16;
typedef		signed int		Int32;
typedef		signed long long	Int64;


// -- unsigned types -- //

typedef		unsigned char		UInt8;
typedef		unsigned short		UInt16;
typedef		unsigned int		UInt32;
typedef		unsigned long long	UInt64;


// -- boolean type -- //

#if	defined(bool)
typedef		bool			Bool;

#else
typedef		unsigned char		Bool;
#define		false			AX_CAST(Bool, 0)
#define		true			AX_CAST(Bool, 1)

#endif


// -- floating point types -- //

typedef		float			Float;
typedef		double			Float64;


// -- character-based types -- //

typedef		char			Char;
typedef		char *			RawString;
typedef		const char *		ReadOnlyString;


// -- memory types -- //

typedef		size_t			ByteSize;
typedef		char *			BytePtr;
typedef		void *			VoidPtr;
typedef		intptr_t		IntPtr;
typedef		uintptr_t		UIntPtr;
typedef		ptrdiff_t		PtrDiff;


// -- variable argument list -- //

#if	defined(_MSC_VER)
typedef		va_list			VaList;

#else
typedef		__builtin_va_list	VaList;

#endif
