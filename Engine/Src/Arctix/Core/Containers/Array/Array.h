#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Containers.h"


AX_API
VoidPtr
AX_IMPL_Array_Construct
(RawString elementType, ByteSize elementSize, UInt32 capacity);

#define AX_Array_Construct(type)											\
	AX_IMPL_Array_Construct(#type, sizeof(type), AX_CONTAINER_DEFAULT_CAPACITY)

#define AX_Array_ConstructReserved(type, capacity)									\
	AX_IMPL_Array_Construct(#type, sizeof(type), capacity)

AX_API
Bool
AX_Array_Destruct
(VoidPtr array);

AX_API
VoidPtr
AX_Array_Resize
(VoidPtr array);

#define	AX_Array_Push(array, ...)											\
{															\
	if (AX_Array_IsFull(array)) {											\
		array = AX_Array_Resize(array);										\
	}														\
															\
	SArray header = AX_Array_GetHeader(array);									\
	*(array + (header->length++)) = __VA_ARGS__;									\
}

AX_API
Bool
AX_IMPL_Array_Append
(VoidPtr *arrayRef, VoidPtr other);

#define AX_Array_Append(array, other)											\
	AX_IMPL_Array_Append(&array, other)

#define	AX_Array_Insert(array, index, ...)										\
{															\
	if (AX_Array_IsIndexValid(array, index)) {									\
		if (AX_Array_IsFull(array)) {										\
			array = AX_Array_Resize(array);									\
		}													\
															\
		{													\
			SArray header = AX_Array_GetHeader(array);							\
															\
			AX_HAL_Memory_Memmove(										\
				array + (index) + 1,									\
				array + (index),									\
				header->elementSize * (header->length - (index))					\
			);												\
															\
			*(array + (index)) = __VA_ARGS__;								\
			header->length++;										\
		}													\
	}														\
}

AX_API
Bool
AX_IMPL_Array_InsertArray
(VoidPtr *arrayRef, Int32 index, VoidPtr other);

#define	AX_Array_InsertArray(array, index, other)									\
	AX_IMPL_Array_InsertArray(&(array), index, other)

#define AX_Array_Remove(array, index)											\
{															\
	if (!AX_Array_IsEmpty(array) && AX_Array_IsIndexValid(array, index)) {						\
		SArray header = AX_Array_GetHeader(array);								\
															\
		AX_HAL_Memory_Memmove(											\
			array + (index),										\
			array + (index) + 1,										\
			header->elementSize * (header->length - (index) - 1)						\
		);													\
															\
		header->length--;											\
	}														\
}

AX_API
Bool
AX_Array_Clear
(VoidPtr array);

AX_API
Bool
AX_Array_Pop
(VoidPtr array);

AX_API
Bool
AX_Array_Swap
(VoidPtr array, Int32 leftIndex, Int32 rightIndex);

AX_API AX_INLINE
Bool
AX_Array_IsFull
(VoidPtr array);

AX_API AX_INLINE
Bool
AX_Array_IsEmpty
(VoidPtr array);

AX_API AX_INLINE
Bool
AX_Array_IsIndexValid
(VoidPtr array, Int32 index);

AX_API AX_INLINE
VoidPtr
AX_Array_GetFirst
(VoidPtr array);

AX_API AX_INLINE
VoidPtr
AX_Array_GetLast
(VoidPtr array);

AX_API AX_INLINE
RawString
AX_Array_GetElementType
(VoidPtr array);

AX_API AX_INLINE
ByteSize
AX_Array_GetElementSize
(VoidPtr array);

AX_API AX_INLINE
UInt32
AX_Array_GetLength
(VoidPtr array);

AX_API AX_INLINE
UInt32
AX_Array_GetCapacity
(VoidPtr array);

AX_API AX_INLINE
SArray
AX_Array_GetHeader
(VoidPtr array);


