#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/DataTypes.h"
#include "Arctix/Defines/Core/Math.h"


AX_API AX_INLINE
UMat4
AX_Math_Mat4_Construct
(
	Float x1y1, Float x2y1, Float x3y1, Float x4y1,
	Float x1y2, Float x2y2, Float x3y2, Float x4y2,
	Float x1y3, Float x2y3, Float x3y3, Float x4y3,
	Float x1y4, Float x2y4, Float x3y4, Float x4y4
);

AX_API AX_INLINE
UMat4
AX_Math_Mat4_Zero
(void);

AX_API AX_INLINE
UMat4
AX_Math_Mat4_Identity
(void);

AX_API AX_INLINE
UMat4
AX_Math_Mat4_Copy
(UMat4 *mat);

AX_API AX_INLINE
UMat4
AX_Math_Mat4_FromVectors
(UVec3 *vec1, UVec3 *vec2);

AX_API
UMat4
AX_Math_Mat4_Add
(UMat4 *left, UMat4 *right);

AX_API
UMat4
AX_Math_Mat4_Sub
(UMat4 *left, UMat4 *right);

AX_API
UMat4
AX_Math_Mat4_Mul
(UMat4 *left, UMat4 *right);

AX_API AX_INLINE
UMat4
AX_Math_Mat4_MulScalar
(UMat4 *mat, Float scalar);

AX_API AX_INLINE
UMat4
AX_Math_Mat4_Orthographic
(Float top, Float bottom, Float left, Float right, Float near, Float far);

AX_API AX_INLINE
UMat4
AX_Math_Mat4_Perspective
(Float fovRadian, Float ratio, Float near, Float far);

AX_API AX_INLINE
UMat4
AX_Math_Mat4_LookAt
(UVec3 *position, UVec3 *target, UVec3 *up);

AX_API
Bool
AX_Math_Mat4_Transpose
(UMat4 *mat);

AX_API AX_INLINE
UMat4
AX_Math_Mat4_Transposed
(UMat4 *mat);

AX_API AX_INLINE
Float
AX_Math_Mat4_Determinant
(UMat4 *mat);

AX_API
Bool
AX_Math_Mat4_Inverse
(UMat4 *mat);

AX_API AX_INLINE
UMat4
AX_Math_Mat4_Inversed
(UMat4 *mat);

AX_API AX_INLINE
UMat4
AX_Math_Mat4_MakeTranslation
(UVec3 *position);

AX_API AX_INLINE
UMat4
AX_Math_Mat4_MakeRotationX
(Float angleRadian);

AX_API AX_INLINE
UMat4
AX_Math_Mat4_MakeRotationY
(Float angleRadian);

AX_API AX_INLINE
UMat4
AX_Math_Mat4_MakeRotationZ
(Float angleRadian);

AX_API
UMat4
AX_Math_Mat4_MakeRotationXYZ
(Float xRadian, Float yRadian, Float zRadian);

AX_API AX_INLINE
UMat4
AX_Math_Mat4_MakeScale
(UVec3 *scale);

AX_API
UMat4
AX_Math_Mat4_Transform
(UVec3 *translation, UVec3 *rotation, UVec3 *scale);

AX_API AX_INLINE
UVec3
AX_Math_Mat4_MakeForwardVector
(UMat4 *mat);

AX_API AX_INLINE
UVec3
AX_Math_Mat4_MakeBackwardVector
(UMat4 *mat);

AX_API AX_INLINE
UVec3
AX_Math_Mat4_MakeUpVector
(UMat4 *mat);

AX_API AX_INLINE
UVec3
AX_Math_Mat4_MakeDownVector
(UMat4 *mat);

AX_API AX_INLINE
UVec3
AX_Math_Mat4_MakeLeftVector
(UMat4 *mat);

AX_API AX_INLINE
UVec3
AX_Math_Mat4_MakeRightVector
(UMat4 *mat);

AX_API AX_INLINE
UVec3
AX_Math_Mat4_Mat4MulVec3
(UMat4 *mat, UVec3 *vec);

AX_API AX_INLINE
UVec3
AX_Math_Mat4_Vec3MulMat4
(UVec3 *vec, UMat4 *mat);

AX_API AX_INLINE
UVec4
AX_Math_Mat4_Mat4MulVec4
(UMat4 *mat, UVec4 *vec);

AX_API AX_INLINE
UVec4
AX_Math_Mat4_Vec4MulMat4
(UVec4 *vec, UMat4 *mat);

AX_API AX_INLINE
UVec4
AX_Math_Mat4_GetRow
(UMat4 *mat, Int32 row);

AX_API AX_INLINE
UVec4
AX_Math_Mat4_GetColumn
(UMat4 *mat, Int32 column);

AX_API
Bool
AX_Math_Mat4_SetRow
(UMat4 *mat, Int32 row, UVec4 *data);

AX_API
Bool
AX_Math_Mat4_SetColumn
(UMat4 *mat, Int32 column, UVec4 *data);

AX_API AX_INLINE
UMat3
AX_Math_Mat4_GetTopLeftMat3
(UMat4 *mat);

AX_API
Bool
AX_Math_Mat4_SetTopLeftMat3
(UMat4 *mat, UMat3 *data);

AX_API AX_INLINE
Bool
AX_Math_Mat4_Equals
(UMat4 *left, UMat4 *right);

AX_API AX_INLINE
Bool
AX_Math_Mat4_ApproxEquals
(UMat4 *left, UMat4 *right, Float limit);

AX_API AX_INLINE
Bool
AX_Math_Mat4_IsZero
(UMat4 *mat);

AX_API AX_INLINE
Bool
AX_Math_Mat4_IsIdentity
(UMat4 *mat);


