#include "Arctix/Core/Hash/FNVHashA64/FNVHashA64.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"


#define FNVHASHA64_PRIME                1099511628211ULL
#define FNVHASHA64_OFFSET_BASIS         14695981039346656037ULL


HashedInt
AX_FNVHashA64_HashBuffer
(const VoidPtr buffer, ByteSize size)
{
        UInt64 hashed = FNVHASHA64_OFFSET_BASIS;
        BytePtr ptr = AX_CAST(BytePtr, buffer);

        for (ByteSize i = 0; i < size; ++i) {
                hashed ^= ptr[i];
                hashed *= FNVHASHA64_PRIME;
        }

        return hashed;
}

