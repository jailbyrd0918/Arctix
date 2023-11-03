#include "Arctix/Core/Containers/List/List.h"

#include "Arctix/Core/Hash/AXHash.h"
#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"


Bool
_AX_List_IsNodeValid
(SList list, SListNode node)
{
	if ((!list) || (!node))
		return false;

	for (SListNode curr = list->head; curr != NULL; ) {
		if (curr == node)
			// both point to the same address
			return true;

		curr = curr->next;
	}

	return false;
}


AX_API
SList
AX_List_Construct
(void)
{
	// construct new list
	SList newList = AX_Module_Memory_Allocate(
		sizeof(AX_Container_List),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
	);

	if (!newList) {
		AX_Module_Memory_Deallocate(
			newList,
			sizeof(AX_Container_List),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
		);

		return NULL;
	}

	// set properties to zero
	AX_HAL_Memory_Memzero(newList, sizeof(AX_Container_List));

	return newList;
}

AX_API
Bool
AX_List_Destruct
(SList list)
{
	if (!list)
		return false;

	// go through all nodes and deallocate them
	for (SListNode curr = list->head; curr != NULL; ) {
		SListNode next = curr->next;

		AX_Module_Memory_Deallocate(
			curr->data,
			curr->dataSize,
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
		);

		AX_Module_Memory_Deallocate(
			curr,
			sizeof(AX_Container_List_Node),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
		);

		curr = next;
	}

	// deallocate the list itself
	AX_Module_Memory_Deallocate(
		list,
		sizeof(AX_Container_List),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
	);


	return true;
}

AX_API
Bool
AX_List_PushBack
(SList list, VoidPtr nodeData, ByteSize dataSize)
{
	if (!list)
		return false;

	if (AX_List_IsEmpty(list)) {
		// construct head node
		SListNode head = AX_Module_Memory_Allocate(
			sizeof(AX_Container_List_Node),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		if (!head) {
			AX_Module_Memory_Deallocate(
				head,
				sizeof(AX_Container_List_Node),
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
			);
			return false;
		}

		head->previous = NULL;
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
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
			);

			return false;
		}

		AX_HAL_Memory_Memcpy(head->data, nodeData, dataSize);

		// update list properties
		{
			list->head = head;
			list->tail = head;
			list->length++;
		}
	}
	else {
		// construct new node
		SListNode node = AX_Module_Memory_Allocate(
			sizeof(AX_Container_List_Node),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		if (!node) {
			AX_Module_Memory_Deallocate(
				node,
				sizeof(AX_Container_List_Node),
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
			);

			return false;
		}

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
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
			);

			return false;
		}

		AX_HAL_Memory_Memcpy(node->data, nodeData, dataSize);

		// push to the list
		{
			SListNode tail = list->tail;
			tail->next = node;
			node->previous = tail;

			list->tail = node;
			list->length++;
		}
	}

	return true;
}

AX_API
Bool
AX_List_PopBack
(SList list)
{
	if (AX_List_IsEmpty(list))
		return false;

	SListNode tail = list->tail;

	// deallocate properties and node itself
	{
		AX_Module_Memory_Deallocate(
			tail->data,
			tail->dataSize,
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
		);

		AX_Module_Memory_Deallocate(
			tail,
			sizeof(AX_Container_List_Node),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
		);
	}

	list->tail->next = NULL;
	list->length--;

	return true;
}

AX_API
Bool
AX_List_PushFront
(SList list, VoidPtr nodeData, ByteSize dataSize)
{
	if (!list)
		return false;

	if (AX_List_IsEmpty(list)) {
		// construct head node
		SListNode head = AX_Module_Memory_Allocate(
			sizeof(AX_Container_List_Node),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		if (!head) {
			AX_Module_Memory_Deallocate(
				head,
				sizeof(AX_Container_List_Node),
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
			);

			return false;
		}

		head->previous = NULL;
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
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
			);

			return false;
		}

		AX_HAL_Memory_Memcpy(head->data, nodeData, dataSize);

		// update list properties
		{
			list->head = head;
			list->tail = head;
			list->length++;
		}
	}
	else {
		// construct new node
		SListNode node = AX_Module_Memory_Allocate(
			sizeof(AX_Container_List_Node),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		if (!node) {
			AX_Module_Memory_Deallocate(
				node,
				sizeof(AX_Container_List_Node),
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
			);

			return false;
		}

		node->previous = NULL;
		node->dataSize = dataSize;

		node->data = AX_Module_Memory_Allocate(
			dataSize,
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		if (!node->data) {
			AX_Module_Memory_Deallocate(
				node->data,
				node->dataSize,
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
			);

			return false;
		}

		AX_HAL_Memory_Memcpy(node->data, nodeData, dataSize);

		// push to front of list
		{
			SListNode head = list->head;

			head->previous = node;
			node->next = head;

			list->head = node;
			list->length++;
		}
	}

	return true;
}

AX_API
Bool
AX_List_PopFront
(SList list)
{
	if (AX_List_IsEmpty(list))
		return false;

	SListNode head = list->head;

	list->head = head->next;

	AX_Module_Memory_Deallocate(
		head->data,
		head->dataSize,
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
	);

	AX_Module_Memory_Deallocate(
		head,
		sizeof(AX_Container_List_Node),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
	);

	list->head->previous = NULL;

	list->length--;
	return true;
}

AX_API
Bool
AX_List_Insert
(SList list, SListNode insertNode, VoidPtr nodeData, ByteSize dataSize)
{
	if (AX_List_IsEmpty(list))
		return false;

	if (!_AX_List_IsNodeValid(list, insertNode))
		return false;

	if (list->length == 0 || list->length == 1)
		return AX_List_PushFront(list, nodeData, dataSize);

	SListNode insertPrevNode = insertNode->previous;

	// construct new node
	{
		SListNode newNode = AX_Module_Memory_Allocate(
			sizeof(AX_Container_List_Node),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		if (!newNode) {
			AX_Module_Memory_Deallocate(
				newNode,
				sizeof(AX_Container_List_Node),
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
			);

			return false;
		}

		newNode->next = insertNode;
		newNode->previous = insertPrevNode;
		newNode->dataSize = dataSize;

		newNode->data = AX_Module_Memory_Allocate(
			dataSize,
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		if (!newNode->data) {
			AX_Module_Memory_Deallocate(
				newNode->data,
				newNode->dataSize,
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
			);

			return false;
		}

		AX_HAL_Memory_Memcpy(newNode->data, nodeData, dataSize);

		// update insert next node
		insertNode->previous = newNode;

		// update insert previous node
		insertPrevNode->next = newNode;
	}

	list->length++;
	return true;
}

AX_API
Bool
AX_List_Remove
(SList list, SListNode removeNode)
{
	if (AX_List_IsEmpty(list))
		return false;

	if (!_AX_List_IsNodeValid(list, removeNode))
		return false;

	if (list->length == 1)
		return AX_List_PopFront(list);

	// get the neighboring nodes
	SListNode removeNextNode = removeNode->previous;
	SListNode removePrevNode = removeNode->next;

	if (!removeNextNode)
		return AX_List_PopFront(list);

	if (!removePrevNode)
		return AX_List_PopBack(list);

	// update neighboring nodes
	{
		removeNextNode->next = removePrevNode;
		removePrevNode->previous = removeNextNode;
	}

	// deallocate erase node
	AX_Module_Memory_Deallocate(
		removeNode,
		sizeof(AX_Container_List_Node),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
	);

	list->length--;
	return true;
}

AX_API
Bool
AX_List_Clear
(SList list)
{
	if (AX_List_IsEmpty(list))
		return false;

	for (SListNode curr = list->head; curr != NULL; ) {
		SListNode next = curr->next;

		AX_Module_Memory_Deallocate(
			curr->data,
			curr->dataSize,
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
		);

		AX_Module_Memory_Deallocate(
			curr,
			sizeof(AX_Container_List_Node),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
		);

		list->length--;
		curr = next;
	}

	return true;
}

AX_API AX_INLINE
Bool
AX_List_IsEmpty
(SList list)
{
	return ((!list) || (list->length == 0));
}

AX_API AX_INLINE
UInt32
AX_List_GetLength
(SList list)
{
	return (AX_List_IsEmpty(list) ? 0 : list->length);
}

AX_API AX_INLINE
SListNode
AX_List_GetHeadNode
(SList list)
{
	return (AX_List_IsEmpty(list) ? NULL : list->head);
}

AX_API AX_INLINE
SListNode
AX_List_GetTailNode
(SList list)
{
	return (AX_List_IsEmpty(list) ? NULL : list->tail);
}


