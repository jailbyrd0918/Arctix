#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Memory.h"


AX_API
SMemArena
AX_Memory_Arena_Construct
(ByteSize capacity);

AX_API
Bool
AX_Memory_Arena_Destruct
(SMemArena arena);

AX_API
VoidPtr
AX_Memory_Arena_AllocateBlock_Aligned
(SMemArena arena, ByteSize blockSize, ByteSize alignment);

AX_API AX_INLINE
VoidPtr
AX_Memory_Arena_AllocateBlock
(SMemArena arena, ByteSize blockSize);

AX_API
Bool
AX_Memory_Arena_Clear
(SMemArena arena);



