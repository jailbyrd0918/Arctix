#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Hash.h"
#include "Arctix/Core/Misc/String/AXString.h"


AX_API AX_INLINE
HashedInt
AX_Hash_HashBuffer
(const VoidPtr buffer, ByteSize size, EHashMode mode);

#define	AX_Hash_HashString(string, mode)	\
	AX_Hash_HashBuffer(string, AX_Misc_String_Strlen(string), mode)
