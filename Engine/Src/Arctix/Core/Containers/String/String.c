#include "Arctix/Core/Containers/String/String.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"
#include "Arctix/Core/Misc/String/AXString.h"

#include <stdarg.h>

#define		AX_STATIC_CHAR_ARRAY_MAX_LENGTH		2'056


AX_INLINE
Bool
_AX_String_Resize
(SString string)
{
	UInt32 newCapacity = string->capacity;

	while (newCapacity <= string->length)
		newCapacity *= AX_CONTAINER_RESIZE_FACTOR;

	VoidPtr newBlock = AX_HAL_Memory_Realloc(string->rawString, AX_CAST(ByteSize, newCapacity));
	if (!newBlock) {
		AX_Module_Memory_Deallocate(
			newBlock,
			newCapacity,
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);
		return false;
	}

	AX_Misc_String_Strcpy(newBlock, string->rawString);

	string->rawString = newBlock;
	string->capacity = newCapacity;
	return true;
}


AX_API
SString
AX_String_Construct
(ReadOnlyString value)
{
	SString newString = AX_Module_Memory_Allocate(
		sizeof(AX_Container_String),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
	);

	if (!newString) {
		AX_Module_Memory_Deallocate(
			newString,
			sizeof(AX_Container_String),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		return NULL;
	}

	// init properties
	{
		newString->capacity = AX_CONTAINER_DEFAULT_CAPACITY;
		newString->rawString = AX_Misc_String_Strdup(value);
		newString->length = AX_CAST(UInt32, AX_Misc_String_Strlen(value));

		if (newString->length >= AX_CONTAINER_DEFAULT_CAPACITY)
			_AX_String_Resize(newString);
	}

	return newString;
}

AX_API
SString
AX_String_ConstructChar
(Char ch, Int32 count)
{
	SString newString = AX_Module_Memory_Allocate(
		sizeof(AX_Container_String),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
	);

	if (!newString) {
		AX_Module_Memory_Deallocate(
			newString,
			sizeof(AX_Container_String),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		return NULL;
	}

	RawString value = AX_Module_Memory_Allocate(
		AX_CAST(ByteSize, AX_CAST(ByteSize, count) + 1),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
	);

	AX_HAL_Memory_Memset(value, ch, count);
	value[count] = '\0';

	// init properties
	{
		newString->capacity = AX_CONTAINER_DEFAULT_CAPACITY;
		newString->rawString = AX_Misc_String_Strdup(value);
		newString->length = AX_CAST(UInt32, AX_Misc_String_Strlen(value));

		if (newString->length >= AX_CONTAINER_DEFAULT_CAPACITY)
			_AX_String_Resize(newString);
	}

	AX_Module_Memory_Deallocate(
		value,
		AX_CAST(ByteSize, AX_CAST(ByteSize, count) + 1),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
	);

	return newString;
}

AX_API
SString
AX_String_ConstructFormatted
(ReadOnlyString format, ...)
{
	if (!format)
		return NULL;

	VaList argList;
	va_start(argList, format);

	Char value[AX_STATIC_CHAR_ARRAY_MAX_LENGTH];
	Int32 result = AX_Misc_String_VaArgs(value, AX_STATIC_CHAR_ARRAY_MAX_LENGTH, format, argList);

	va_end(argList);
	return AX_String_Construct(value);
}

AX_API
Bool
AX_String_Destruct
(SString string)
{
	if (!string)
		return false;

	// set properties to zero and deallocate
	{
		AX_HAL_Memory_Free(string->rawString);

		AX_HAL_Memory_Memzero(string, sizeof(AX_Container_String));
	
		AX_Module_Memory_Deallocate(
			string,
			sizeof(AX_Container_String),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);
		
		string = NULL;
	}

	return false;
}

AX_API
Bool
AX_String_Append
(SString string, ReadOnlyString appendStr)
{
	if (!string)
		return false;

	string->length += AX_CAST(UInt32, AX_Misc_String_Strlen(appendStr));

	if (string->length >= string->capacity)
		_AX_String_Resize(string);

	string->rawString = AX_Misc_String_Strcat(string->rawString, appendStr);
	return true;
}

AX_API
Bool
AX_String_AppendChar
(SString string, Char appendCh)
{
	if (!string)
		return false;

	AX_HAL_Memory_Memset(string->rawString + string->length++, appendCh, sizeof(Char));
	AX_HAL_Memory_Memset(string->rawString + string->length, '\0', sizeof(Char));

	if (string->length >= string->capacity)
		_AX_String_Resize(string);

	return true;
}

AX_API
Bool
AX_String_AppendChars
(SString string, ReadOnlyString appendStr, UInt32 count)
{
	if (!string)
		return false;

	string->length += count;

	if (string->length >= string->capacity)
		_AX_String_Resize(string);

	string->rawString = AX_Misc_String_Strncat(string->rawString, appendStr, count);
	return true;
}

AX_API AX_INLINE
Bool
AX_String_Append_Int8
(SString string, Int8 value)
{
	return AX_String_AppendFormatted(string, "%c", value);
}

AX_API AX_INLINE
Bool
AX_String_Append_Int16
(SString string, Int16 value)
{
	return AX_String_AppendFormatted(string, "%hd", value);
}

AX_API AX_INLINE
Bool
AX_String_Append_Int32
(SString string, Int32 value)
{
	return AX_String_AppendFormatted(string, "%d", value);
}

AX_API AX_INLINE
Bool
AX_String_Append_Int64
(SString string, Int64 value)
{
	return AX_String_AppendFormatted(string, "%lld", value);
}

AX_API AX_INLINE
Bool
AX_String_Append_UInt8
(SString string, UInt8 value)
{
	return AX_String_AppendFormatted(string, "%c", value);
}

AX_API AX_INLINE
Bool
AX_String_Append_UInt16
(SString string, Int16 value)
{
	return AX_String_AppendFormatted(string, "%hu", value);
}

AX_API AX_INLINE
Bool
AX_String_Append_UInt32
(SString string, UInt32 value)
{
	return AX_String_AppendFormatted(string, "%u", value);
}

AX_API AX_INLINE
Bool
AX_String_Append_UInt64
(SString string, UInt64 value)
{
	return AX_String_AppendFormatted(string, "%llu", value);
}

AX_API AX_INLINE
Bool
AX_String_Append_Bool
(SString string, Bool value)
{
	return AX_String_Append(string, value ? "true" : "false");
}

AX_API AX_INLINE
Bool
AX_String_Append_BoolNum
(SString string, Bool value)
{
	return AX_String_AppendChar(string, value ? '1' : '0');
}

AX_API AX_INLINE
Bool
AX_String_Append_Float
(SString string, Float value)
{
	return AX_String_AppendFormatted(string, "%f", value);
}

AX_API AX_INLINE
Bool
AX_String_Append_Float64
(SString string, Float64 value)
{
	return AX_String_AppendFormatted(string, "%lf", value);
}

AX_API
Bool
AX_String_AppendFormatted
(SString string, ReadOnlyString format, ...)
{
	if (!string)
		return false;

	VaList argList;
	va_start(argList, format);

	Char appendStr[AX_STATIC_CHAR_ARRAY_MAX_LENGTH];
	Int32 result = AX_Misc_String_VaArgs(appendStr, AX_STATIC_CHAR_ARRAY_MAX_LENGTH, format, argList);

	string->length += AX_CAST(UInt32, AX_Misc_String_Strlen(appendStr));
	if (string->length >= string->capacity)
		_AX_String_Resize(string);

	va_end(argList);
	return (AX_Misc_String_Strcat(string->rawString, appendStr) != NULL);
}

AX_API
Bool
AX_String_FilterChar
(SString string, Char ch)
{
	if (AX_String_IsEmpty(string) || (ch == '\0'))
		return false;

	RawString
		strPtr = string->rawString,
		occurrence;

	Int32 idx = 0;

	while ((occurrence = AX_Misc_String_Strchr(strPtr, ch)) != NULL) {
		string->rawString[idx++] = *occurrence;
		strPtr = occurrence + 1;
	}

	string->rawString[idx] = '\0';
	string->length = AX_CAST(UInt32, strlen(string->rawString));

	return true;
}

AX_API AX_INLINE
Int32
AX_String_Compare
(SString left, SString right, Bool caseSensitive)
{
	return AX_Misc_String_Strcmp(left->rawString, right->rawString, caseSensitive);
}

AX_API AX_INLINE
Int32
AX_String_Equals
(SString string, SString target, Bool caseSensitive)
{
	return (AX_Misc_String_Strcmp(string->rawString, target->rawString, caseSensitive) == 0);
}

AX_API AX_INLINE
Bool
AX_String_Contains
(SString string, ReadOnlyString substring, Bool caseSensitive)
{
	return
		(!string) ?
		false :
		(AX_Misc_String_Strstr(string->rawString, substring, caseSensitive) != NULL);
}

AX_API AX_INLINE
Bool
AX_String_BeginsWith
(SString string, ReadOnlyString substring)
{
	return
		((!string) || (AX_Misc_String_Strlen(substring) > string->length)) ?
		false :
		(AX_Misc_String_Strncmp(string->rawString, substring, AX_Misc_String_Strlen(substring), true) == 0);
}

AX_API
Bool
AX_String_EndsWith
(SString string, ReadOnlyString substring)
{
	if ((!string) || (AX_Misc_String_Strlen(substring) > string->length))
		return false;

	RawString
		checkPtr = string->rawString + string->length - AX_Misc_String_Strlen(substring),
		last = AX_Misc_String_Strstr(checkPtr, substring, true);

	return ((last != NULL) && (last == checkPtr));
}

AX_API
Bool
AX_String_Clear
(SString string)
{
	if (AX_String_IsEmpty(string))
		return false;

	string->length = 0;
	string->rawString = '\0';

	return true;
}

AX_API AX_INLINE
Int32
AX_String_Search
(SString string, ReadOnlyString substring, Bool caseSensitive)
{
	return
		((!string || !substring) || (AX_Misc_String_Strlen(substring) > string->length)) ?
		-1 :
		AX_CAST(Int32, AX_CAST(PtrDiff, AX_Misc_String_Strstr(string->rawString, substring, caseSensitive) - string->rawString));
}

AX_API AX_INLINE
Int32
AX_String_SearchFirstChar
(SString string, Char ch)
{
	return
		(AX_String_IsEmpty(string)) ?
		-1 :
		AX_CAST(Int32, AX_CAST(PtrDiff, AX_Misc_String_Strchr(string->rawString, ch) - string->rawString));
}

AX_API AX_INLINE
Int32
AX_String_SearchLastChar
(SString string, Char ch)
{
	return
		(AX_String_IsEmpty(string)) ?
		-1 :
		AX_CAST(Int32, AX_CAST(PtrDiff, AX_Misc_String_Strrchr(string->rawString, ch) - string->rawString));
}

AX_API
Bool
AX_String_InsertAt
(SString string, ReadOnlyString substr, Int32 index)
{
	if ((!string || !substr) || (AX_Misc_String_Strlen(substr) == 0))
		return false;

	ByteSize
		subLen = AX_Misc_String_Strlen(substr),
		oldLen = string->length;

	string->length += AX_CAST(UInt32, subLen);
	if (string->length >= string->capacity)
		_AX_String_Resize(string);

	// shift the characters after insertion point
	AX_HAL_Memory_Memmove(
		string->rawString + index + subLen,
		string->rawString + index,
		oldLen - index
	);

	AX_Misc_String_Strncpy(string->rawString + index, substr, subLen);
	string->rawString[string->length] = '\0';

	return true;
}

AX_API
Bool
AX_String_RemoveAt
(SString string, Int32 index, ByteSize count)
{
	if (AX_String_IsEmpty(string) || (!AX_String_IsIndexValid(string, index)) || (count > (AX_CAST(ByteSize, string->length) - index)))
		return false;

	UInt32 newLen = string->length - AX_CAST(UInt32, count);

	AX_HAL_Memory_Memmove(
		string->rawString + index,
		string->rawString + index + count,
		newLen
	);

	string->length = newLen;
	string->rawString[string->length] = '\0';

	return true;
}

AX_API
Bool
AX_String_InsertCharAt
(SString string, Char ch, Int32 index)
{
	if ((!string) || (!AX_String_IsIndexValid(string, index)))
		return false;

	ByteSize oldLen = string->length;

	string->length++;
	if (string->length >= string->capacity)
		_AX_String_Resize(string);

	AX_HAL_Memory_Memmove(
		string->rawString + index + 1,
		string->rawString + index,
		oldLen - index
	);

	string->rawString[index] = ch;
	string->rawString[string->length] = '\0';

	return true;
}

AX_API
Bool
AX_String_RemoveCharAt
(SString string, Int32 index)
{
	if ((!string) || (!AX_String_IsIndexValid(string, index)))
		return false;

	UInt32 newLen = string->length - 1;

	AX_HAL_Memory_Memmove(
		string->rawString + index,
		string->rawString + index + 1,
		newLen
	);

	string->length = newLen;
	string->rawString[string->length] = '\0';

	return true;
}

AX_API
Bool
AX_String_Replace
(SString string, ReadOnlyString oldSubstr, ReadOnlyString newSubstr)
{
	if (AX_String_IsEmpty(string) || (!oldSubstr) || (!newSubstr))
		return false;

	ByteSize oldSubstrLen = AX_Misc_String_Strlen(oldSubstr);

	RawString
		strPtr = string->rawString,
		replacePtr;

	while ((replacePtr = AX_Misc_String_Strstr(strPtr, oldSubstr, true)) != NULL) {
		Int32 replaceIdx = AX_CAST(Int32, AX_CAST(PtrDiff, replacePtr - string->rawString));

		AX_String_RemoveAt(string, replaceIdx, oldSubstrLen);
		AX_String_InsertAt(string, newSubstr, replaceIdx);

		strPtr = replacePtr + 1;
	}

	return true;
}

AX_API
Bool
AX_String_ReplaceChar
(SString string, Char oldChar, Char newChar)
{
	if (AX_String_IsEmpty(string))
		return false;

	RawString
		strPtr = string->rawString,
		replacePtr;

	while ((replacePtr = AX_Misc_String_Strchr(strPtr, oldChar)) != NULL) {
		Int32 replaceIdx = AX_CAST(Int32, AX_CAST(PtrDiff, replacePtr - string->rawString));
		AX_HAL_Memory_Memset(string->rawString + replaceIdx, newChar, sizeof(Char));

		strPtr = replacePtr + 1;
	}

	return true;
}

AX_API
Bool
AX_String_Reverse
(SString string)
{
	if (AX_String_IsEmpty(string))
		return false;

	RawString
		start = string->rawString,
		end = string->rawString + string->length - 1;

	while (start < end)
		AX_HAL_Memory_Memswap(start++, end--, sizeof(Char));

	return true;
}

AX_API
Bool
AX_String_ToLower
(SString string)
{
	if (AX_String_IsEmpty(string))
		return false;

	for (UInt32 i = 0; i < string->length; ++i)
		string->rawString[i] += (string->rawString[i] >= 'A' && string->rawString[i] <= 'Z') ? 32 : 0;

	return true;
}

AX_API
Bool
AX_String_ToUpper
(SString string)
{
	if (AX_String_IsEmpty(string))
		return false;

	for (UInt32 i = 0; i < string->length; i++)
		string->rawString[i] -= (string->rawString[i] >= 'a' && string->rawString[i] <= 'z') ? 32 : 0;

	return true;
}

AX_API AX_INLINE
Bool
AX_String_IsEmpty
(SString string)
{
	return ((!string) || (string->length == 0));
}

AX_API AX_INLINE
Bool
AX_String_IsIndexValid
(SString string, Int32 index)
{
	return ((string != NULL) && (index >= 0 && index < AX_CAST(Int32, string->length)));
}

AX_API AX_INLINE
RawString
AX_String_GetString
(SString string)
{
	return (!string) ? NULL : string->rawString;
}

AX_API AX_INLINE
Int32
AX_String_GetLength
(SString string)
{
	return (!string) ? -1 : string->length;
}

AX_API AX_INLINE
Int32
AX_String_GetCapacity
(SString string)
{
	return (!string) ? -1 : string->capacity;
}

AX_API AX_INLINE
Bool
AX_String_IsNumeric
(SString string)
{
	return (!string) ? false : AX_Misc_String_IsNumeric(string->rawString);
}

AX_API
RawString
AX_String_GetMid
(SString string, Int32 startIndex, Bool caseSensitive)
{
	if ((!string) || (!AX_String_IsIndexValid(string, startIndex)))
		return NULL;

	RawString start = string->rawString + startIndex;
	return AX_Misc_String_Strfind(string->rawString, start, caseSensitive);
}
