#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Containers.h"


AX_API
SList
AX_List_Construct
(void);

AX_API
Bool
AX_List_Destruct
(SList list);

AX_API
Bool
AX_List_PushBack
(SList list, VoidPtr nodeData, ByteSize dataSize);

AX_API
Bool
AX_List_PopBack
(SList list);

AX_API
Bool
AX_List_PushFront
(SList list, VoidPtr nodeData, ByteSize dataSize);

AX_API
Bool
AX_List_PopFront
(SList list);

AX_API
Bool
AX_List_Insert
(SList list, SListNode insertNode, VoidPtr nodeData, ByteSize dataSize);

AX_API
Bool
AX_List_Remove
(SList list, SListNode removeNode);

AX_API
Bool
AX_List_Clear
(SList list);

AX_API AX_INLINE
Bool
AX_List_IsEmpty
(SList list);

AX_API AX_INLINE
UInt32
AX_List_GetLength
(SList list);

AX_API AX_INLINE
SListNode
AX_List_GetHeadNode
(SList list);

AX_API AX_INLINE
SListNode
AX_List_GetTailNode
(SList list);


