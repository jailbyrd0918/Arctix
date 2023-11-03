#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"

#include "Arctix/Core/Misc/String/AXString.h"

#include "Arctix/Core/Containers/Stack/Stack.h"


AX_API
SStack
AX_Stack_Construct
(void)
{
	SStack newStack;

	// construct new stack
	{
		newStack = AX_Module_Memory_Allocate(
			sizeof(AX_Container_Stack),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		if (!newStack) {
			AX_Module_Memory_Deallocate(
				newStack,
				sizeof(AX_Container_Stack),
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
			);

			return NULL;
		}
	}

	// set properties to zero
	AX_HAL_Memory_Memzero(newStack, sizeof(AX_Container_Stack));

	return newStack;
}

AX_API
Bool
AX_Stack_Destruct
(SStack stack)
{
	if (!stack)
		return false;

	// go through all nodes and deallocate them
	for (SStackNode curr = stack->top; curr != NULL; ) {
		SStackNode prev = curr->previous;

		AX_Module_Memory_Deallocate(
			curr->data,
			curr->dataSize,
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		AX_Module_Memory_Deallocate(curr,
			sizeof(AX_Container_Stack_Node),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		curr = prev;
	};

	// dealocate the stack itself
	AX_Module_Memory_Deallocate(
		stack,
		sizeof(AX_Container_Stack),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
	);

	return true;
}

AX_API
Bool
AX_Stack_Push
(SStack stack, VoidPtr nodeData, ByteSize dataSize)
{
	if (!stack)
		return false;

	if (AX_Stack_IsEmpty(stack)) {
		// construct top node
		SStackNode top = AX_Module_Memory_Allocate(
			sizeof(AX_Container_Stack_Node),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		if (!top) {
			AX_Module_Memory_Deallocate(
				top,
				sizeof(AX_Container_Stack_Node),
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
			);

			return false;
		}

		top->previous = NULL;
		top->dataSize = dataSize;

		top->data = AX_Module_Memory_Allocate(
			dataSize,
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		if (!top->data) {
			AX_Module_Memory_Deallocate(
				top->data,
				top->dataSize,
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
			);

			return false;
		}

		AX_HAL_Memory_Memcpy(top->data, nodeData, dataSize);

		// update stack properties
		{
			stack->top = top;
			stack->length++;
		}
	}
	else {
		// construct new node
		SStackNode node = AX_Module_Memory_Allocate(
			sizeof(AX_Container_Stack_Node),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		if (!node) {
			AX_Module_Memory_Deallocate(
				node,
				sizeof(AX_Container_Stack_Node),
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
			);

			return false;
		}

		node->dataSize = dataSize;

		node->data = AX_Module_Memory_Allocate(
			dataSize,
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		if (!node->data) {
			AX_Module_Memory_Deallocate(
				node->data,
				dataSize,
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
			);

			return false;
		}

		AX_HAL_Memory_Memcpy(node->data, nodeData, dataSize);

		// push to the stack
		{
			SStackNode top = stack->top;
			node->previous = top;

			stack->top = node;
			stack->length++;
		}
	}

	return true;
}

AX_API
Bool
AX_Stack_Pop
(SStack stack)
{
	if (AX_Stack_IsEmpty(stack))
		return false;

	SStackNode currTop = stack->top;
	stack->top = currTop->previous;

	AX_Module_Memory_Deallocate(
		currTop->data,
		currTop->dataSize,
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
	);

	AX_Module_Memory_Deallocate(
		currTop,
		sizeof(AX_Container_Stack_Node),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
	);

	stack->length--;
	return true;
}

AX_API
Bool
AX_Stack_Clear
(SStack stack)
{
	if (AX_Stack_IsEmpty(stack))
		return false;

	for (SStackNode curr = stack->top; curr != NULL; ) {
		SStackNode prev = curr->previous;

		AX_Module_Memory_Deallocate(
			curr->data,
			curr->dataSize,
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		AX_Module_Memory_Deallocate(
			curr,
			sizeof(AX_Container_Stack_Node),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		stack->length--;
		curr = prev;
	}

	return true;
}

AX_API AX_INLINE
SStackNode
AX_Stack_Peek
(SStack stack)
{
	return (AX_Stack_IsEmpty(stack) ? NULL : stack->top);
}

AX_API AX_INLINE
Bool
AX_Stack_IsEmpty
(SStack stack)
{
	return ((!stack) || (stack->length == 0));
}

AX_API AX_INLINE
UInt32
AX_Stack_GetLength
(SStack stack)
{
	return (!stack) ? 0 : stack->length;
}


