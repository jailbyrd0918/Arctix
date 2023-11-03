#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Containers.h"
#include "Arctix/Defines/Core/Math.h"


AX_API
Bool
AX_Math_RNG_Initialize
(UInt64 seed);

AX_API
Bool
AX_Math_RNG_NewSeed
(UInt64 newSeed);

AX_API AX_INLINE
Bool
AX_Math_RNG_Reset
(void);

AX_API AX_INLINE
UInt64
AX_Math_RNG_GetInitialSeed
(void);

AX_API AX_INLINE
UInt64
AX_Math_RNG_GetCurrentSeed
(void);

AX_API AX_INLINE
Bool
AX_Math_RNG_RandomBool
(void);

AX_API AX_INLINE
Char
AX_Math_RNG_RandomChar
(Bool uppercase);

AX_API AX_INLINE
Int32
AX_Math_RNG_RandomInt
(void);

AX_API AX_INLINE
Int32
AX_Math_RNG_RandomInt_Ranged
(Int32 min, Int32 max);

AX_API AX_INLINE
UInt32
AX_Math_RNG_RandomUnsignedInt
(void);

AX_API AX_INLINE
UInt32
AX_Math_RNG_RandomUnsignedInt_Ranged
(UInt32 min, UInt32 max);

AX_API AX_INLINE
Float
AX_Math_RNG_RandomFloat
(void);

AX_API AX_INLINE
Float
AX_Math_RNG_RandomFloat_Ranged
(Float min, Float max);

AX_API
Float
AX_Math_RNG_RandomFraction
(Int32 decimalPlaces);

AX_API AX_INLINE
UVec2
AX_Math_RNG_RandomVec2
(void);

AX_API AX_INLINE
UVec2
AX_Math_RNG_RandomVec2_Ranged
(Float min, Float max);

AX_API AX_INLINE
UVec3
AX_Math_RNG_RandomVec3
(void);

AX_API AX_INLINE
UVec3
AX_Math_RNG_RandomVec3_Ranged
(Float min, Float max);

AX_API AX_INLINE
UVec4
AX_Math_RNG_RandomVec4
(void);

AX_API AX_INLINE
UVec4
AX_Math_RNG_RandomVec4_Ranged
(Float min, Float max);

AX_API AX_INLINE
UMat3
AX_Math_RNG_RandomMat3x3
(void);

AX_API AX_INLINE
UMat3
AX_Math_RNG_RandomMat3x3_Ranged
(Float min, Float max);

AX_API AX_INLINE
UMat4
AX_Math_RNG_RandomMat4x4
(void);

AX_API AX_INLINE
UMat4
AX_Math_RNG_RandomMat4x4_Ranged
(Float min, Float max);

AX_API
SString
AX_Math_RNG_ToString
(void);
