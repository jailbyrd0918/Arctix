#include "Arctix/Core/Hash/MurmurHash64/MurmurHash64.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"


#define	MURMUR64_SEED		0
#define	MURMUR64_CONSTANT1	9782798678568883157ULL
#define	MURMUR64_CONSTANT2	5545529020109919103ULL
#define	MURMUR64_CONSTANT3	944331445U
#define	MURMUR64_CONSTANT4	1390208809U
#define	MURMUR64_MULTIPLIER	5
#define	MURMUR64_MODMASK	18446744073709551608ULL
#define	MURMUR64_PRIME1		18397679294719823053ULL
#define	MURMUR64_PRIME2		14181476777654086739ULL


AX_INLINE
UInt64
_AX_MurMur64_Rotate
(UInt64 value, UInt32 shift)
{
	return
		(shift == 0) ?
		value :
		((value << shift) | (value >> (64 - shift)));
}

AX_INLINE
UInt64
_AX_Murmur64_Avalance
(UInt64 hash)
{
	hash ^= hash >> 33;
	hash *= MURMUR64_PRIME1;
	hash ^= hash >> 33;
	hash *= MURMUR64_PRIME2;
	hash ^= hash >> 33;

	return hash;
}

HashedInt
AX_MurmurHash64_HashBuffer
(const VoidPtr buffer, ByteSize size)
{
	if (!buffer)
		return 0;
	
	BytePtr ptr = AX_CAST(BytePtr, buffer);
	ByteSize blockNum = size / 16;

	UInt64
		h1 = MURMUR64_SEED,
		h2 = MURMUR64_SEED,
		*blocks = AX_CAST(UInt64 *, ptr);

	for (ByteSize i = 0; i < blockNum; ++i) {
		UInt64
			k1 = blocks[(i * 2) + 0],
			k2 = blocks[(i * 2) + 1];

		k1 *= MURMUR64_CONSTANT1;
		k1 = _AX_MurMur64_Rotate(k1, 31);
		k1 *= MURMUR64_CONSTANT2;
		h1 ^= k1;

		h1 = _AX_MurMur64_Rotate(h1, 27);
		h1 += h2;
		h1 = (h1 * MURMUR64_MULTIPLIER) + MURMUR64_CONSTANT4;

		k2 *= MURMUR64_CONSTANT2;
		k2 = _AX_MurMur64_Rotate(k2, 33);
		k2 *= MURMUR64_CONSTANT1;
		h2 ^= k2;

		h2 = _AX_MurMur64_Rotate(h2, 31);
		h2 += h1;
		h2 = (h2 * MURMUR64_MULTIPLIER) + MURMUR64_CONSTANT3;
	}

	BytePtr end = ptr + (blockNum * 16);
	UInt64 k1 = 0, k2 = 0;

	switch (size & 15) {
		case 15:
			k2 ^= AX_CAST(UInt64, end[14]) << 48;

		case 14:
			k2 ^= AX_CAST(UInt64, end[13]) << 40;

		case 13:
			k2 ^= AX_CAST(UInt64, end[12]) << 32;

		case 12:
			k2 ^= AX_CAST(UInt64, end[11]) << 24;

		case 11:
			k2 ^= AX_CAST(UInt64, end[10]) << 16;

		case 10:
			k2 ^= AX_CAST(UInt64, end[9]) << 8;

		case 9:
		{
			k2 ^= AX_CAST(UInt64, end[8]) << 0;
			k2 *= MURMUR64_CONSTANT2;
			k2 = _AX_MurMur64_Rotate(k2, 33);
			k2 *= MURMUR64_CONSTANT1;
			h2 ^= k2;
		}

		case 8:
			k1 ^= AX_CAST(UInt64, end[7]) << 56;

		case 7:
			k1 ^= AX_CAST(UInt64, end[6]) << 48;

		case 6:
			k1 ^= AX_CAST(UInt64, end[5]) << 40;

		case 5:
			k1 ^= AX_CAST(UInt64, end[4]) << 32;

		case 4:
			k1 ^= AX_CAST(UInt64, end[3]) << 24;

		case 3:
			k1 ^= AX_CAST(UInt64, end[2]) << 16;

		case 2:
			k1 ^= AX_CAST(UInt64, end[1]) << 8;

		case 1:
		{
			k1 ^= AX_CAST(UInt64, end[0]) << 0;
			k1 *= MURMUR64_CONSTANT1;
			k1 = _AX_MurMur64_Rotate(k1, 33);
			k1 *= MURMUR64_CONSTANT2;
			h1 ^= k1;
		}
	}

	h1 ^= size;
	h2 ^= size;

	h1 += h2;
	h2 += h1;

	h1 = (h1 & MURMUR64_MODMASK) | (size & 7);
	h2 = (h2 & MURMUR64_MODMASK) | (size & 7);

	h1 += h2;
	h2 += h1;

	h1 = _AX_Murmur64_Avalance(h1);
	h2 = _AX_Murmur64_Avalance(h2);

	return (h1 + h2);
}


