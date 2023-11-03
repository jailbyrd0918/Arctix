#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/DataTypes.h"


AX_API AX_INLINE
RawString
AX_Misc_String_Strcat
(RawString dest, ReadOnlyString src);

AX_API AX_INLINE
RawString
AX_Misc_String_Strncat
(RawString dest, ReadOnlyString src, ByteSize count);

AX_API AX_INLINE
RawString
AX_Misc_String_Strchr
(RawString str, Char ch);

AX_API AX_INLINE
RawString
AX_Misc_String_Strrchr
(RawString str, Char ch);

AX_API AX_INLINE
Int32
AX_Misc_String_Strcmp
(ReadOnlyString left, ReadOnlyString right, Bool caseSensitive);

AX_API AX_INLINE
Int32
AX_Misc_String_Strncmp
(ReadOnlyString left, ReadOnlyString right, ByteSize count, Bool caseSensitive);

AX_API AX_INLINE
RawString
AX_Misc_String_Strcpy
(RawString dest, ReadOnlyString src);

AX_API AX_INLINE
RawString
AX_Misc_String_Strncpy
(RawString dest, ReadOnlyString src, ByteSize count);

AX_API AX_INLINE
ByteSize
AX_Misc_String_Strspn
(RawString str, ReadOnlyString mask);

AX_API AX_INLINE
ByteSize
AX_Misc_String_Strcspn
(RawString str, ReadOnlyString mask);

AX_API AX_INLINE
RawString
AX_Misc_String_Strdup
(ReadOnlyString str);

AX_API AX_INLINE
ByteSize
AX_Misc_String_Strlen
(ReadOnlyString str);

AX_API AX_INLINE
RawString
AX_Misc_String_Strerr
(Int32 errorNum);

AX_API AX_INLINE
RawString
AX_Misc_String_Strpbrk
(RawString str, ReadOnlyString mask);

AX_API AX_INLINE
RawString
AX_Misc_String_Strtok
(RawString str, ReadOnlyString delim);

AX_API AX_INLINE
ByteSize
AX_Misc_String_Strxfrm
(RawString dest, ReadOnlyString src, ByteSize count);

AX_API AX_INLINE
Float64
AX_Misc_String_Atof
(ReadOnlyString str);

AX_API AX_INLINE
Int32
AX_Misc_String_Atoi
(ReadOnlyString str);

AX_API AX_INLINE
Int64
AX_Misc_String_Atol
(ReadOnlyString str);

AX_API AX_INLINE
Int64
AX_Misc_String_Strtol
(ReadOnlyString str, RawString *end, Int32 base);

AX_API AX_INLINE
UInt64
AX_Misc_String_Strtoul
(ReadOnlyString str, RawString *end, Int32 base);

AX_API AX_INLINE
Float
AX_Misc_String_Strtof
(ReadOnlyString str, RawString *end);

AX_API
RawString
AX_Misc_String_Strstr
(RawString str, ReadOnlyString substr, Bool caseSensitive);

AX_API
RawString
AX_Misc_String_Strfind
(RawString str, ReadOnlyString substr, Bool caseSensitive);

AX_API
Int32
AX_Misc_String_VaArgs
(RawString dest, ByteSize destSize, ReadOnlyString format, VaList args);

AX_API
Int32
AX_Misc_String_Format
(RawString dest, ByteSize destSize, ReadOnlyString format, ...);

AX_API
Bool
AX_Misc_String_IsNumeric
(ReadOnlyString str);

AX_API
Bool
AX_Misc_String_IsAlpha
(ReadOnlyString str);

