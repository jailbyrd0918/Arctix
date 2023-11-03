#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Containers.h"


AX_API
SString
AX_String_Construct
(ReadOnlyString value);

AX_API
SString
AX_String_ConstructChar
(Char ch, Int32 count);

AX_API
SString
AX_String_ConstructFormatted
(ReadOnlyString format, ...);

AX_API
Bool
AX_String_Destruct
(SString string);

AX_API
Bool
AX_String_Append
(SString string, ReadOnlyString appendStr);

AX_API
Bool
AX_String_AppendChar
(SString string, Char appendCh);

AX_API
Bool
AX_String_AppendChars
(SString string, ReadOnlyString appendStr, UInt32 count);

AX_API AX_INLINE
Bool
AX_String_Append_Int8
(SString string, Int8 value);

AX_API AX_INLINE
Bool
AX_String_Append_Int16
(SString string, Int16 value);

AX_API AX_INLINE
Bool
AX_String_Append_Int32
(SString string, Int32 value);

AX_API AX_INLINE
Bool
AX_String_Append_Int64
(SString string, Int64 value);

AX_API AX_INLINE
Bool
AX_String_Append_UInt8
(SString string, UInt8 value);

AX_API AX_INLINE
Bool
AX_String_Append_UInt16
(SString string, Int16 value);

AX_API AX_INLINE
Bool
AX_String_Append_UInt32
(SString string, UInt32 value);

AX_API AX_INLINE
Bool
AX_String_Append_UInt64
(SString string, UInt64 value);

AX_API AX_INLINE
Bool
AX_String_Append_Bool
(SString string, Bool value);

AX_API AX_INLINE
Bool
AX_String_Append_BoolNum
(SString string, Bool value);

AX_API AX_INLINE
Bool
AX_String_Append_Float
(SString string, Float value);

AX_API AX_INLINE
Bool
AX_String_Append_Float64
(SString string, Float64 value);

AX_API
Bool
AX_String_AppendFormatted
(SString string, ReadOnlyString format, ...);

AX_API
Bool
AX_String_FilterChar
(SString string, Char ch);

AX_API AX_INLINE
Int32
AX_String_Compare
(SString left, SString right, Bool caseSensitive);

AX_API AX_INLINE
Int32
AX_String_Equals
(SString string, SString target, Bool caseSensitive);

AX_API AX_INLINE
Bool
AX_String_Contains
(SString string, ReadOnlyString substring, Bool caseSensitive);

AX_API AX_INLINE
Bool
AX_String_BeginsWith
(SString string, ReadOnlyString substring);

AX_API
Bool
AX_String_EndsWith
(SString string, ReadOnlyString substring);

AX_API
Bool
AX_String_Clear
(SString string);

AX_API AX_INLINE
Int32
AX_String_Search
(SString string, ReadOnlyString substring, Bool caseSensitive);

AX_API AX_INLINE
Int32
AX_String_SearchFirstChar
(SString string, Char ch);

AX_API AX_INLINE
Int32
AX_String_SearchLastChar
(SString string, Char ch);

AX_API
Bool
AX_String_InsertAt
(SString string, ReadOnlyString substr, Int32 index);

AX_API
Bool
AX_String_RemoveAt
(SString string, Int32 index, ByteSize count);

AX_API
Bool
AX_String_InsertCharAt
(SString string, Char ch, Int32 index);

AX_API
Bool
AX_String_RemoveCharAt
(SString string, Int32 index);

AX_API
Bool
AX_String_Replace
(SString string, ReadOnlyString oldSubstr, ReadOnlyString newSubstr);

AX_API
Bool
AX_String_ReplaceChar
(SString string, Char oldChar, Char newChar);

AX_API
Bool
AX_String_Reverse
(SString string);

AX_API
Bool
AX_String_ToLower
(SString string);

AX_API
Bool
AX_String_ToUpper
(SString string);

AX_API AX_INLINE
Bool
AX_String_IsEmpty
(SString string);

AX_API AX_INLINE
Bool
AX_String_IsIndexValid
(SString string, Int32 index);

AX_API AX_INLINE
RawString
AX_String_GetString
(SString string);

AX_API AX_INLINE
Int32
AX_String_GetLength
(SString string);

AX_API AX_INLINE
Int32
AX_String_GetCapacity
(SString string);

AX_API AX_INLINE
Bool
AX_String_IsNumeric
(SString string);

AX_API AX_INLINE
RawString
AX_String_GetMid
(SString string, Int32 startIndex, Bool caseSensitive);


