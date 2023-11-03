#include "Arctix/Core/Memory/Allocators/MemBuddy.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"
#include "Arctix/Core/Math/Helper/MathHelper.h"


VoidPtr
_AX_Memory_Buddy_AlignedAlloc
(ByteSize size, ByteSize alignment, ByteSize *getAdjustment)
{
	if ((alignment == 0) || (alignment > AX_MEMORY_ALIGNMENT_MAX))
		return NULL;

	ByteSize extSize = size + alignment;

	VoidPtr ptr = AX_Module_Memory_Allocate(
		extSize, 
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
	);

	UIntPtr rawPtr = AX_CAST(UIntPtr, ptr),
		mask = alignment - 1,
		misalignment = (rawPtr & mask);

	PtrDiff adjustment = AX_CAST(PtrDiff, alignment - misalignment);
	*getAdjustment = adjustment;

	UIntPtr alignedPtr = rawPtr + adjustment;
	return AX_CAST(VoidPtr, alignedPtr);
}

Bool
_AX_Memory_Buddy_AlignedFree
(VoidPtr ptr, ByteSize size, ByteSize adjustment)
{
	if (!ptr)
		return false;

	UIntPtr alignedPtr = AX_CAST(UIntPtr, ptr),
		rawPtr = alignedPtr - adjustment;

	AX_Module_Memory_Deallocate(
		AX_CAST(VoidPtr, rawPtr),
		size,
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
	);

	return true;
}

AX_INLINE
ByteSize
_AX_Memory_Buddy_GetSizeAdjustment
(ByteSize size, ByteSize alignment)
{
	return ((size + (alignment - 1)) & ~(alignment - 1));
}

ByteSize
_AX_Memory_Buddy_GetRequiredBlockSize
(SMemBuddy buddy, ByteSize size)
{
	size = _AX_Memory_Buddy_GetSizeAdjustment(size + sizeof(AX_Memory_Allocator_Buddy_Block), buddy->adjustment);
	
	ByteSize reqSize = 1;
	while (size > reqSize) 
		reqSize <<= 1;

	return reqSize;
}

AX_INLINE
SMemBuddyBlock
_AX_Memory_Buddy_NextAdjacentBlock
(SMemBuddyBlock block)
{
	return AX_CAST(SMemBuddyBlock, AX_CAST(BytePtr, block) + block->size);
}

void
_AX_Memory_Buddy_Merge
(SMemBuddyBlock head, SMemBuddyBlock tail)
{
	do {
		SMemBuddyBlock curr = head;
		SMemBuddyBlock adj = _AX_Memory_Buddy_NextAdjacentBlock(curr);

		Bool noMerging = true;

		// make sure the buddies are within range of the memory block
		while ((curr < tail) && (adj < tail)) {
			if ((curr->isFree) && (adj->isFree) && (curr->size == adj->size)) {
				// merge buddies
				curr->size <<= 1;
				curr = _AX_Memory_Buddy_NextAdjacentBlock(curr);
				if (curr < tail) {
					adj = _AX_Memory_Buddy_NextAdjacentBlock(curr);
					noMerging = false;
				}
			}
			else if (curr->size < adj->size) {
				curr = adj;
				adj = _AX_Memory_Buddy_NextAdjacentBlock(adj);
			}
			else {
				curr = _AX_Memory_Buddy_NextAdjacentBlock(adj);
				if (curr < tail)
					adj = _AX_Memory_Buddy_NextAdjacentBlock(curr);
			}

			if (noMerging)
				return;
		}

	} while (true);
}

SMemBuddyBlock
_AX_Memory_Buddy_Split
(SMemBuddyBlock block, ByteSize size)
{
	if ((block != NULL) && (size > 0)) {
		// perform recursive split...
		while (size < block->size) {
			ByteSize sz = block->size >> 1;
			block->size = sz;

			block = _AX_Memory_Buddy_NextAdjacentBlock(block);
			block->size = sz;
			block->isFree = true;
		}

		if (size <= block->size)
			return block;
	}

	// when block cannot fit the requested allocation size
	return NULL;
}

SMemBuddyBlock
_AX_Memory_Buddy_Find_Best_Block
(SMemBuddyBlock head, SMemBuddyBlock tail, ByteSize size)
{
	SMemBuddyBlock curr = head;
	SMemBuddyBlock adj = _AX_Memory_Buddy_NextAdjacentBlock(curr);
	SMemBuddyBlock best = NULL;

	if ((adj == tail) || (curr->isFree))
		return _AX_Memory_Buddy_Split(curr, size);

	// make sure the buddies are within range of the memory block
	while ((curr < tail) && (adj < tail)) {

		// in case both buddies are free
		if ((curr->isFree) && (adj->isFree) && (curr->size == adj->size)) {
			// merge them
			curr->size <<= 1;
			if ((size <= curr->size) && ((best == NULL) || (curr->size <= best->size)))
				best = curr;

			curr = _AX_Memory_Buddy_NextAdjacentBlock(adj);
			if (curr < tail)
				adj = _AX_Memory_Buddy_NextAdjacentBlock(curr);

			continue;
		}

		if ((curr->isFree) && (size <= curr->size) && ((best == NULL) || (curr->size <= best->size)))
			best = curr;

		if ((adj->isFree) && (size <= adj->size) && ((best == NULL) || (adj->size < best->size)))
			best = adj;

		if (curr->size <= adj->size) {
			curr = _AX_Memory_Buddy_NextAdjacentBlock(adj);
			if (curr < tail)
				adj = _AX_Memory_Buddy_NextAdjacentBlock(curr);
		}
		else {
			curr = adj;
			adj = _AX_Memory_Buddy_NextAdjacentBlock(adj);
		}
	}

	if (best != NULL)
		// in case the best block is also the perfect fit
		return _AX_Memory_Buddy_Split(best, size);

	// out of memory
	return NULL;
}


AX_API
SMemBuddy
AX_Memory_Buddy_Construct_Aligned
(ByteSize capacity, ByteSize alignment)
{
	if (capacity == 0)
		return NULL;

	if (!AX_Math_IsPowerOfTwo(capacity) || !AX_Math_IsPowerOfTwo(alignment))
		return NULL;

	if (!AX_Math_IsPowerOfTwo(sizeof(AX_Memory_Allocator_Buddy_Block)))
		return NULL;

	alignment = AX_MATH_MAX(alignment, sizeof(AX_Memory_Allocator_Buddy_Block));

	SMemBuddy buddy = AX_Module_Memory_Allocate(
		sizeof(AX_Memory_Allocator_Buddy),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
	);
	if (!buddy) {
		AX_Module_Memory_Deallocate(
			buddy,
			sizeof(AX_Memory_Allocator_Buddy),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
		);

		return NULL;
	}

	// init capacity and allocate memory block, getting allocator adjustment
	{
		buddy->capacity = capacity + alignment;
		buddy->memoryBlock = _AX_Memory_Buddy_AlignedAlloc(capacity, alignment, &(buddy->adjustment));
		if (!buddy->memoryBlock) {
			_AX_Memory_Buddy_AlignedFree(buddy->memoryBlock, capacity, buddy->adjustment);
			return NULL;
		}
	}

	// init properties
	{
		buddy->head = AX_CAST(SMemBuddyBlock, buddy->memoryBlock);
		buddy->head->size = capacity;
		buddy->head->isFree = true;

		buddy->tail = _AX_Memory_Buddy_NextAdjacentBlock(buddy->head);

		buddy->usedMemory = 0;
	}

	return buddy;
}

AX_API AX_INLINE
SMemBuddy
AX_Memory_Buddy_Construct
(ByteSize capacity)
{
	return AX_Memory_Buddy_Construct_Aligned(capacity, AX_MEMORY_DEFAULT_ALIGNMENT);
}

AX_API
Bool
AX_Memory_Buddy_Destruct
(SMemBuddy buddy)
{
	if (!buddy)
		return false;

	// deallocate the memory block
	_AX_Memory_Buddy_AlignedFree(buddy->memoryBlock, buddy->capacity, buddy->adjustment);

	// deallocate the pool itself
	AX_Module_Memory_Deallocate(
		buddy,
		sizeof(AX_Memory_Allocator_Buddy),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
	);

	return true;
}

AX_API
VoidPtr
AX_Memory_Buddy_Allocate
(SMemBuddy buddy, ByteSize dataSize)
{
	if (!buddy || (dataSize == 0))
		return NULL;

	ByteSize size = _AX_Memory_Buddy_GetRequiredBlockSize(buddy, dataSize);

	SMemBuddyBlock block = _AX_Memory_Buddy_Find_Best_Block(buddy->head, buddy->tail, size);
	if (!block) {
		// merge all free buddies and search again
		_AX_Memory_Buddy_Merge(buddy->head, buddy->tail);
		block = _AX_Memory_Buddy_Find_Best_Block(buddy->head, buddy->tail, size);
	}

	if (block != NULL) {
		block->isFree = false;
		buddy->usedMemory += size;
		
		return AX_CAST(VoidPtr, block);
	}

	return NULL;
}

AX_API
Bool
AX_Memory_Buddy_Free
(SMemBuddy buddy, VoidPtr ptr)
{
	if (!buddy || !ptr || !(buddy->head) || (AX_CAST(UIntPtr, ptr) >= AX_CAST(UIntPtr, buddy->tail)))
		return false;

	SMemBuddyBlock block = AX_CAST(SMemBuddyBlock, ptr);
	block->isFree = true;

	_AX_Memory_Buddy_Merge(buddy->head, buddy->tail);
	return true;
}


