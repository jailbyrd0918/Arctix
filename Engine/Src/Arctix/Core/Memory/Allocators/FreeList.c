#include "Arctix/Core/Memory/Allocators/FreeList.h"

#include "Arctix/Core/Modules/Memory/MemoryModule.h"
#include "Arctix/Core/Math/Helper/MathHelper.h"


typedef
struct AX_Memory_Allocator_FreeList_Header
{
	ByteSize	adjustment;
	ByteSize	blockSize;
}
AX_Memory_Allocator_FreeList_Header;

typedef AX_Memory_Allocator_FreeList_Header *SFreeListHeader;


ByteSize
_AX_Memory_FreeList_GetAlignForwardAdjustment
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

VoidPtr
_AX_Memory_FreeList_AlignedAlloc
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
_AX_Memory_FreeList_AlignedFree
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
_AX_Memory_FreeList_IsAddressValid
(SFreeList freeList, VoidPtr ptr)
{
	#define	POOL_START	(AX_CAST(BytePtr, freeList->memoryBlock))
	#define	POOL_END	(AX_CAST(BytePtr, freeList->memoryBlock) + freeList->capacity)

	return
		(ptr != NULL) &&
		(freeList != NULL) &&
		(ptr >= AX_CAST(VoidPtr, POOL_START)) &&
		(ptr < AX_CAST(VoidPtr, POOL_END));
}

void
_AX_Memory_FreeList_InsertNode
(SFreeListNode *headNode, SFreeListNode prevNode, SFreeListNode newNode)
{
	if (!prevNode) {
		if (!(*headNode))
			// the list is empty ...
			newNode->next = *headNode;
		else
			// new node becomes head
			*headNode = newNode;
	}
	else {
		if (!prevNode->next) {
			// new node becomes tail
			prevNode->next = newNode;
			newNode->next = NULL;
		}
		else {
			// cut in between nodes
			newNode->next = prevNode->next;
			prevNode->next = newNode;
		}
	}
}

void
_AX_Memory_FreeList_RemoveNode
(SFreeListNode *headNode, SFreeListNode prevNode, SFreeListNode removeNode)
{
	if (!prevNode)
		*headNode = removeNode->next;
	else
		prevNode->next = removeNode->next;
}

SFreeListNode
_AX_Memory_FreeList_Find_FirstFit
(SFreeList freeList, ByteSize dataSize, ByteSize alignment, ByteSize *getAdjustment, SFreeListNode *getPrevNode)
{	
	SFreeListNode curr = freeList->head;
	SFreeListNode prev = NULL;

	ByteSize adjustment = 0;

	while (curr != NULL) {
		adjustment = _AX_Memory_FreeList_GetAlignForwardAdjustment(curr, alignment, sizeof(AX_Memory_Allocator_FreeList_Header));
		
		ByteSize reqSpace = dataSize + adjustment;
		if (curr->blockSize >= reqSpace)
			// found the node with enough space
			break;

		prev = curr;
		curr = curr->next;
	}

	if (adjustment > 0)
		*getAdjustment = adjustment;

	if (prev != NULL)
		*getPrevNode = prev;

	return curr;
}

SFreeListNode
_AX_Memory_FreeList_Find_BestFit
(SFreeList freeList, ByteSize dataSize, ByteSize alignment, ByteSize *getAdjustment, SFreeListNode *getPrevNode)
{
	ByteSize bestSizeDiff = SIZE_MAX;

	SFreeListNode curr = freeList->head;
	SFreeListNode prev = NULL;
	SFreeListNode best = NULL;

	ByteSize adjustment = 0;

	while (curr != NULL) {
		adjustment = _AX_Memory_FreeList_GetAlignForwardAdjustment(curr, alignment, sizeof(AX_Memory_Allocator_FreeList_Header));
		
		ByteSize reqSpace = dataSize + adjustment;
		if ((curr->blockSize >= reqSpace) && ((curr->blockSize - reqSpace) < bestSizeDiff))
			best = curr;

		prev = curr;
		curr = curr->next;
	}

	if (adjustment > 0)
		*getAdjustment = adjustment;

	if (prev != NULL)
		*getPrevNode = prev;

	return best;
}

SFreeListNode
_AX_Memory_FreeList_Find_NextFit
(SFreeList freeList, ByteSize dataSize, ByteSize alignment, ByteSize *getAdjustment, SFreeListNode *getPrevNode)
{
	SFreeListNode curr = freeList->tail;
	SFreeListNode prev = NULL;

	ByteSize adjustment = 0;

	while (curr != NULL) {
		adjustment = _AX_Memory_FreeList_GetAlignForwardAdjustment(curr, alignment, sizeof(AX_Memory_Allocator_FreeList_Header));

		ByteSize reqSpace = dataSize + adjustment;
		if (curr->blockSize >= reqSpace)
			// found the node with enough space
			break;

		prev = curr;
		curr = curr->next;

		// in case iteration reaches the end of the freelist...
		if (curr == NULL)
			// wrap around to the beginning
			curr = freeList->head;
	}

	if (adjustment > 0)
		*getAdjustment = adjustment;

	if (prev != NULL)
		*getPrevNode = prev;

	return curr;
}

void
_AX_Memory_FreeList_Merge
(SFreeList freeList, SFreeListNode prevNode, SFreeListNode freeNode)
{
	VoidPtr freeNodeAdjBlock = AX_CAST(VoidPtr, AX_CAST(BytePtr, freeNode) + freeNode->blockSize);
	VoidPtr prevNodeAdjBlock = AX_CAST(VoidPtr, AX_CAST(BytePtr, prevNode) + freeNode->blockSize);

	// if the next block is physically adjacent to the block of free node
	if ((freeNode->next != NULL) && (freeNode->next == freeNodeAdjBlock)) {
		// merge with the next block
		freeNode->blockSize += freeNode->next->blockSize;
		
		// remove the next node from free list
		_AX_Memory_FreeList_RemoveNode(&(freeList->head), freeNode, freeNode->next);
	}

	// if the next block is physically adjacent to the block of previous node
	if ((prevNode->next != NULL) && (prevNode->next == prevNodeAdjBlock)) {
		// merge with the next block
		prevNode->blockSize += prevNode->next->blockSize;

		// remove the next node from free list
		_AX_Memory_FreeList_RemoveNode(&(freeList->head), prevNode, prevNode->next);
	}
}


AX_API
SFreeList
AX_Memory_FreeList_Construct
(ByteSize capacity, ByteSize alignment, EFreeListPolicy policy)
{
	if (capacity == 0)
		return NULL;

	if (!AX_Math_IsPowerOfTwo(alignment))
		return NULL;

	SFreeList freeList = AX_Module_Memory_Allocate(
		sizeof(AX_Memory_Allocator_FreeList),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
	);

	if (!freeList) {
		AX_Module_Memory_Deallocate(
			freeList,
			sizeof(AX_Memory_Allocator_FreeList),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
		);

		return NULL;
	}

	// init capacity and allocate memory block, getting allocator adjustment
	{
		freeList->capacity = capacity + alignment;
		freeList->memoryBlock = _AX_Memory_FreeList_AlignedAlloc(capacity, alignment, &(freeList->adjustment));
		if (!freeList->memoryBlock) {
			_AX_Memory_FreeList_AlignedFree(freeList->memoryBlock, freeList->capacity, freeList->adjustment);
			return NULL;
		}
	}

	// init properties
	{
		freeList->head = AX_CAST(SFreeListNode, freeList->memoryBlock);
		freeList->tail = freeList->head;
		freeList->policy = policy;
		freeList->usedMemory = 0;
	}

	return freeList;
}

AX_API
Bool
AX_Memory_FreeList_Destruct
(SFreeList freeList)
{
	if (!freeList)
		return false;

	// deallocate the memory block
	_AX_Memory_FreeList_AlignedFree(freeList->memoryBlock, freeList->capacity, freeList->adjustment);

	// deallocate the pool itself
	AX_Module_Memory_Deallocate(
		freeList,
		sizeof(AX_Memory_Allocator_FreeList),
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_ALLOCATOR)
	);

	return true;
}

AX_API
VoidPtr
AX_Memory_FreeList_Allocate
(SFreeList freeList, ByteSize dataSize, ByteSize alignment)
{
	if (!freeList || (dataSize == 0))
		return NULL;

	ByteSize adjustment = 0;
	SFreeListNode prevNode = NULL;
	SFreeListNode currNode = NULL;

	dataSize = AX_MATH_MAX(dataSize, sizeof(AX_Memory_Allocator_FreeList_Node));
	alignment = AX_MATH_MAX(alignment, AX_MEMORY_DEFAULT_ALIGNMENT);

	switch (freeList->policy) {
		case AX_MEMORY_FREELIST_POLICY_FIRST_FIT:
			currNode = _AX_Memory_FreeList_Find_FirstFit(freeList, dataSize, alignment, &adjustment, &prevNode);
			break;

		case AX_MEMORY_FREELIST_POLICY_BEST_FIT:
			currNode = _AX_Memory_FreeList_Find_BestFit(freeList, dataSize, alignment, &adjustment, &prevNode);
			break;

		case AX_MEMORY_FREELIST_POLICY_NEXT_FIT:
			currNode = _AX_Memory_FreeList_Find_NextFit(freeList, dataSize, alignment, &adjustment, &prevNode);
			break;

		default:
			break;
	}

	if (!currNode)
		return NULL;

	ByteSize
		alignedAdjustment = adjustment - sizeof(AX_Memory_Allocator_FreeList_Header),
		reqSpace = dataSize + adjustment,
		remaining = currNode->blockSize - reqSpace;

	if (remaining > 0) {
		SFreeListNode newNode = AX_CAST(SFreeListNode, AX_CAST(BytePtr, currNode) + alignedAdjustment);
		newNode->blockSize = dataSize;
		
		_AX_Memory_FreeList_InsertNode(&(freeList->head), currNode, newNode);
	}

	_AX_Memory_FreeList_RemoveNode(&(freeList->head), prevNode, currNode);

	SFreeListHeader header = AX_CAST(SFreeListHeader, AX_CAST(BytePtr, currNode) + alignedAdjustment);
	header->blockSize = reqSpace;
	header->adjustment = alignedAdjustment;

	freeList->usedMemory += reqSpace;
	freeList->tail = currNode;

	return AX_CAST(VoidPtr, AX_CAST(BytePtr, header) + sizeof(AX_Memory_Allocator_FreeList_Header));
}

AX_API
Bool
AX_Memory_FreeList_Free
(SFreeList freeList, VoidPtr ptr)
{
	if (!freeList)
		return false;

	if (!_AX_Memory_FreeList_IsAddressValid(freeList, ptr))
		return false;

	SFreeListHeader header = AX_CAST(SFreeListHeader, AX_CAST(BytePtr, ptr) - sizeof(AX_Memory_Allocator_FreeList_Header));
	
	SFreeListNode freeNode = AX_CAST(SFreeListNode, header);
	freeNode->blockSize = header->blockSize + header->adjustment;
	freeNode->next = NULL;


	SFreeListNode prevNode = NULL;
	SFreeListNode currNode = freeList->head;
	while (currNode != NULL) {
		if (AX_CAST(UIntPtr, ptr) < AX_CAST(UIntPtr, currNode)) {
			_AX_Memory_FreeList_InsertNode(&(freeList->head), prevNode, freeNode);
			break;
		}

		prevNode = currNode;
		currNode = currNode->next;
	}

	freeList->usedMemory -= freeNode->blockSize;
	_AX_Memory_FreeList_Merge(freeList, prevNode, freeNode);
	return true;
}

