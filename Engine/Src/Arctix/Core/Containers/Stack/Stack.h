#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Containers.h"


AX_API
SStack
AX_Stack_Construct
(void);

AX_API
Bool
AX_Stack_Destruct
(SStack stack);

AX_API
Bool
AX_Stack_Push
(SStack stack, VoidPtr nodeData, ByteSize dataSize);

AX_API
Bool
AX_Stack_Pop
(SStack stack);

AX_API
Bool
AX_Stack_Clear
(SStack stack);

AX_API AX_INLINE
SStackNode
AX_Stack_Peek
(SStack stack);

AX_API AX_INLINE
Bool
AX_Stack_IsEmpty
(SStack stack);

AX_API AX_INLINE
UInt32
AX_Stack_GetLength
(SStack stack);



