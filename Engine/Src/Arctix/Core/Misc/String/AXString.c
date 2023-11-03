#include "Arctix/Core/Misc/String/AXString.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>


AX_API AX_INLINE
RawString
AX_Misc_String_Strcat
(RawString dest, ReadOnlyString src)
{
	return strcat(dest, src);
}

AX_API AX_INLINE
RawString
AX_Misc_String_Strncat
(RawString dest, ReadOnlyString src, ByteSize count)
{
	return strncat(dest, src, count);
}

AX_API AX_INLINE
RawString
AX_Misc_String_Strchr
(RawString str, Char ch)
{
	return strchr(str, ch);
}

AX_API AX_INLINE
RawString
AX_Misc_String_Strrchr
(RawString str, Char ch)
{
	return strrchr(str, ch);
}

AX_API AX_INLINE
Int32
AX_Misc_String_Strcmp
(ReadOnlyString left, ReadOnlyString right, Bool caseSensitive)
{
	#if	defined(_MSC_VER)
	return (caseSensitive ? strcmp(left, right) : strcmpi(left, right));

	#else
	return (caseSensitive ? strcmp(left, right) : strcasecmp(left, right));

	#endif
}

AX_API AX_INLINE
Int32
AX_Misc_String_Strncmp
(ReadOnlyString left, ReadOnlyString right, ByteSize count, Bool caseSensitive)
{
	#if	defined(_MSC_VER)
	return (caseSensitive ? strncmp(left, right, count) : strnicmp(left, right, count));

	#else
	return (caseSensitive ? (strncmp(left, right, count) : (strncasecmp(left, right, count));

	#endif
}

AX_API AX_INLINE
RawString
AX_Misc_String_Strcpy
(RawString dest, ReadOnlyString src)
{
	return strcpy(dest, src);
}

AX_API AX_INLINE
RawString
AX_Misc_String_Strncpy
(RawString dest, ReadOnlyString src, ByteSize count)
{
	return strncpy(dest, src, count);
}

AX_API AX_INLINE
ByteSize
AX_Misc_String_Strspn
(RawString str, ReadOnlyString mask)
{
	return strspn(str, mask);
}

AX_API AX_INLINE
ByteSize
AX_Misc_String_Strcspn
(RawString str, ReadOnlyString mask)
{
	return strcspn(str, mask);
}

AX_API AX_INLINE
RawString
AX_Misc_String_Strdup
(ReadOnlyString str)
{
	return strdup(str);
}

AX_API AX_INLINE
ByteSize
AX_Misc_String_Strlen
(ReadOnlyString str)
{
	return strlen(str);
}

AX_API AX_INLINE
RawString
AX_Misc_String_Strerr
(Int32 errorNum)
{
	return strerror(errorNum);
}

AX_API AX_INLINE
RawString
AX_Misc_String_Strpbrk
(RawString str, ReadOnlyString mask)
{
	return strpbrk(str, mask);
}

AX_API AX_INLINE
RawString
AX_Misc_String_Strtok
(RawString str, ReadOnlyString delim)
{
	return strtok(str, delim);
}

AX_API AX_INLINE
ByteSize
AX_Misc_String_Strxfrm
(RawString dest, ReadOnlyString src, ByteSize count)
{
	return strxfrm(dest, src, count);
}

AX_API AX_INLINE
Float64
AX_Misc_String_Atof
(ReadOnlyString str)
{
	return atof(str);
}

AX_API AX_INLINE
Int32
AX_Misc_String_Atoi
(ReadOnlyString str)
{
	return atoi(str);
}

AX_API AX_INLINE
Int64
AX_Misc_String_Atol
(ReadOnlyString str)
{
	return atol(str);
}

AX_API AX_INLINE
Int64
AX_Misc_String_Strtol
(ReadOnlyString str, RawString *end, Int32 base)
{
	return strtol(str, end, base);
}

AX_API AX_INLINE
UInt64
AX_Misc_String_Strtoul
(ReadOnlyString str, RawString *end, Int32 base)
{
	return strtoul(str, end, base);
}

AX_API AX_INLINE
Float
AX_Misc_String_Strtof
(ReadOnlyString str, RawString *end)
{
	return strtof(str, end);
}

AX_API
RawString
AX_Misc_String_Strstr
(RawString str, ReadOnlyString substr, Bool caseSensitive)
{
	if (!str || !substr)
		return NULL;

	if (caseSensitive) {
		return strstr(str, substr);
	}
	else {
		RawString
			strPtr = str,
			substrPtr = AX_Misc_String_Strdup(substr);

		while (*strPtr) {
			RawString
				strCurr = strPtr,
				substrCurr = substrPtr;

			while (
				(*strCurr && *substrCurr) &&
				(tolower(*strCurr) == tolower(*substrCurr))
				) {
				++strCurr;
				++substrCurr;
			}

			if (*substrCurr == '\0') {
				AX_HAL_Memory_Free(substrPtr);
				return strPtr;
			}

			++strPtr;
		}

		AX_HAL_Memory_Free(substrPtr);
		return NULL;
	}
}

AX_API
RawString
AX_Misc_String_Strfind
(RawString str, ReadOnlyString substr, Bool caseSensitive)
{
	if (!str || !substr)
		return NULL;

	RawString
		strPtr = str,
		substrPtr = AX_Misc_String_Strdup(substr);

	while (*strPtr) {
		RawString
			strCurr = strPtr,
			substrCurr = substrPtr;

		if (caseSensitive) {
			while (
				(*strCurr && *substrCurr) &&
				(*strCurr == *substrCurr)
				) {
				++strCurr;
				++substrCurr;
			}
		}
		else {
			while (
				(*strCurr && *substrCurr) &&
				(tolower(*strCurr) == tolower(*substrCurr))
				) {
				++strCurr;
				++substrCurr;
			}
		}

		if (*substrCurr == '\0') {
			AX_HAL_Memory_Free(substrPtr);
			return strPtr;
		}

		++strPtr;
	}

	AX_HAL_Memory_Free(substrPtr);
	return NULL;
}

AX_API
Int32
AX_Misc_String_VaArgs
(RawString dest, ByteSize destSize, ReadOnlyString format, VaList args)
{
	if (!dest || !format)
		return -1;

	Int32 res = vsnprintf(dest, destSize, format, args);
	if (res <= 0) {
		dest = '\0';
		return -1;
	}

	return res;
}

AX_API
Int32
AX_Misc_String_Format
(RawString dest, ByteSize destSize, ReadOnlyString format, ...)
{
	if (!dest || !format)
		return -1;

	VaList argList;
	va_start(argList, format);

	Int32 res = AX_Misc_String_VaArgs(dest, destSize, format, argList);

	va_end(argList);
	return res;
}

AX_API
Bool
AX_Misc_String_IsNumeric
(ReadOnlyString str)
{
	if (!str || AX_Misc_String_Strlen(str) == 0)
		return false;

	RawString
		strPtr = AX_Misc_String_Strdup(str),
		strPtrCurr = strPtr;

	while (*strPtrCurr != '\0') {
		if ((*strPtrCurr < '0') || (*strPtrCurr > '9')) {
			AX_HAL_Memory_Free(strPtr);
			return false;
		}

		++strPtrCurr;
	}

	AX_HAL_Memory_Free(strPtr);
	return true;
}

AX_API
Bool
AX_Misc_String_IsAlpha
(ReadOnlyString str)
{
	if (!str || AX_Misc_String_Strlen(str) == 0)
		return false;

	RawString 
		strPtr = AX_Misc_String_Strdup(str),
		strPtrCurr = strPtr;

	while (*strPtrCurr != '\0') {
		if (!(*strPtrCurr >= 'A' && *strPtrCurr <= 'Z') && !(*strPtrCurr >= 'a' && *strPtrCurr <= 'z')) {
			AX_HAL_Memory_Free(strPtr);
			return false;
		}

		++strPtrCurr;
	}

	AX_HAL_Memory_Free(strPtr);
	return true;
}


