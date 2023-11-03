#include "Arctix/Core/Hash/AXHash.h"

#include "Arctix/Core/Hash/CityHash64/CityHash64.h"
#include "Arctix/Core/Hash/FNVHashA64/FNVHashA64.h"
#include "Arctix/Core/Hash/MurmurHash64/MurmurHash64.h"
#include "Arctix/Core/Hash/XXHash64/XXHash64.h"

AX_API AX_INLINE
HashedInt
AX_Hash_HashBuffer
(const VoidPtr buffer, ByteSize size, EHashMode mode)
{
	return
		(mode == AX_HASH_MODE_CITY64) ?
		AX_CityHash64_HashBuffer(buffer, size) :
		(mode == AX_HASH_MODE_FNVA64) ?
		AX_FNVHashA64_HashBuffer(buffer, size) :
		(mode == AX_HASH_MODE_MURMUR64) ?
		AX_MurmurHash64_HashBuffer(buffer, size) :
		(mode == AX_HASH_MODE_XX64) ?
		AX_XXHash64_HashBuffer(buffer, size) :
		0;
}