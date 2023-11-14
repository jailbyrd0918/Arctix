#pragma once

#include "Arctix/Defines/Core/DataTypes.h"


// -- memory alignment -- //

#if	!defined(AX_MEMORY_DEFAULT_ALIGNMENT)
#	define	AX_MEMORY_DEFAULT_ALIGNMENT		sizeof(VoidPtr)
#endif

#if	!defined(AX_MEMORY_ALIGNMENT_MAX)
#	define	AX_MEMORY_ALIGNMENT_MAX			128
#endif


// -- allocators -- //

typedef
struct AX_Memory_Allocator_Arena
{
	UInt64		usedMemory;
	UInt64		capacity;
	UInt32		numAllocs;
	VoidPtr		memoryBlock;
}
AX_Memory_Allocator_Arena;

typedef	AX_Memory_Allocator_Arena *			SMemArena;

typedef
struct AX_Memory_Allocator_Stack
{
	UInt64		usedMemory;
	UInt64		capacity;
	UInt32		numAllocs;

	VoidPtr		memoryBlock;
	VoidPtr		currPtr;
}
AX_Memory_Allocator_Stack;

typedef AX_Memory_Allocator_Stack *			SMemStack;

typedef struct AX_Memory_Allocator_Pool_Node		AX_Memory_Allocator_Pool_Node;
typedef	AX_Memory_Allocator_Pool_Node *			SMemPoolNode;

struct AX_Memory_Allocator_Pool_Node
{
	SMemPoolNode	next;
};

typedef
struct AX_Memory_Allocator_Pool
{
	UInt64		capacity;
	ByteSize	adjustment;

	SMemPoolNode	head;
	VoidPtr		memoryBlock;

	ByteSize	chunkSize;
	ByteSize	chunkCount;
}
AX_Memory_Allocator_Pool;

typedef AX_Memory_Allocator_Pool *			SMemPool;

typedef struct AX_Memory_Allocator_FreeList_Node	AX_Memory_Allocator_FreeList_Node;
typedef AX_Memory_Allocator_FreeList_Node *		SFreeListNode;

struct AX_Memory_Allocator_FreeList_Node
{
	SFreeListNode	next;
	ByteSize	blockSize;
};

typedef
enum AX_Memory_Allocator_FreeList_Placement_Policy
{
	AX_MEMORY_FREELIST_POLICY_FIRST_FIT,
	AX_MEMORY_FREELIST_POLICY_BEST_FIT,

	// similar to first-fit,
	// but intead of starting from the beginning of the freelist
	// the iteration starts from the last allocated node
	AX_MEMORY_FREELIST_POLICY_NEXT_FIT
}
EFreeListPolicy;

typedef
struct AX_Memory_Allocator_FreeList
{
	UInt64		usedMemory;
	UInt64		capacity;
	ByteSize	adjustment;

	SFreeListNode	head;
	SFreeListNode	tail;
	VoidPtr		memoryBlock;

	EFreeListPolicy	policy;
}
AX_Memory_Allocator_FreeList;

typedef AX_Memory_Allocator_FreeList *			SFreeList;

typedef struct AX_Memory_Allocator_Buddy_Block		AX_Memory_Allocator_Buddy_Block;
typedef AX_Memory_Allocator_Buddy_Block	*		SMemBuddyBlock;

struct AX_Memory_Allocator_Buddy_Block
{
	ByteSize	size;
	Bool		isFree;
};

typedef
struct AX_Memory_Allocator_Buddy
{
	UInt64		usedMemory;
	UInt64		capacity;
	ByteSize	adjustment;

	SMemBuddyBlock	head;
	SMemBuddyBlock	tail;
	VoidPtr		memoryBlock;
}
AX_Memory_Allocator_Buddy;

typedef AX_Memory_Allocator_Buddy *			SMemBuddy;


// -- module -- //


// memory allocation tag

typedef
enum AX_Memory_Tag
{
	AX_MEMORY_TAG_TEMP,
	AX_MEMORY_TAG_APPLICATION,
	AX_MEMORY_TAG_GAME,
	AX_MEMORY_TAG_CONTAINER,
	AX_MEMORY_TAG_ALLOCATOR,
	AX_MEMORY_TAG_DELEGATE,
	AX_MEMORY_TAG_RENDERER,
	AX_MEMORY_TAG_RESOURCES,

	AX_MEMORY_TAGS_TOTAL
}
EMemTag;



// -- binary swapping -- //

#if	defined(__clang__) || defined(__gcc__)
#       define	AX_BSWAP64(x)	__builtin_bswap64(x)
#	define	AX_BSWAP32(X)	__builtin_bswap32(x)

#else
#	define AX_BSWAP32(x)	(						\
			(((x) & 0xff000000U) >> 24)	|			\
			(((x) & 0x00ff0000U) >> 8)	|			\
			(((x) & 0x0000ff00U) << 8)	|			\
			(((x) & 0x000000ffU) << 24)				\
			)

#       define	AX_BSWAP64(x)	(						\
			(((x) & 0xff00000000000000ULL) >> 56)	|		\
			(((x) & 0x00ff000000000000ULL) >> 40)	|		\
			(((x) & 0x0000ff0000000000ULL) >> 24)	|		\
			(((x) & 0x000000ff00000000ULL) >> 8)	|		\
			(((x) & 0x00000000ff000000ULL) << 8)	|		\
			(((x) & 0x0000000000ff0000ULL) << 24)	|		\
			(((x) & 0x000000000000ff00ULL) << 40)	|		\
			(((x) & 0x00000000000000ffULL) << 56)			\
			)
#endif


// -- bit-flag operations -- //

#define	AX_BIT_FLAG_FIELD(bit)				(1 << bit)
#define	AX_BIT_FLAG_TOGGLE(flags, bitmask)		(flags ^= (bitmask))
#define	AX_BIT_FLAG_SET(flags, bitmask)			(flags |= (bitmask))
#define	AX_BIT_FLAG_CLEAR(flags, bitmask)		(flags &= ~(bitmask))
#define	AX_BIT_FLAG_IF_SET(flags, bitmask)		(flags & (bitmask))
#define	AX_BIT_FLAG_IF_ALL_SET(flags, bitmask)		((flags & (bitmask)) == (bitmask))
#define	AX_BIT_FLAG_IF_NOT_SET(flags, bitmask)		(!AX_BIT_FLAG_IF_SET(flags, bitmask))



