#pragma once

#include "Arctix/Defines/Core/Hash.h"
#include "Arctix/Defines/Core/DataTypes.h"


#define	AX_CONTAINER_DEFAULT_CAPACITY	10
#define	AX_CONTAINER_RESIZE_FACTOR	2


// -- string container -- //

typedef
struct AX_Container_String
{
	RawString	rawString;

	UInt32		length;
	UInt32		capacity;
}
AX_Container_String;

typedef	AX_Container_String *		SString;


// -- dynamic array container -- //

typedef
struct AX_Container_Array
{
	RawString	elementType;
	ByteSize	elementSize;

	UInt32		length;
	UInt32		capacity;

	VoidPtr *	dataPtr;
}
AX_Container_Array;

typedef struct AX_Container_Array *	SArray;


// -- stack container -- //

typedef struct AX_Container_Stack_Node	AX_Container_Stack_Node;
typedef AX_Container_Stack_Node *	SStackNode;

struct AX_Container_Stack_Node
{
	VoidPtr		data;
	ByteSize	dataSize;

	SStackNode	previous;
};

typedef
struct AX_Container_Stack
{
	SStackNode	top;

	UInt32		length;
}
AX_Container_Stack;

typedef AX_Container_Stack *		SStack;


// -- list container -- //

typedef struct AX_Container_List_Node	AX_Container_List_Node;
typedef AX_Container_List_Node *	SListNode;

struct AX_Container_List_Node
{
	VoidPtr		data;
	ByteSize	dataSize;

	SListNode	next;
	SListNode	previous;
};

typedef
struct AX_Container_List
{
	SListNode	head;
	SListNode	tail;

	UInt32		length;
}
AX_Container_List;

typedef AX_Container_List *		SList;


// -- queue container -- //

typedef struct AX_Container_Queue_Node	AX_Container_Queue_Node;
typedef AX_Container_Queue_Node *	SQueueNode;

struct AX_Container_Queue_Node
{
	VoidPtr		data;
	ByteSize	dataSize;

	SQueueNode	next;
};

typedef
struct AX_Container_Queue
{
	SQueueNode	head;
	SQueueNode	tail;

	UInt32		length;
}
AX_Container_Queue;

typedef AX_Container_Queue *		SQueue;


// -- map container -- //

typedef
struct AX_Container_Map
{
	VoidPtr		pool;
	ByteSize	dataSize;

	UInt32		length;
	UInt32		capacity;
}
AX_Container_Map;

typedef	AX_Container_Map *		SMap;


