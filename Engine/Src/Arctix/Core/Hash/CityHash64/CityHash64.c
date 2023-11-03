#include "Arctix/Core/Hash/CityHash64/CityHash64.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"


#define CITYHASH64_PRIME1	0xc3a5c85c97cb3127ULL
#define CITYHASH64_PRIME2	0xb492b66fbe98f273ULL
#define	CITYHASH64_PRIME3	0x9ae16a3b2f90404fULL


AX_INLINE
Bool
_AX_CityHash64_IsLittleEndian
(UInt64 value)
{
	return (AX_CAST(BytePtr, &value) == AX_CAST(BytePtr, 0x10));
}

AX_INLINE
UInt64
_AX_CityHash64_Rotate
(UInt64 value, UInt32 shift)
{
	return
		(shift == 0) ?
		value :
		((value << shift) | (value >> (64 - shift)));
}

UInt32
_AX_CityHash64_Load32
(const VoidPtr memPtr)
{
	UInt32 output;
	AX_HAL_Memory_Memcpy(&output, memPtr, sizeof(UInt32));

	// if the value is little endian
	if (_AX_CityHash64_IsLittleEndian(output))
		// do nothing
		output = output;

	// otherwise (big endian)
	else
		// perform binary swapping
		output = AX_BSWAP32(output);

	return output;
}

UInt64
_AX_CityHash64_Load64
(const VoidPtr memPtr)
{
	UInt64 output;
	AX_HAL_Memory_Memcpy(&output, memPtr, sizeof(UInt64));

	// if the value is little endian
	if (_AX_CityHash64_IsLittleEndian(output))
		// do nothing
		output = output;

	// otherwise (big endian)
	else
		// perform binary swapping
		output = AX_BSWAP64(output);

	return output;
}

UInt64
_AX_CityHash64_Hash16
(UInt64 a, UInt64 b, UInt64 mul)
{
	UInt64 x = (a ^ b) * mul;
	x ^= (x >> 47);

	UInt64 y = (b ^ x) * mul;

	UInt64 z = (y ^ (y >> 47)) * mul;
	return z;
}

AX_INLINE
UInt64
_AX_CityHash64_ShiftMix
(UInt64 value)
{
	return value ^ (value >> 47);
}

UInt64
_AX_CityHash64_HashFrom0To16
(const VoidPtr input, ByteSize inputLength)
{
	BytePtr ptr = AX_CAST(BytePtr, input);
	BytePtr end = ptr + inputLength;

	if ((ptr + 8) <= end) {
		UInt64 mul = CITYHASH64_PRIME3 + (inputLength << 1);
		UInt64 a = _AX_CityHash64_Load64(ptr) + CITYHASH64_PRIME3;
		UInt64 b = _AX_CityHash64_Load64((ptr + inputLength) - 8);
		UInt64 c = _AX_CityHash64_Rotate(b, 37) * mul + a;
		UInt64 d = (_AX_CityHash64_Rotate(a, 25) + b) * mul;
		return _AX_CityHash64_Hash16(c, d, mul);
	}

	if ((ptr + 4) <= end) {
		UInt64 mul = CITYHASH64_PRIME3 + (inputLength << 1);
		UInt64 a = _AX_CityHash64_Load32(ptr);
		return _AX_CityHash64_Hash16(
			inputLength + (a << 3),
			_AX_CityHash64_Load32((ptr + inputLength) - 4),
			mul
		);
	}

	if (inputLength > 0) {
		UInt8 a = AX_CAST(UInt8, ptr[0]);
		UInt8 b = AX_CAST(UInt8, ptr[inputLength >> 1]);
		UInt8 c = AX_CAST(UInt8, ptr[inputLength - 1]);

		UInt32 x = AX_CAST(UInt32, a) + (AX_CAST(UInt32, b) << 8);
		UInt32 y = AX_CAST(UInt32, inputLength) + (AX_CAST(UInt32, c) << 2);
		return _AX_CityHash64_ShiftMix(x * CITYHASH64_PRIME3 ^ y * CITYHASH64_PRIME1) * CITYHASH64_PRIME3;
	}

	return CITYHASH64_PRIME3;
}

UInt64
_AX_CityHash64_HashFrom17To32
(const VoidPtr input, ByteSize inputLength)
{
	BytePtr ptr = AX_CAST(BytePtr, input);

	UInt64 mul = CITYHASH64_PRIME3 + (inputLength << 1);
	UInt64 a = _AX_CityHash64_Load64(ptr) * CITYHASH64_PRIME2;
	UInt64 b = _AX_CityHash64_Load64(ptr + 8);
	UInt64 c = _AX_CityHash64_Load64((ptr + inputLength) - 8) * mul;
	UInt64 d = _AX_CityHash64_Load64((ptr + inputLength) - 16) * CITYHASH64_PRIME3;

	return _AX_CityHash64_Hash16(
		_AX_CityHash64_Rotate(a + b, 43) + _AX_CityHash64_Rotate(c, 30) + d,
		a + _AX_CityHash64_Rotate(b + CITYHASH64_PRIME3, 18) + c,
		mul
	);
}

UInt64
_AX_CityHash64_HashFrom33To64
(const VoidPtr input, ByteSize inputLength)
{
	BytePtr ptr = AX_CAST(BytePtr, input);
	
	UInt64 mul = CITYHASH64_PRIME3 + inputLength * 2;
	
	UInt64 a = _AX_CityHash64_Load64(ptr) * CITYHASH64_PRIME3;
	UInt64 b = _AX_CityHash64_Load64(ptr + 8);
	UInt64 c = _AX_CityHash64_Load64((ptr + inputLength) - 24);
	UInt64 d = _AX_CityHash64_Load64((ptr + inputLength) - 32);
	UInt64 e = _AX_CityHash64_Load64(ptr + 16) * CITYHASH64_PRIME3;
	UInt64 f = _AX_CityHash64_Load64(ptr + 24) * 9;
	UInt64 g = _AX_CityHash64_Load64((ptr + inputLength) - 8);
	UInt64 h = _AX_CityHash64_Load64((ptr + inputLength) - 16) * mul;
	
	UInt64 u = _AX_CityHash64_Rotate(a + g, 43) + (_AX_CityHash64_Rotate(b, 30) + c) * 9;
	UInt64 v = (((a + g) ^ d) + f) + 1;
	UInt64 w = AX_BSWAP64((u + v) * mul) + h;
	
	UInt64 x = _AX_CityHash64_Rotate(e + f, 42) + c;
	UInt64 y = (AX_BSWAP64((v + w) * mul) + g) * mul;
	UInt64 z = c + e + f;
	
	a = AX_BSWAP64((x + z) * mul + y) + b;
	b = _AX_CityHash64_ShiftMix((((z + a) * mul) + d) + h) * mul;
	
	return (b + x);
}

HashedInt
AX_CityHash64_HashBuffer
(const VoidPtr buffer, ByteSize size)
{
	return
		(!buffer) ?
		0 :
		(size <= 16) ?
		_AX_CityHash64_HashFrom0To16(buffer, size) :
		(size <= 32) ?
		_AX_CityHash64_HashFrom17To32(buffer, size) :
		_AX_CityHash64_HashFrom33To64(buffer, size);
}

