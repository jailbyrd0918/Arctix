#include "Arctix/Core/Hash/XXHash64/XXHash64.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"


#define XXHASH64_PRIME1 11400714785074694791ULL
#define XXHASH64_PRIME2 14029467366897019727ULL
#define XXHASH64_PRIME3 1609587929392839161ULL
#define XXHASH64_PRIME4 9650029242287828579ULL
#define XXHASH64_PRIME5 2870177450012600261ULL


typedef
struct AX_XXHash64_State
{
	UInt64	totalLength;
	UInt64	v[4];

	UInt64	mem64[4];
	UInt32	memsize;
}
AX_XXHash64_State;

static AX_XXHash64_State	state;


AX_INLINE
void
_AX_XXHash64_Reset
(void)
{
	AX_HAL_Memory_Memzero(&state, sizeof(AX_XXHash64_State));
}

AX_INLINE
UInt64
_AX_XXHash64_Rotate
(UInt64 value, UInt32 shift)
{
	return
		(shift == 0) ?
		value :
		((value << shift) | (value >> (64 - shift)));
}

AX_INLINE
UInt64
_AX_XXHash64_Read64
(const VoidPtr memPtr, const Bool bigEndian)
{
	const BytePtr readPtr = AX_CAST(BytePtr, memPtr);

	return (bigEndian) ?
		(
			readPtr[7]
			| (AX_CAST(UInt64, readPtr[6]) << 8)
			| (AX_CAST(UInt64, readPtr[5]) << 16)
			| (AX_CAST(UInt64, readPtr[4]) << 24)
			| (AX_CAST(UInt64, readPtr[3]) << 32)
			| (AX_CAST(UInt64, readPtr[2]) << 40)
			| (AX_CAST(UInt64, readPtr[1]) << 48)
			| (AX_CAST(UInt64, readPtr[0]) << 56)
			) :
		(
			readPtr[0]
			| (AX_CAST(UInt64, readPtr[1]) << 8)
			| (AX_CAST(UInt64, readPtr[2]) << 16)
			| (AX_CAST(UInt64, readPtr[3]) << 24)
			| (AX_CAST(UInt64, readPtr[4]) << 32)
			| (AX_CAST(UInt64, readPtr[5]) << 40)
			| (AX_CAST(UInt64, readPtr[6]) << 48)
			| (AX_CAST(UInt64, readPtr[7]) << 56)
			);
}

AX_INLINE
UInt64
_AX_XXHash64_Round
(UInt64 value, UInt64 input)
{
	return (_AX_XXHash64_Rotate(value + (input * XXHASH64_PRIME2), 31) * XXHASH64_PRIME1);
}

AX_INLINE
UInt64
_AX_XXHash64_Avalance
(UInt64 hash)
{
	hash ^= hash >> 33;
	hash *= XXHASH64_PRIME2;
	hash ^= hash >> 29;
	hash *= XXHASH64_PRIME3;
	hash ^= hash >> 32;

	return hash;
}

Bool
_AX_XXHash64_Update
(const VoidPtr input, ByteSize inputLength)
{
	BytePtr ptr = AX_CAST(BytePtr, input);
	BytePtr end = ptr + inputLength;

	state.totalLength += inputLength;

	if ((state.memsize + inputLength) < 32) {
		// fill in the temp buffer
		BytePtr fillPtr = AX_CAST(BytePtr, state.mem64) + state.memsize;
		AX_HAL_Memory_Memcpy(fillPtr, input, inputLength);
		state.memsize += AX_CAST(UInt32, inputLength);
		return false;
	}

	if (state.memsize > 0) {
		BytePtr fillPtr = AX_CAST(BytePtr, state.mem64) + state.memsize;
		ByteSize fillSize = AX_CAST(ByteSize, 32 - state.memsize);

		// fill the remainder of the buffer
		AX_HAL_Memory_Memcpy(fillPtr, input, fillSize);

		// process the remainder
		{
			state.v[0] = _AX_XXHash64_Round(state.v[0], _AX_XXHash64_Read64(state.mem64 + 0, false));
			state.v[1] = _AX_XXHash64_Round(state.v[1], _AX_XXHash64_Read64(state.mem64 + 1, false));
			state.v[2] = _AX_XXHash64_Round(state.v[2], _AX_XXHash64_Read64(state.mem64 + 2, false));
			state.v[3] = _AX_XXHash64_Round(state.v[3], _AX_XXHash64_Read64(state.mem64 + 3, false));
		}

		ptr += fillSize;
		state.memsize = 0;
	}

	if ((ptr + 32) <= end) {
		BytePtr limit = end - 32;

		do {
			state.v[0] = _AX_XXHash64_Round(state.v[0], _AX_XXHash64_Read64(ptr + 0, false));
			state.v[1] = _AX_XXHash64_Round(state.v[1], _AX_XXHash64_Read64(ptr + 8, false));
			state.v[2] = _AX_XXHash64_Round(state.v[2], _AX_XXHash64_Read64(ptr + 16, false));
			state.v[3] = _AX_XXHash64_Round(state.v[3], _AX_XXHash64_Read64(ptr + 24, false));
			ptr += 32;
		} while (ptr <= limit);
	}

	if (ptr < end) {
		AX_HAL_Memory_Memcpy(state.mem64, ptr, AX_CAST(ByteSize, AX_CAST(PtrDiff, end - ptr)));
		state.memsize = AX_CAST(UInt32, end - ptr);
	}

	return true;
}


UInt64
_AX_XXHash64_Finalize
(void)
{
	UInt64 hash;
	BytePtr ptr = AX_CAST(BytePtr, state.mem64);
	BytePtr end = ptr + state.memsize;

	if (state.totalLength >= 32)
		hash =
		_AX_XXHash64_Rotate(state.v[0], 1) +
		_AX_XXHash64_Rotate(state.v[1], 7) +
		_AX_XXHash64_Rotate(state.v[2], 12) +
		_AX_XXHash64_Rotate(state.v[3], 18);
	else
		hash = state.v[2] + XXHASH64_PRIME5;

	hash += state.totalLength;

	while ((ptr + 8) <= end) {
		UInt64 k1 = _AX_XXHash64_Read64(ptr, false);
		k1 *= XXHASH64_PRIME2;
		k1 = _AX_XXHash64_Rotate(k1, 31);
		k1 *= XXHASH64_PRIME1;

		hash ^= k1;
		hash = (_AX_XXHash64_Rotate(hash, 27) * XXHASH64_PRIME1) + XXHASH64_PRIME4;

		ptr += 8;
	}

	if ((ptr + 4) <= end) {
		hash ^= AX_CAST(UInt64, *(AX_CAST(UInt32 *, ptr))) * XXHASH64_PRIME1;
		hash = (_AX_XXHash64_Rotate(hash, 23) * XXHASH64_PRIME2) + XXHASH64_PRIME3;

		ptr += 4;
	}

	while (ptr < end) {
		hash ^= AX_CAST(UInt64, *ptr) * XXHASH64_PRIME5;
		hash = _AX_XXHash64_Rotate(hash, 11) * XXHASH64_PRIME1;

		ptr++;
	}

	return _AX_XXHash64_Avalance(hash);
}

HashedInt
AX_XXHash64_HashBuffer
(const VoidPtr buffer, ByteSize size)
{
	if (!buffer)
		return 0;

	_AX_XXHash64_Reset();

	BytePtr ptr = AX_CAST(BytePtr, buffer);
	BytePtr end = ptr + size;

	while ((ptr + 32) <= end) {
		_AX_XXHash64_Update(ptr, 32);
		ptr += 32;
	}

	if (ptr < end)
		_AX_XXHash64_Update(ptr, AX_CAST(PtrDiff, end - ptr));

	return _AX_XXHash64_Finalize();
}
