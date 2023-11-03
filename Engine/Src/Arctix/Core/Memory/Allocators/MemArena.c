#include "Arctix/Core/Memory/Allocators/MemArena.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"
#include "Arctix/Core/Math/Helper/MathHelper.h"


AX_INLINE
VoidPtr
_AX_Memory_Arena_AlignForward
(VoidPtr ptr, ByteSize alignment)
{
	#define	MISALIGNMENT	(AX_CAST(UIntPtr, ptr) & (alignment - 1))
	return AX_CAST(VoidPtr, AX_CAST(UIntPtr, ptr) + ((MISALIGNMENT != 0) ? (alignment - MISALIGNMENT) : 0));
}


AX_API
SMemArena
AX_Memory_Arena_Construct
(ByteSize capacity)
{
	SMemArena arena = AX_Module_Memory_Allocate(
		sizeof(AX_Memory_Allocator_Arena),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
	);

	if (!arena) {
		AX_Module_Memory_Deallocate(
			arena,
			sizeof(AX_Memory_Allocator_Arena),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
		);

		return NULL;
	}

	// init properties
	{
		arena->capacity = capacity;
		arena->usedMemory = 0;
		arena->numAllocs = 0;

		arena->memoryBlock = AX_Module_Memory_Allocate(
			capacity,
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
		);

		if (!arena->memoryBlock) {
			AX_Module_Memory_Deallocate(
				arena->memoryBlock,
				capacity,
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
			);

			return NULL;
		}
	}

	return arena;
}

AX_API
Bool
AX_Memory_Arena_Destruct
(SMemArena arena)
{
	if (!arena)
		return false;

	// deallocate the data memory block
	AX_Module_Memory_Deallocate(
		arena->memoryBlock,
		arena->capacity,
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
	);

	// deallocate the arena itself
	AX_Module_Memory_Deallocate(
		arena,
		sizeof(AX_Memory_Allocator_Arena),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
	);

	return true;
}

AX_API
VoidPtr
AX_Memory_Arena_AllocateBlock_Aligned
(SMemArena arena, ByteSize blockSize, ByteSize alignment)
{
	if ((!arena) || !(arena->memoryBlock) || (blockSize == 0) || (alignment == 0))
		return NULL;

	if (!AX_Math_IsPowerOfTwo(alignment))
		return NULL;

	BytePtr currPtr, alignedPtr;
	ByteSize adjustment;

	// align the current offset forward to specified alignment 
	{
		currPtr = AX_CAST(BytePtr, arena->memoryBlock) + arena->usedMemory;
		alignedPtr = _AX_Memory_Arena_AlignForward(currPtr, alignment);
		
		adjustment = AX_CAST(PtrDiff, alignedPtr - currPtr);
	}

	if ((arena->usedMemory + adjustment + blockSize) > arena->capacity)
		return NULL;

	// update properties
	{
		arena->usedMemory += blockSize + adjustment;
		arena->numAllocs++;
	}

	return AX_HAL_Memory_Memzero(alignedPtr, blockSize);
}

AX_API AX_INLINE
VoidPtr
AX_Memory_Arena_AllocateBlock
(SMemArena arena, ByteSize blockSize)
{
	return AX_Memory_Arena_AllocateBlock_Aligned(arena, blockSize, AX_MEMORY_DEFAULT_ALIGNMENT);
}


AX_API
Bool
AX_Memory_Arena_Clear
(SMemArena arena)
{
	if (!arena)
		return false;

	// set properties to zero
	{
		AX_HAL_Memory_Memzero(arena->memoryBlock, arena->usedMemory);
		arena->usedMemory = 0;
		arena->numAllocs = 0;
	}

	return true;
}

