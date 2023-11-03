#include "Arctix/Core/Memory/Allocators/MemStack.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"
#include "Arctix/Core/Math/Helper/MathHelper.h"


typedef
struct AX_Memory_Allocator_Stack_Header
{
	VoidPtr		prevPtr;

	ByteSize	adjustment;
	ByteSize	blockSize;
}
AX_Memory_Allocator_Stack_Header;

typedef AX_Memory_Allocator_Stack_Header *SMemStackHeader;


ByteSize
_AX_Memory_Stack_GetAlignForwardAdjustment
(VoidPtr ptr, ByteSize alignment, ByteSize headerSize)
{
	UIntPtr rawPtr = AX_CAST(UIntPtr, ptr),
		bitmask = alignment - 1,
		misalignment = (rawPtr & bitmask);

	UIntPtr adjustment = (misalignment != 0) ? (alignment - misalignment) : 0,
		reqSpace = headerSize;

	if (adjustment < reqSpace) {
		reqSpace -= adjustment;

		if ((reqSpace & (alignment - 1)) != 0) {
			adjustment += alignment * (1 + (reqSpace / alignment));
		}
		else {
			adjustment += alignment * (reqSpace / alignment);
		}
	}

	return AX_CAST(ByteSize, adjustment);
}


AX_API
SMemStack
AX_Memory_Stack_Construct
(ByteSize capacity)
{
	SMemStack stack = AX_Module_Memory_Allocate(
		sizeof(AX_Memory_Allocator_Stack),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
	);

	if (!stack) {
		AX_Module_Memory_Deallocate(
			stack,
			sizeof(AX_Memory_Allocator_Stack),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
		);

		return NULL;
	}

	// init properties
	{
		stack->capacity = capacity;
		stack->numAllocs = 0;
		stack->usedMemory = 0;

		stack->memoryBlock = AX_Module_Memory_Allocate(
			capacity,
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
		);

		if (!stack->memoryBlock) {
			AX_Module_Memory_Deallocate(
				stack->memoryBlock,
				capacity,
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
			);

			return NULL;
		}

		stack->currPtr = stack->memoryBlock;
	}

	return stack;
}

AX_API
Bool
AX_Memory_Stack_Destruct
(SMemStack stack)
{
	if (!stack)
		return false;

	// deallocate the data memory block
	AX_Module_Memory_Deallocate(
		stack->memoryBlock,
		stack->capacity,
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
	);

	// deallocate the stack itself
	AX_Module_Memory_Deallocate(
		stack,
		sizeof(AX_Memory_Allocator_Stack),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
	);

	return true;
}

AX_API
VoidPtr
AX_Memory_Stack_Push_Aligned
(SMemStack stack, ByteSize blockSize, ByteSize alignment)
{
	if ((!stack) || !(stack->memoryBlock) || (blockSize == 0) || (alignment == 0))
		return NULL;

	if (!AX_Math_IsPowerOfTwo(alignment))
		return NULL;

	BytePtr currPtr, nextPtr;
	ByteSize adjustment;
	SMemStackHeader header;

	// ensure that alignment do not exceed 128 bits for proper padding
	alignment = AX_MATH_MIN(alignment, AX_MEMORY_ALIGNMENT_MAX);

	currPtr = stack->currPtr;

	adjustment = _AX_Memory_Stack_GetAlignForwardAdjustment(currPtr, alignment, sizeof(AX_Memory_Allocator_Stack_Header));
	if ((stack->usedMemory + adjustment + blockSize) > stack->capacity)
		return NULL;

	// init header properties
	{
		nextPtr = currPtr + adjustment;
		header = AX_CAST(SMemStackHeader, nextPtr - sizeof(AX_Memory_Allocator_Stack_Header));
		header->prevPtr = currPtr;
		header->adjustment = adjustment;
		header->blockSize = blockSize;
	}

	// update properties
	{
		stack->currPtr = nextPtr;
		stack->usedMemory += adjustment + blockSize;
		stack->numAllocs++;
	}

	return AX_HAL_Memory_Memzero(nextPtr, blockSize);
}

AX_API AX_INLINE
VoidPtr
AX_Memory_Stack_Push
(SMemStack stack, ByteSize blockSize)
{
	return AX_Memory_Stack_Push_Aligned(stack, blockSize, AX_MEMORY_DEFAULT_ALIGNMENT);
}

AX_API
Bool
AX_Memory_Stack_Pop
(SMemStack stack)
{
	if (!stack || !stack->memoryBlock)
		return false;

	BytePtr startPtr = AX_CAST(BytePtr, stack->memoryBlock),
		currPtr = AX_CAST(BytePtr, stack->currPtr);

	SMemStackHeader header = AX_CAST(SMemStackHeader, currPtr - sizeof(AX_Memory_Allocator_Stack_Header));

	// update properties
	{
		stack->currPtr = header->prevPtr;
		stack->usedMemory -= (header->adjustment + header->blockSize);
		stack->numAllocs--;
	}

	return true;
}

AX_API
Bool
AX_Memory_Stack_Clear
(SMemStack stack)
{
	if (!stack)
		return false;

	// set properties to zero
	{
		AX_HAL_Memory_Memzero(stack->memoryBlock, stack->usedMemory);
		stack->usedMemory = 0;
		stack->numAllocs = 0;
		stack->currPtr = stack->memoryBlock;
	}

	return true;
}
