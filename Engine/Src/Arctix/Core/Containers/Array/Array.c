#include "Arctix/Core/Misc/String/AXString.h"
#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"

#include "Arctix/Core/Containers/Array/Array.h"


AX_API
VoidPtr
AX_IMPL_Array_Construct
(RawString elementType, ByteSize elementSize, UInt32 capacity)
{
	ByteSize
		headerSize = sizeof(AX_Container_Array),
		arraySize = elementSize * capacity,
		totalSize = headerSize + arraySize;

	VoidPtr newArray = AX_Module_Memory_Allocate(
		totalSize,
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
	);

	if (!newArray) {
		AX_Module_Memory_Deallocate(
			newArray,
			totalSize,
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		return NULL;
	}

	AX_HAL_Memory_Memzero(newArray, totalSize);

	SArray newArrayHeader = AX_CAST(SArray, newArray);

	// init properties
	{
		newArrayHeader->elementType = AX_Misc_String_Strdup(elementType);
		newArrayHeader->elementSize = elementSize;
		newArrayHeader->capacity = capacity;
		newArrayHeader->dataPtr = AX_CAST(VoidPtr, AX_CAST(BytePtr, newArray) + headerSize);
	}

	return newArrayHeader->dataPtr;
}

AX_API
Bool
AX_Array_Destruct
(VoidPtr array)
{
	if (!array)
		return false;

	SArray header = AX_CAST(SArray, AX_CAST(BytePtr, array) - sizeof(AX_Container_Array));

	// deallocate
	{
		AX_HAL_Memory_Free(header->elementType);

		AX_Module_Memory_Deallocate(
			header,
			sizeof(AX_Container_Array) + (header->elementSize * header->capacity),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);
	}

	return true;
}

AX_API
VoidPtr
AX_Array_Resize
(VoidPtr array)
{
	if (!array)
		return NULL;

	SArray oldHeader = AX_Array_GetHeader(array);
	UInt32 newCapacity = oldHeader->capacity * AX_CONTAINER_RESIZE_FACTOR;

	// if new capacity is zero, then add it to 1
	newCapacity += (newCapacity == 0);

	VoidPtr newArray =
		AX_IMPL_Array_Construct(
			oldHeader->elementType,
			oldHeader->elementSize,
			newCapacity
		);

	if (!newArray)
		return NULL;

	AX_HAL_Memory_Memcpy(newArray, array, AX_Array_GetElementSize(array) * AX_Array_GetLength(array));

	SArray newHeader = AX_Array_GetHeader(newArray);
	newHeader->length = oldHeader->length;

	AX_Array_Destruct(array);
	return newArray;
}

AX_API
Bool
AX_IMPL_Array_Append
(VoidPtr *arrayRef, VoidPtr other)
{
	if (AX_Array_IsEmpty(*arrayRef) || AX_Array_IsEmpty(other))
		return false;

	// two arrays have to be the same type
	if (AX_Misc_String_Strcmp(AX_Array_GetElementType(*arrayRef), AX_Array_GetElementType(other), true) != 0)
		return false;

	UInt32 newLength = AX_Array_GetLength(*arrayRef) + AX_Array_GetLength(other);
	while (AX_Array_GetCapacity(*arrayRef) <= newLength)
		*arrayRef = AX_Array_Resize(*arrayRef);

	SArray header = AX_Array_GetHeader(*arrayRef);

	AX_HAL_Memory_Memcpy(
		AX_CAST(BytePtr, *arrayRef) + (header->elementSize * header->length),
		other,
		AX_Array_GetElementSize(other) * AX_Array_GetLength(other)
	);

	header->length = newLength;

	return true;
}

AX_API
Bool
AX_IMPL_Array_InsertArray
(VoidPtr *arrayRef, Int32 index, VoidPtr other)
{
	if (AX_Array_IsEmpty(*arrayRef) || AX_Array_IsEmpty(other))
		return false;

	if (!(index >= 0 && AX_CAST(UInt32, index) < AX_Array_GetLength(*arrayRef)))
		return false;

	// two arrays have to be the same type
	if (AX_Misc_String_Strcmp(AX_Array_GetElementType(*arrayRef), AX_Array_GetElementType(other), true) != 0)
		return false;

	UInt32 newLength = AX_Array_GetLength(*arrayRef) + AX_Array_GetLength(other);
	while (AX_Array_GetCapacity(*arrayRef) <= newLength)
		*arrayRef = AX_Array_Resize(*arrayRef);

	SArray header = AX_Array_GetHeader(*arrayRef);

	// shift the data of array by length of 'other' array
	AX_HAL_Memory_Memmove(
		AX_CAST(BytePtr, *arrayRef) + AX_CAST(ByteSize, header->elementSize * AX_CAST(ByteSize, (index + AX_Array_GetLength(other)))),
		AX_CAST(BytePtr, *arrayRef) + AX_CAST(ByteSize, header->elementSize * AX_CAST(ByteSize, index)),
		header->elementSize * AX_CAST(ByteSize, header->length - index)
	);

	// copy over data from 'other' array
	AX_HAL_Memory_Memcpy(
		AX_CAST(BytePtr, *arrayRef) + (header->elementSize * index),
		other,
		AX_Array_GetElementSize(other) * AX_Array_GetLength(other)
	);

	header->length = newLength;
	return true;
}

AX_API
Bool
AX_Array_Clear
(VoidPtr array)
{
	if (AX_Array_IsEmpty(array))
		return false;

	SArray header = AX_Array_GetHeader(array);
	header->length = 0;
	return true;
}

AX_API
Bool
AX_Array_Pop
(VoidPtr array)
{
	if (AX_Array_IsEmpty(array))
		return false;

	SArray header = AX_Array_GetHeader(array);
	header->length--;
	return true;
}

AX_API
Bool
AX_Array_Swap
(VoidPtr array, Int32 leftIndex, Int32 rightIndex)
{
	if (AX_Array_IsEmpty(array) || !AX_Array_IsIndexValid(array, leftIndex) || !AX_Array_IsIndexValid(array, rightIndex))
		return false;

	SArray header = AX_Array_GetHeader(array);

	AX_HAL_Memory_Memswap(
		AX_CAST(BytePtr, array) + (header->elementSize * leftIndex),
		AX_CAST(BytePtr, array) + (header->elementSize * rightIndex),
		header->elementSize
	);

	return true;
}

AX_API AX_INLINE
Bool
AX_Array_IsFull
(VoidPtr array)
{
	return
		(!array) ?
		false :
		(AX_Array_GetLength(array) == AX_Array_GetCapacity(array));
}

AX_API AX_INLINE
Bool
AX_Array_IsEmpty
(VoidPtr array)
{
	return ((!array) || (AX_Array_GetLength(array) == 0));
}

AX_API AX_INLINE
Bool
AX_Array_IsIndexValid
(VoidPtr array, Int32 index)
{
	return (!array) ? false : ((index >= 0) && (AX_CAST(UInt32, index) < AX_Array_GetLength(array)));
}

AX_API AX_INLINE
VoidPtr
AX_Array_GetFirst
(VoidPtr array)
{
	return (!array) ? NULL : array;
}

AX_API AX_INLINE
VoidPtr
AX_Array_GetLast
(VoidPtr array)
{
	return (!array) ? NULL : (AX_CAST(BytePtr, array) + (AX_Array_GetElementSize(array) * (AX_Array_GetLength(array) - 1)));
}

AX_API AX_INLINE
RawString
AX_Array_GetElementType
(VoidPtr array)
{
	return (!array) ? NULL : (AX_Array_GetHeader(array)->elementType);
}

AX_API AX_INLINE
ByteSize
AX_Array_GetElementSize
(VoidPtr array)
{
	return (!array) ? 0 : (AX_Array_GetHeader(array)->elementSize);
}

AX_API AX_INLINE
UInt32
AX_Array_GetLength
(VoidPtr array)
{
	return (!array) ? 0 : (AX_Array_GetHeader(array)->length);
}

AX_API AX_INLINE
UInt32
AX_Array_GetCapacity
(VoidPtr array)
{
	return (!array) ? 0 : (AX_Array_GetHeader(array)->capacity);
}

AX_API AX_INLINE
SArray
AX_Array_GetHeader
(VoidPtr array)
{
	return (!array) ? NULL : AX_CAST(SArray, AX_CAST(BytePtr, array) - sizeof(AX_Container_Array));
}

