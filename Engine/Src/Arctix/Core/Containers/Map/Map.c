#include "Arctix/Core/Containers/Map/Map.h"

#include "Arctix/Core/Hash/AXHash.h"
#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"


typedef
struct AX_Container_Map_Item
{
	VoidPtr		key;
	UInt64		hashedID;
}
AX_Container_Map_Item;

typedef	AX_Container_Map_Item *SMapItem;


AX_INLINE
Bool
_AX_Map_IsKeyValid
(const VoidPtr key)
{
	return ((key != NULL) && (AX_Misc_String_Strlen(key) > 0));
}

SMap
_AX_Map_Resize
(SMap map)
{
	if (!map)
		return NULL;

	UInt32 newCapacity = map->capacity * AX_CONTAINER_RESIZE_FACTOR;

	// if new capacity is zero, then add it to 1
	newCapacity += (newCapacity == 0);

	SMap newMap = AX_IMPL_Map_Construct(map->dataSize, newCapacity);
	if (!newMap) {
		AX_Map_Destruct(newMap);
		return map;
	}

	// copy over data of map data pool
	ByteSize poolCopySize = (sizeof(AX_Container_Map_Item) + map->dataSize) * map->capacity;
	AX_HAL_Memory_Memcpy(newMap->pool, map->pool, poolCopySize);

	// copy over map length
	newMap->length = map->length;

	return newMap;
}


AX_API
SMap
AX_IMPL_Map_Construct
(ByteSize dataSize, UInt32 capacity)
{
	if ((capacity == 0) || (dataSize == 0))
		return NULL;

	// construct new map
	SMap newMap = AX_HAL_Memory_Malloc(sizeof(AX_Container_Map));

	if (!newMap) {
		AX_Module_Memory_Deallocate(
			newMap,
			sizeof(AX_Container_Map),
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		return NULL;
	}

	// init properties
	{
		newMap->length = 0;
		newMap->capacity = capacity;
		newMap->dataSize = dataSize;

		newMap->pool = AX_Module_Memory_Allocate(
			(sizeof(AX_Container_Map_Item) + dataSize) * capacity,
			AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
		);

		if (!newMap->pool) {
			AX_Module_Memory_Deallocate(
				newMap->pool,
				(sizeof(AX_Container_Map_Item) + dataSize) * capacity,
				AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
			);

			return NULL;
		}

		AX_HAL_Memory_Memzero(newMap->pool, (sizeof(AX_Container_Map_Item) + dataSize) * capacity);
	}

	return newMap;
}

AX_API
Bool
AX_Map_Destruct
(SMap map)
{
	if (!map)
		return false;

	// deallocate map data pool
	AX_Module_Memory_Deallocate(
		map->pool,
		(sizeof(AX_Container_Map_Item) + map->dataSize) * map->capacity,
		AX_Module_Memory_GetEngineMemoryTagKey(AX_MEMORY_TAG_CONTAINER)
	);

	// deallocate the map itself
	AX_HAL_Memory_Free(map);

	return true;
}

AX_API
Bool
AX_IMPL_Map_Insert
(SMap map, const VoidPtr key, VoidPtr value)
{
	if ((!map) || (!value))
		return false;

	if (!_AX_Map_IsKeyValid(key))
		return false;

	if (AX_Map_IsFull(map))
		_AX_Map_Resize(map);

	HashedInt hashed1 = AX_Hash_HashString(key, AX_HASH_MODE_FNVA64);
	HashedInt hashed2 = AX_Hash_HashString(key, AX_HASH_MODE_CITY64);

	UInt32 index = hashed1 % map->capacity;
	UInt32 stepSize = (hashed2 % (map->capacity - 1)) + 1;

	ByteSize insertOffset = (map->dataSize + sizeof(AX_Container_Map_Item)) * index;
	SMapItem checkSlot = AX_CAST(SMapItem, AX_CAST(BytePtr, map->pool) + insertOffset);

	if (checkSlot->key != NULL) {
		// in case slot with matching key already exists in the map
		if (checkSlot->hashedID == hashed1)
			return false;

		index = (index + stepSize) % map->capacity;
		insertOffset = (map->dataSize + sizeof(AX_Container_Map_Item)) * index;
		checkSlot = AX_CAST(SMapItem, AX_CAST(BytePtr, map->pool) + insertOffset);
	}

	// found available slot ...
	{
		// have insert slot be of the same address
		VoidPtr insertPtr = checkSlot;

		// map item data
		{
			AX_Container_Map_Item itemData = {
				.key = key,
				.hashedID = hashed1
			};

			AX_HAL_Memory_Memcpy(insertPtr, &itemData, sizeof(AX_Container_Map_Item));
		}

		// actual data
		AX_HAL_Memory_Memcpy(AX_CAST(BytePtr, insertPtr) + sizeof(AX_Container_Map_Item), value, map->dataSize);
	}

	map->length++;
	return true;
}

AX_API
Bool
AX_Map_Remove
(SMap map, const VoidPtr key)
{
	if (!_AX_Map_IsKeyValid(key))
		return false;

	if (AX_Map_IsEmpty(map))
		return false;

	HashedInt hashed1 = AX_Hash_HashString(key, AX_HASH_MODE_FNVA64);
	HashedInt hashed2 = AX_Hash_HashString(key, AX_HASH_MODE_CITY64);

	UInt32 index = hashed1 % map->capacity;
	UInt32 stepSize = (hashed2 % (map->capacity - 1)) + 1;

	ByteSize checkOffset = (map->dataSize + sizeof(AX_Container_Map_Item)) * index;
	SMapItem checkSlot = AX_CAST(SMapItem, AX_CAST(BytePtr, map->pool) + checkOffset);

	while (checkSlot->key != NULL) {
		if (checkSlot->hashedID == hashed1) {
			// perform removal
			{
				// set map item data to zero
				AX_HAL_Memory_Memzero(checkSlot, sizeof(AX_Container_Map_Item));

				// set actual data to zero
				AX_HAL_Memory_Memzero(AX_CAST(BytePtr, checkSlot) + sizeof(AX_Container_Map_Item), map->dataSize);
			}

			map->length--;
			return true;
		}

		index = (index + stepSize) % map->capacity;
		checkOffset = (map->dataSize + sizeof(AX_Container_Map_Item)) * index;
		checkSlot = AX_CAST(SMapItem, AX_CAST(BytePtr, map->pool) + checkOffset);
	}

	return false;
}

AX_API
VoidPtr
AX_Map_Search
(SMap map, const VoidPtr key)
{
	if (!_AX_Map_IsKeyValid(key))
		return NULL;

	if (AX_Map_IsEmpty(map))
		return NULL;

	HashedInt hashed1 = AX_Hash_HashString(key, AX_HASH_MODE_FNVA64);
	HashedInt hashed2 = AX_Hash_HashString(key, AX_HASH_MODE_CITY64);

	UInt32 index = hashed1 % map->capacity;
	UInt32 stepSize = (hashed2 % (map->capacity - 1)) + 1;

	ByteSize checkOffset = (map->dataSize + sizeof(AX_Container_Map_Item)) * index;
	SMapItem checkSlot = AX_CAST(SMapItem, AX_CAST(BytePtr, map->pool) + checkOffset);

	while (checkSlot->key != NULL) {
		if (checkSlot->hashedID == hashed1)
			return AX_CAST(VoidPtr, AX_CAST(BytePtr, checkSlot) + sizeof(AX_Container_Map_Item));

		index = (index + stepSize) % map->capacity;
		checkOffset = (map->dataSize + sizeof(AX_Container_Map_Item)) * index;
		checkSlot = AX_CAST(SMapItem, AX_CAST(BytePtr, map->pool) + checkOffset);
	}

	return NULL;
}

AX_API
Bool
AX_IMPL_Map_Set
(SMap map, const VoidPtr key, VoidPtr newValue)
{
	if (AX_Map_IsEmpty(map))
		return false;

	if (!_AX_Map_IsKeyValid(key))
		return false;

	HashedInt hashed1 = AX_Hash_HashString(key, AX_HASH_MODE_FNVA64);
	HashedInt hashed2 = AX_Hash_HashString(key, AX_HASH_MODE_CITY64);

	UInt32 index = hashed1 % map->capacity;
	UInt32 stepSize = (hashed2 % (map->capacity - 1)) + 1;

	ByteSize checkOffset = (map->dataSize + sizeof(AX_Container_Map_Item)) * index;
	SMapItem checkSlot = AX_CAST(SMapItem, AX_CAST(BytePtr, map->pool) + checkOffset);

	while (checkSlot->key != NULL) {
		if (checkSlot->hashedID == hashed1) {
			AX_HAL_Memory_Memcpy(AX_CAST(BytePtr, checkSlot) + sizeof(AX_Container_Map_Item), newValue, map->dataSize);
			return true;
		}

		index = (index + stepSize) % map->capacity;
		checkOffset = (map->dataSize + sizeof(AX_Container_Map_Item)) * index;
		checkSlot = AX_CAST(SMapItem, AX_CAST(BytePtr, map->pool) + checkOffset);
	}

	return false;
}

AX_API
Bool
AX_Map_Clear
(SMap map)
{
	if (AX_Map_IsEmpty(map))
		return false;

	ByteSize poolClearSize = (sizeof(AX_Container_Map_Item) + map->dataSize) * map->capacity;
	AX_HAL_Memory_Memzero(map->pool, poolClearSize);

	map->length = 0;
	return true;
}

AX_API AX_INLINE
Bool
AX_Map_Contains
(SMap map, const VoidPtr key)
{
	return (AX_Map_Search(map, key) != NULL);
}

AX_API AX_INLINE
Bool
AX_Map_IsEmpty
(SMap map)
{
	return ((!map) || (map->length == 0));
}

AX_API AX_INLINE
Bool
AX_Map_IsFull
(SMap map)
{
	return ((map != NULL) && (map->length == map->capacity));
}

AX_API AX_INLINE
UInt32
AX_Map_GetLength
(SMap map)
{
	return (!map) ? 0 : map->length;
}

AX_API AX_INLINE
UInt32
AX_Map_GetCapacity
(SMap map)
{
	return (!map) ? 0 : map->capacity;
}

