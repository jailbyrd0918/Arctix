#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Memory.h"


AX_API
SMemBuddy
AX_Memory_Buddy_Construct_Aligned
(ByteSize capacity, ByteSize alignment);

AX_API AX_INLINE
SMemBuddy
AX_Memory_Buddy_Construct
(ByteSize capacity);

AX_API
Bool
AX_Memory_Buddy_Destruct
(SMemBuddy buddy);

AX_API
VoidPtr
AX_Memory_Buddy_Allocate
(SMemBuddy buddy, ByteSize dataSize);

AX_API
Bool
AX_Memory_Buddy_Free
(SMemBuddy buddy, VoidPtr ptr);

