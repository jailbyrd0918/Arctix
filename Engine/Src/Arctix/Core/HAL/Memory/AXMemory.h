#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/DataTypes.h"


AX_API AX_INLINE
VoidPtr
AX_HAL_Memory_Malloc
(ByteSize size);

AX_API AX_INLINE
void
AX_HAL_Memory_Free
(VoidPtr ptr);

AX_API AX_INLINE
VoidPtr
AX_HAL_Memory_Calloc
(ByteSize count, ByteSize size);

AX_API AX_INLINE
VoidPtr
AX_HAL_Memory_Realloc
(VoidPtr ptr, ByteSize size);

AX_API AX_INLINE
VoidPtr
AX_HAL_Memory_Memset
(VoidPtr ptr, Int32 value, ByteSize size);

AX_API AX_INLINE
VoidPtr
AX_HAL_Memory_Memcpy
(VoidPtr dest, const VoidPtr src, ByteSize size);

AX_API AX_INLINE
VoidPtr
AX_HAL_Memory_Memmove
(VoidPtr dest, const VoidPtr src, ByteSize size);

AX_API AX_INLINE
VoidPtr
AX_HAL_Memory_Memzero
(VoidPtr ptr, ByteSize size);

AX_API AX_INLINE
Int32
AX_HAL_Memory_Memcmp
(VoidPtr left, VoidPtr right, ByteSize size);

AX_API
Bool
AX_HAL_Memory_Memswap
(VoidPtr left, VoidPtr right, ByteSize size);

