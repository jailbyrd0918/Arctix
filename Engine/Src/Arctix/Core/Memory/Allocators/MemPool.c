#include "Arctix/Core/Memory/Allocators/MemPool.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"
#include "Arctix/Core/Math/Helper/MathHelper.h"


VoidPtr
_AX_Memory_Pool_AlignedAlloc
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
_AX_Memory_Pool_AlignedFree
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
_AX_Memory_Pool_IsAddressValid
(SMemPool pool, VoidPtr ptr)
{
	#define	POOL_START	(AX_CAST(BytePtr, pool->memoryBlock))
	#define	POOL_END	(AX_CAST(BytePtr, pool->memoryBlock) + pool->capacity)

	return 
		(ptr != NULL) && 
		(pool != NULL) && 
		(ptr >= AX_CAST(VoidPtr, POOL_START)) &&
		(ptr < AX_CAST(VoidPtr, POOL_END));
}


AX_API
SMemPool
AX_Memory_Pool_Construct_Aligned
(ByteSize chunkSize, ByteSize chunkCount, ByteSize alignment)
{
	if ((chunkCount == 0) || (chunkSize == 0))
		return NULL;

	if (chunkSize < sizeof(VoidPtr))
		return NULL;

	if (!AX_Math_IsPowerOfTwo(alignment))
		return NULL;

	SMemPool pool = AX_Module_Memory_Allocate(
		sizeof(AX_Memory_Allocator_Pool),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
	);

	if (!pool) {
		AX_Module_Memory_Deallocate(
			pool,
			sizeof(AX_Memory_Allocator_Pool),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
		);

		return NULL;
	}
	
	// init capacity and allocate memory block, getting allocator adjustment
	{
		pool->capacity = (chunkSize * chunkCount) + alignment;
		pool->memoryBlock = _AX_Memory_Pool_AlignedAlloc(chunkSize * chunkCount, alignment, &(pool->adjustment));
		if (!pool->memoryBlock) {
			_AX_Memory_Pool_AlignedFree(
				pool->memoryBlock, 
				chunkSize * chunkCount, 
				pool->adjustment
			);

			return NULL;
		}
	}
	
	// init free list
	{
		pool->head = AX_CAST(SMemPoolNode, pool->memoryBlock);
		
		SMemPoolNode currNode = pool->head;
		for (ByteSize i = 0; i < chunkCount - 1; ++i) {
			SMemPoolNode nextNode = currNode + chunkSize;
			currNode->next = nextNode;
			currNode = nextNode;
		}
		currNode = NULL;
	}

	// init remaining properties
	{
		pool->chunkSize = chunkSize;
		pool->chunkCount = chunkCount;
	}

	return pool;
}

AX_API AX_INLINE
SMemPool
AX_Memory_Pool_Construct
(ByteSize chunkSize, ByteSize chunkCount)
{
	return AX_Memory_Pool_Construct_Aligned(chunkSize, chunkCount, AX_MEMORY_DEFAULT_ALIGNMENT);
}

AX_API
Bool
AX_Memory_Pool_Destruct
(SMemPool pool)
{
	if (!pool)
		return false;

	// deallocate the memory block
	_AX_Memory_Pool_AlignedFree(pool->memoryBlock, pool->chunkSize * pool->chunkCount, pool->adjustment);
	
	// deallocate the pool itself
	AX_Module_Memory_Deallocate(
		pool,
		sizeof(AX_Memory_Allocator_Pool),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
	);

	return true;
}

AX_API
VoidPtr
AX_Memory_Pool_Allocate
(SMemPool pool)
{
	SMemPoolNode node = pool->head;
	if (!node)
		return NULL;

	pool->head = pool->head->next;
	return AX_HAL_Memory_Memzero(node, pool->chunkSize);
}

AX_API
Bool
AX_Memory_Pool_Free
(SMemPool pool, VoidPtr ptr)
{
	if (!pool || !ptr)
		return false;

	if (!_AX_Memory_Pool_IsAddressValid(pool, ptr))
		return false;

	SMemPoolNode node = AX_CAST(SMemPoolNode, ptr);
	
	// push free node to the list
	{
		node->next = pool->head;
		pool->head = node;
	}

	return true;
}

AX_API
Bool
AX_Memory_Pool_FreeAll
(SMemPool pool)
{
	if (!pool)
		return false;
	
	for (ByteSize i = 0; i < pool->chunkCount - 1; ++i) {
		BytePtr ptr = AX_CAST(BytePtr, pool->memoryBlock) + (i * pool->chunkSize);
		SMemPoolNode node = AX_CAST(SMemPoolNode, ptr);

		// push free node to the list
		{
			node->next = pool->head;
			pool->head = node;
		}
	}

	return true;
}


