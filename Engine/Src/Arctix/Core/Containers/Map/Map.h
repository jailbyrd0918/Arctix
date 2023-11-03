#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Containers.h"


AX_API
SMap
AX_IMPL_Map_Construct
(ByteSize dataSize, UInt32 capacity);

#define	AX_Map_Construct(type)												\
	AX_IMPL_Map_Construct(sizeof(type), AX_CONTAINER_DEFAULT_CAPACITY)

#define	AX_Map_ConstructReserved(type, capacity)										\
	AX_IMPL_Map_Construct(sizeof(type), capacity)

AX_API
Bool
AX_Map_Destruct
(SMap map);

AX_API
Bool
AX_IMPL_Map_Insert
(SMap map, const VoidPtr key, VoidPtr value);

#define	AX_Map_Insert(map, key, value)											\
	AX_IMPL_Map_Insert(map, key, (&(value)))

AX_API
Bool
AX_Map_Remove
(SMap map, const VoidPtr key);

AX_API
VoidPtr
AX_Map_Search
(SMap map, const VoidPtr key);

#define	AX_Map_GetValuePtr(map, type, key)										\
	AX_CAST(type *, AX_Map_Search(map, key))

#define AX_Map_GetValue(map, type, key)											\
	(AX_Map_GetValuePtr(map, type, key)) ? *(AX_Map_GetValuePtr(map, type, key)) : (type){0}

AX_API
Bool
AX_IMPL_Map_Set
(SMap map, const VoidPtr key, VoidPtr newValue);

#define	AX_Map_SetValue(map, key, value)										\
	AX_IMPL_Map_Set(map, key, (&(value)))

AX_API
Bool
AX_Map_Clear
(SMap map);

AX_API AX_INLINE
Bool
AX_Map_Contains
(SMap map, const VoidPtr key);

AX_API AX_INLINE
Bool
AX_Map_IsEmpty
(SMap map);

AX_API AX_INLINE
Bool
AX_Map_IsFull
(SMap map);

AX_API AX_INLINE
UInt32
AX_Map_GetLength
(SMap map);

AX_API AX_INLINE
UInt32
AX_Map_GetCapacity
(SMap map);



