#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/DataTypes.h"
#include "Arctix/Defines/Core/Math.h"


AX_API AX_INLINE
UVec3
AX_Math_Vec3_Construct
(Float x, Float y, Float z);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Zero
(void);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_One
(void);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Scale
(Float scale);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Up
(void);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Down
(void);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Left
(void);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Right
(void);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Forward
(void);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Backward
(void);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Copy
(UVec3 *vec);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Add
(UVec3 left, UVec3 right);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Sub
(UVec3 left, UVec3 right);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Mul
(UVec3 left, UVec3 right);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_MulScalar
(UVec3 vec, Float scalar);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_MulAdd
(UVec3 vec1, UVec3 vec2, UVec3 vec3);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Div
(UVec3 left, UVec3 right);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_DivScalar
(UVec3 vec, Float scalar);

AX_API AX_INLINE
Float
AX_Math_Vec3_MagnitudeSquared
(UVec3 vec);

AX_API AX_INLINE
Float
AX_Math_Vec3_Magnitude
(UVec3 vec);

AX_API
Bool
AX_Math_Vec3_Normalize
(UVec3 *vec);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Normalized
(UVec3 vec);

AX_API AX_INLINE
Float
AX_Math_Vec3_Dot
(UVec3 left, UVec3 right);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Cross
(UVec3 left, UVec3 right);

AX_API AX_INLINE
Bool
AX_Math_Vec3_Equals
(UVec3 left, UVec3 right);

AX_API AX_INLINE
Bool
AX_Math_Vec3_ApproxEquals
(UVec3 left, UVec3 right, Float limit);

AX_API AX_INLINE
Float
AX_Math_Vec3_Distance
(UVec3 left, UVec3 right);

AX_API AX_INLINE
Float
AX_Math_Vec3_DistanceSquared
(UVec3 left, UVec3 right);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Transform
(UVec3 vec, Float w, UMat4 mat);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_ConvertFromVec4
(UVec4 vec);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Lerp
(UVec3 start, UVec3 end, Float time);

AX_API
UVec3
AX_Math_Vec3_Slerp
(UVec3 start, UVec3 end, Float time);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Project
(UVec3 vec, UVec3 normal);

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Reflect
(UVec3 vec, UVec3 normal);





