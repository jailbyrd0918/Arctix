#include "Arctix/Core/Containers/Queue/Queue.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"


AX_API
SQueue
AX_Queue_Construct
(void)
{
	// construct new queue
	SQueue newQueue = AX_Module_Memory_Allocate(
		sizeof(AX_Container_Queue),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
	);

	if (!newQueue) {
		AX_Module_Memory_Deallocate(
			newQueue,
			sizeof(AX_Container_Queue),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		return NULL;
	}

	// set properties to zero
	AX_HAL_Memory_Memzero(newQueue, sizeof(AX_Container_Queue));

	return newQueue;
}

AX_API
Bool
AX_Queue_Destruct
(SQueue queue)
{
	if (!queue)
		return false;

	// go through all nodes and deallocate them
	for (SQueueNode curr = queue->head; curr != NULL; ) {
		SQueueNode next = curr->next;

		AX_Module_Memory_Deallocate(
			curr->data,
			curr->dataSize,
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		AX_Module_Memory_Deallocate(
			curr,
			sizeof(AX_Container_Queue_Node),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		curr = next;
	}

	// deallocate the queue itself
	AX_Module_Memory_Deallocate(
		queue,
		sizeof(AX_Container_Queue),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
	);

	return true;
}

AX_API
Bool
AX_Queue_Enqueue
(SQueue queue, VoidPtr nodeData, ByteSize dataSize)
{
	if (!queue)
		return false;

	if (AX_Queue_IsEmpty(queue)) {
		// construct head node
		SQueueNode head = AX_Module_Memory_Allocate(
			sizeof(AX_Container_Queue_Node),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		if (!head) {
			AX_Module_Memory_Deallocate(
				head,
				sizeof(AX_Container_Queue_Node),
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
			);

			return false;
		}

		// init node properties
		{
			head->next = NULL;
			head->dataSize = dataSize;

			head->data = AX_Module_Memory_Allocate(
				dataSize,
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
			);

			if (!head->data) {
				AX_Module_Memory_Deallocate(
					head->data,
					head->dataSize,
					AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
				);

				return false;
			}
			AX_HAL_Memory_Memcpy(head->data, nodeData, dataSize);
		}

		// update queue properties
		{
			queue->head = head;
			queue->tail = head;
			queue->length++;
		}
	}
	else {
		// construct new node
		SQueueNode node = AX_Module_Memory_Allocate(
			sizeof(AX_Container_Queue_Node),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		if (!node) {
			AX_Module_Memory_Deallocate(
				node,
				sizeof(AX_Container_Queue_Node),
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
			);

			return false;
		}

		// init node properties
		{
			node->next = NULL;
			node->dataSize = dataSize;

			node->data = AX_Module_Memory_Allocate(
				dataSize,
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
			);

			if (!node->data) {
				AX_Module_Memory_Deallocate(
					node->data,
					node->dataSize,
					AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
				);

				return false;
			}
			AX_HAL_Memory_Memcpy(node->data, nodeData, dataSize);
		}

		// enqueue
		{
			SQueueNode tail = queue->tail;
			tail->next = node;

			queue->tail = node;
			queue->length++;
		}
	}

	return true;
}

AX_API
Bool
AX_Queue_Dequeue
(SQueue queue)
{
	if (AX_Queue_IsEmpty(queue))
		return false;

	// get queue head
	SQueueNode head = queue->head;

	// set queue's head as current head's next node
	queue->head = head->next;

	// deallocate current head
	{
		AX_Module_Memory_Deallocate(
			head->data,
			head->dataSize,
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		AX_Module_Memory_Deallocate(
			head,
			sizeof(AX_Container_Queue_Node),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);
	}

	queue->length--;
	return true;
}

AX_API
Bool
AX_Queue_Clear
(SQueue queue)
{
	if (AX_Queue_IsEmpty(queue))
		return false;

	// go through all the nodes and deallocate them
	for (SQueueNode curr = queue->head; curr != NULL; ) {
		SQueueNode next = curr->next;

		AX_Module_Memory_Deallocate(
			curr->data,
			curr->dataSize,
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		AX_Module_Memory_Deallocate(
			curr,
			sizeof(AX_Container_Queue_Node),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		queue->length--;
		curr = next;
	}

	return true;
}

AX_API AX_INLINE
Bool
AX_Queue_IsEmpty
(SQueue queue)
{
	return ((!queue) || (queue->length == 0));
}

AX_API AX_INLINE
UInt32
AX_Queue_GetLength
(SQueue queue)
{
	return (!queue) ? 0 : (queue->length);
}

AX_API AX_INLINE
SQueueNode
AX_Queue_GetHeadNode
(SQueue queue)
{
	return (AX_Queue_IsEmpty(queue)) ? NULL : (queue->head);
}

AX_API AX_INLINE
SQueueNode
AX_Queue_GetTailNode
(SQueue queue)
{
	return (AX_Queue_IsEmpty(queue)) ? NULL : (queue->head);
}


