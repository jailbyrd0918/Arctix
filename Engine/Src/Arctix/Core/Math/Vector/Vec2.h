#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/DataTypes.h"
#include "Arctix/Defines/Core/Math.h"


AX_API AX_INLINE
UVec2
AX_Math_Vec2_Construct
(Float x, Float y);

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Zero
(void);

AX_API AX_INLINE
UVec2
AX_Math_Vec2_One
(void);

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Scale
(Float scale);

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Up
(void);

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Down
(void);

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Left
(void);

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Right
(void);

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Copy
(UVec2 *vec);

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Add
(UVec2 left, UVec2 right);

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Sub
(UVec2 left, UVec2 right);

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Mul
(UVec2 left, UVec2 right);

AX_API AX_INLINE
UVec2
AX_Math_Vec2_MulScalar
(UVec2 vec, Float scalar);

AX_API AX_INLINE
UVec2
AX_Math_Vec2_MulAdd
(UVec2 a, UVec2 b, UVec2 c);

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Div
(UVec2 left, UVec2 right);

AX_API AX_INLINE
UVec2
AX_Math_Vec2_DivScalar
(UVec2 vec, Float scalar);

AX_API AX_INLINE
Float
AX_Math_Vec2_MagnitudeSquared
(UVec2 vec);

AX_API AX_INLINE
Float
AX_Math_Vec2_Magnitude
(UVec2 vec);

AX_API
Bool
AX_Math_Vec2_Normalize
(UVec2 *vec);

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Normalized
(UVec2 vec);

AX_API AX_INLINE
Bool
AX_Math_Vec2_Equals
(UVec2 left, UVec2 right);

AX_API AX_INLINE
Float
AX_Math_Vec2_Distance
(UVec2 left, UVec2 right);

AX_API AX_INLINE
Float
AX_Math_Vec2_DistanceSquared
(UVec2 left, UVec2 right);

AX_API AX_INLINE
Float
AX_Math_Vec2_Dot
(UVec2 left, UVec2 right);

AX_API AX_INLINE
Float
AX_Math_Vec2_Cross
(UVec2 left, UVec2 right);

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Lerp
(UVec2 start, UVec2 end, Float time);

AX_API
UVec2
AX_Math_Vec2_Slerp
(UVec2 start, UVec2 end, Float time);

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Project
(UVec2 vec, UVec2 normal);

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Reflect
(UVec2 vec, UVec2 normal);




