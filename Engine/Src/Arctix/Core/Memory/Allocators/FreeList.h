#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Memory.h"


AX_API
SFreeList
AX_Memory_FreeList_Construct
(ByteSize capacity, ByteSize alignment, EFreeListPolicy policy);

AX_API
Bool
AX_Memory_FreeList_Destruct
(SFreeList freeList);

AX_API
VoidPtr
AX_Memory_FreeList_Allocate
(SFreeList freeList, ByteSize dataSize, ByteSize alignment);

AX_API
Bool
AX_Memory_FreeList_Free
(SFreeList freeList, VoidPtr ptr);



