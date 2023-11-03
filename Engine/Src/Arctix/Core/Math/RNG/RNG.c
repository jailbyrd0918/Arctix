#include "Arctix/Core/Math/RNG/RNG.h"

#include "Arctix/Core/Containers/String/String.h"
#include "Arctix/Core/Misc/String/AXString.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>


#define	RNG_SEED_MULTIPLIER		1812433253UL
#define	RNG_GENERATE_MULTIPLIER		1181783497276652981ULL
#define	RNG_FLOAT_MULTIPLIER		(1.0f / 4294967296.0f)

#define	BUFFER_MAX_SIZE			512


typedef
struct AX_Math_RNG_State
{
	UInt64		initialSeed;
	UInt64		currentSeed;

	UInt64		values[16];
	UInt8		index;
}
SRngState;

static SRngState	state;
static Bool		stateInitialized = false;


UInt64
_AX_Math_RNG_GenerateRandom
(void)
{
	UInt8		index = state.index;
	const UInt64	s = state.values[index++];
	UInt64		t = state.values[index &= 15];

	t ^= t << 31;
	t ^= t >> 11;
	t ^= s ^ (s >> 30);

	state.values[index] = t;
	state.index = index;

	return (t * RNG_GENERATE_MULTIPLIER);
}

AX_API
Bool
AX_Math_RNG_Initialize
(UInt64 seed)
{
	if (stateInitialized)
		return false;

	state.values[0] = seed;

	for (UInt8 i = 0; i < 16; ++i)
		state.values[i] = (seed ^ (seed >> 30)) + (AX_CAST(UInt64, i) * RNG_SEED_MULTIPLIER);

	state.index = 0;
	state.initialSeed = seed;
	state.currentSeed = seed;
	return (stateInitialized = true);
}

AX_API
Bool
AX_Math_RNG_NewSeed
(UInt64 newSeed)
{
	if (newSeed == state.currentSeed)
		return false;

	state.values[0] = newSeed;

	for (UInt8 i = 0; i < 16; ++i)
		state.values[i] = (newSeed ^ (newSeed >> 30)) + (AX_CAST(UInt64, i) * RNG_SEED_MULTIPLIER);

	state.index = 0;
	state.currentSeed = newSeed;
	return true;
}

AX_API AX_INLINE
Bool
AX_Math_RNG_Reset
(void)
{
	return (state.initialSeed == state.currentSeed) ? false : AX_Math_RNG_NewSeed(state.initialSeed);
}

AX_API AX_INLINE
UInt64
AX_Math_RNG_GetInitialSeed
(void)
{
	return state.initialSeed;
}

AX_API AX_INLINE
UInt64
AX_Math_RNG_GetCurrentSeed
(void)
{
	return state.currentSeed;
}

AX_API AX_INLINE
Bool
AX_Math_RNG_RandomBool
(void)
{
	return AX_CAST(Bool, _AX_Math_RNG_GenerateRandom() & 1);
}

AX_API AX_INLINE
Char
AX_Math_RNG_RandomChar
(Bool uppercase)
{
	return (uppercase ? 'A' : 'a') + (AX_CAST(UInt32, _AX_Math_RNG_GenerateRandom()) % 26);
}

AX_API AX_INLINE
Int32
AX_Math_RNG_RandomInt
(void)
{
	return AX_CAST(Int32, _AX_Math_RNG_GenerateRandom());
}

AX_API AX_INLINE
Int32
AX_Math_RNG_RandomInt_Ranged
(Int32 min, Int32 max)
{
	return
		(max < min) ?
		((AX_CAST(UInt32, _AX_Math_RNG_GenerateRandom()) % (min - max + 1)) + max) :
		((AX_CAST(UInt32, _AX_Math_RNG_GenerateRandom()) % (max - min + 1)) + min);
}

AX_API AX_INLINE
UInt32
AX_Math_RNG_RandomUnsignedInt
(void)
{
	return AX_CAST(UInt32, _AX_Math_RNG_GenerateRandom());
}

AX_API AX_INLINE
UInt32
AX_Math_RNG_RandomUnsignedInt_Ranged
(UInt32 min, UInt32 max)
{
	return
		(max < min) ?
		((AX_CAST(UInt32, _AX_Math_RNG_GenerateRandom()) % (min - max + 1)) + max) :
		((AX_CAST(UInt32, _AX_Math_RNG_GenerateRandom()) % (max - min + 1)) + min);
}

AX_API AX_INLINE
Float
AX_Math_RNG_RandomFloat
(void)
{
	return (AX_CAST(Float, AX_CAST(UInt32, _AX_Math_RNG_GenerateRandom())) * RNG_FLOAT_MULTIPLIER * RAND_MAX);
}

AX_API AX_INLINE
Float
AX_Math_RNG_RandomFloat_Ranged
(Float min, Float max)
{
	return
		(max < min) ?
		((AX_CAST(Float, AX_CAST(UInt32, _AX_Math_RNG_GenerateRandom())) * RNG_FLOAT_MULTIPLIER) * ((min - max) + 1) + max) :
		((AX_CAST(Float, AX_CAST(UInt32, _AX_Math_RNG_GenerateRandom())) * RNG_FLOAT_MULTIPLIER) * ((max - min) + 1) + min);
}

AX_API
Float
AX_Math_RNG_RandomFraction
(Int32 decimalPlaces)
{
	if (decimalPlaces < 0)
		return 0.0f;

	// random float
	Float randValue = AX_Math_RNG_RandomFloat();

	// calculate multiplier accord to the specified decimal places
	Float mul = 1.0f;
	for (Int32 i = 0; i < decimalPlaces; ++i)
		mul *= 10.0f;

	return (randValue / mul);
}

AX_API AX_INLINE
UVec2
AX_Math_RNG_RandomVec2
(void)
{
	return AX_STRUCT(
		UVec2, 
		AX_Math_RNG_RandomFloat(), 
		AX_Math_RNG_RandomFloat()
	);
}

AX_API AX_INLINE
UVec2
AX_Math_RNG_RandomVec2_Ranged
(Float min, Float max)
{
	return AX_STRUCT(
		UVec2,
		AX_Math_RNG_RandomFloat_Ranged(min, max),
		AX_Math_RNG_RandomFloat_Ranged(min, max)
	);
}

AX_API AX_INLINE
UVec3
AX_Math_RNG_RandomVec3
(void)
{
	return AX_STRUCT(
		UVec3,
		AX_Math_RNG_RandomFloat(),
		AX_Math_RNG_RandomFloat(),
		AX_Math_RNG_RandomFloat()
	);
}

AX_API AX_INLINE
UVec3
AX_Math_RNG_RandomVec3_Ranged
(Float min, Float max)
{
	return AX_STRUCT(
		UVec3,
		AX_Math_RNG_RandomFloat_Ranged(min, max),
		AX_Math_RNG_RandomFloat_Ranged(min, max),
		AX_Math_RNG_RandomFloat_Ranged(min, max)
	);
}

AX_API AX_INLINE
UVec4
AX_Math_RNG_RandomVec4
(void)
{
	return AX_STRUCT(
		UVec4,
		AX_Math_RNG_RandomFloat(),
		AX_Math_RNG_RandomFloat(),
		AX_Math_RNG_RandomFloat(),
		AX_Math_RNG_RandomFloat()
	);
}

AX_API AX_INLINE
UVec4
AX_Math_RNG_RandomVec4_Ranged
(Float min, Float max)
{
	return AX_STRUCT(
		UVec4,
		AX_Math_RNG_RandomFloat_Ranged(min, max),
		AX_Math_RNG_RandomFloat_Ranged(min, max),
		AX_Math_RNG_RandomFloat_Ranged(min, max),
		AX_Math_RNG_RandomFloat_Ranged(min, max)
	);
}

AX_API
UMat3
AX_Math_RNG_RandomMat3x3
(void)
{
	UMat3 mat;

	for (Int32 i = 0; i < 9; i += 3) {
		mat.elements[i + 0] = AX_Math_RNG_RandomFloat();
		mat.elements[i + 1] = AX_Math_RNG_RandomFloat();
		mat.elements[i + 2] = AX_Math_RNG_RandomFloat();
	}

	return mat;
}

AX_API
UMat3
AX_Math_RNG_RandomMat3x3_Ranged
(Float min, Float max)
{
	UMat3 mat;

	for (Int32 i = 0; i < 9; i += 3) {
		mat.elements[i + 0] = AX_Math_RNG_RandomFloat_Ranged(min, max);
		mat.elements[i + 1] = AX_Math_RNG_RandomFloat_Ranged(min, max);
		mat.elements[i + 2] = AX_Math_RNG_RandomFloat_Ranged(min, max);
	}

	return mat;
}

AX_API
UMat4
AX_Math_RNG_RandomMat4x4
(void)
{
	UMat4 mat;

	for (Int32 i = 0; i < 16; i += 4) {
		mat.elements[i + 0] = AX_Math_RNG_RandomFloat();
		mat.elements[i + 1] = AX_Math_RNG_RandomFloat();
		mat.elements[i + 2] = AX_Math_RNG_RandomFloat();
		mat.elements[i + 3] = AX_Math_RNG_RandomFloat();
	}

	return mat;
}

AX_API
UMat4
AX_Math_RNG_RandomMat4x4_Ranged
(Float min, Float max)
{
	UMat4 mat;

	for (Int32 i = 0; i < 16; i += 4) {
		mat.elements[i + 0] = AX_Math_RNG_RandomFloat_Ranged(min, max);
		mat.elements[i + 1] = AX_Math_RNG_RandomFloat_Ranged(min, max);
		mat.elements[i + 2] = AX_Math_RNG_RandomFloat_Ranged(min, max);
		mat.elements[i + 3] = AX_Math_RNG_RandomFloat_Ranged(min, max);
	}

	return mat;
}

AX_API
SString
AX_Math_RNG_ToString
(void)
{
	SString output = AX_String_Construct("");

	for (UInt8 i = 0; i < 16; ++i)
		AX_String_Append_UInt64(output, state.values[i]);

	return output;
}


