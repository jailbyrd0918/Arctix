#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/DataTypes.h"
#include "Arctix/Defines/Core/Math.h"


AX_API AX_INLINE
UVec4
AX_Math_Vec4_Construct
(Float x, Float y, Float z, Float w);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Zero
(void);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_One
(void);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Scale
(Float scale);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Up
(void);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Down
(void);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Left
(void);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Right
(void);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Forward
(void);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Backward
(void);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Copy
(UVec4 *vec);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Add
(UVec4 left, UVec4 right);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Sub
(UVec4 left, UVec4 right);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Mul
(UVec4 left, UVec4 right);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_MulScalar
(UVec4 vec, Float scalar);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_MulAdd
(UVec4 vec1, UVec4 vec2, UVec4 vec3);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Div
(UVec4 left, UVec4 right);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_DivScalar
(UVec4 vec, Float scalar);

AX_API AX_INLINE
Float
AX_Math_Vec4_MagnitudeSquared
(UVec4 vec);

AX_API AX_INLINE
Float
AX_Math_Vec4_Magnitude
(UVec4 vec);

AX_API
Bool
AX_Math_Vec4_Normalize
(UVec4 *vec);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Normalized
(UVec4 vec);

AX_API AX_INLINE
Float
AX_Math_Vec4_Dot
(UVec4 left, UVec4 right);

AX_API AX_INLINE
Bool
AX_Math_Vec4_Equals
(UVec4 left, UVec4 right);

AX_API AX_INLINE
Bool
AX_Math_Vec4_ApproxEquals
(UVec4 left, UVec4 right, Float limit);

AX_API AX_INLINE
Float
AX_Math_Vec4_Distance
(UVec4 left, UVec4 right);

AX_API AX_INLINE
Float
AX_Math_Vec4_DistanceSquared
(UVec4 left, UVec4 right);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Transform
(UVec4 vec, Float w, UMat4 mat);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_ConvertFromVec3
(UVec3 vec, Float w);

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Lerp
(UVec4 start, UVec4 end, Float time);

AX_API
UVec4
AX_Math_Vec4_Slerp
(UVec4 start, UVec4 end, Float time);

AX_API
UVec4
AX_Math_Vec4_Project
(UVec4 vec, UVec4 normal);


