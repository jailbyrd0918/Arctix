#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Containers.h"


AX_API
SQueue
AX_Queue_Construct
(void);

AX_API
Bool
AX_Queue_Destruct
(SQueue queue);

AX_API
Bool
AX_Queue_Enqueue
(SQueue queue, VoidPtr nodeData, ByteSize dataSize);

AX_API
Bool
AX_Queue_Dequeue
(SQueue queue);

AX_API
Bool
AX_Queue_Clear
(SQueue queue);

AX_API AX_INLINE
Bool
AX_Queue_IsEmpty
(SQueue queue);

AX_API AX_INLINE
UInt32
AX_Queue_GetLength
(SQueue queue);

AX_API AX_INLINE
SQueueNode
AX_Queue_GetHeadNode
(SQueue queue);

AX_API AX_INLINE
SQueueNode
AX_Queue_GetTailNode
(SQueue queue);

