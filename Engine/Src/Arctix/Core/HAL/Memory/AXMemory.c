#include "Arctix/Core/HAL/Memory/AXMemory.h"

#include <stdlib.h>
#include <string.h>


AX_API AX_INLINE
VoidPtr
AX_HAL_Memory_Malloc
(ByteSize size)
{
	return malloc(size);
}

AX_API AX_INLINE
void
AX_HAL_Memory_Free
(VoidPtr ptr)
{
	free(ptr);
}

AX_API AX_INLINE
VoidPtr
AX_HAL_Memory_Calloc
(ByteSize count, ByteSize size)
{
	return calloc(count, size);
}

AX_API AX_INLINE
VoidPtr
AX_HAL_Memory_Realloc
(VoidPtr ptr, ByteSize size)
{
	return realloc(ptr, size);
}

AX_API AX_INLINE
VoidPtr
AX_HAL_Memory_Memset
(VoidPtr ptr, Int32 value, ByteSize size)
{
	return memset(ptr, value, size);
}

AX_API AX_INLINE
VoidPtr
AX_HAL_Memory_Memcpy
(VoidPtr dest, const VoidPtr src, ByteSize size)
{
	return memcpy(dest, src, size);
}

AX_API AX_INLINE
VoidPtr
AX_HAL_Memory_Memmove
(VoidPtr dest, const VoidPtr src, ByteSize size)
{
	return memmove(dest, src, size);
}

AX_API AX_INLINE
VoidPtr
AX_HAL_Memory_Memzero
(VoidPtr ptr, ByteSize size)
{
	return memset(ptr, 0, size);
}

AX_API AX_INLINE
Int32
AX_HAL_Memory_Memcmp
(VoidPtr left, VoidPtr right, ByteSize size)
{
	return memcmp(left, right, size);
}

AX_API
Bool
AX_HAL_Memory_Memswap
(VoidPtr left, VoidPtr right, ByteSize size)
{
	VoidPtr temp = AX_HAL_Memory_Malloc(size);
	if (!temp)
		return false;
	
	AX_HAL_Memory_Memcpy(temp, left, size);
	AX_HAL_Memory_Memcpy(left, right, size);
	AX_HAL_Memory_Memcpy(right, temp, size);

	AX_HAL_Memory_Free(temp);
	return true;
}

