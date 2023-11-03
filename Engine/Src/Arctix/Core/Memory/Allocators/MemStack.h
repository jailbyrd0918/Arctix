#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Memory.h"


AX_API
SMemStack
AX_Memory_Stack_Construct
(ByteSize capacity);

AX_API
Bool
AX_Memory_Stack_Destruct
(SMemStack stack);

AX_API
VoidPtr
AX_Memory_Stack_Push_Aligned
(SMemStack stack, ByteSize blockSize, ByteSize alignment);

AX_API AX_INLINE
VoidPtr
AX_Memory_Stack_Push
(SMemStack stack, ByteSize blockSize);

AX_API
Bool
AX_Memory_Stack_Pop
(SMemStack stack);

AX_API
Bool
AX_Memory_Stack_Clear
(SMemStack stack);


