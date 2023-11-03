#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Memory.h"


AX_API
SMemPool
AX_Memory_Pool_Construct_Aligned
(ByteSize chunkSize, ByteSize chunkCount, ByteSize alignment);

AX_API AX_INLINE
SMemPool
AX_Memory_Pool_Construct
(ByteSize chunkSize, ByteSize chunkCount);

AX_API
Bool
AX_Memory_Pool_Destruct
(SMemPool pool);

AX_API
VoidPtr
AX_Memory_Pool_Allocate
(SMemPool pool);

AX_API
Bool
AX_Memory_Pool_Free
(SMemPool pool, VoidPtr ptr);

AX_API
Bool
AX_Memory_Pool_FreeAll
(SMemPool pool);


