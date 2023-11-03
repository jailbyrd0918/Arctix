#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/DataTypes.h"
#include "Arctix/Defines/Core/Math.h"


AX_API AX_INLINE
UMat3
AX_Math_Mat3_Construct
(
	Float x1y1, Float x2y1, Float x3y1,
	Float x1y2, Float x2y2, Float x3y2,
	Float x1y3, Float x2y3, Float x3y3
);

AX_API AX_INLINE
UMat3
AX_Math_Mat3_Zero
(void);

AX_API AX_INLINE
UMat3
AX_Math_Mat3_Identity
(void);

AX_API AX_INLINE
UMat3
AX_Math_Mat3_Copy
(UMat3 *mat);

AX_API AX_INLINE
UMat3
AX_Math_Mat3_FromVectors
(UVec2 *vec1, UVec2 *vec2);

AX_API
UMat3
AX_Math_Mat3_Add
(UMat3 *left, UMat3 *right);

AX_API
UMat3
AX_Math_Mat3_Sub
(UMat3 *left, UMat3 *right);

AX_API AX_INLINE
UMat3
AX_Math_Mat3_Mul
(UMat3 *left, UMat3 *right);

AX_API AX_INLINE
UMat3
AX_Math_Mat3_MulScalar
(UMat3 *mat, Float scalar);

AX_API
UVec2
AX_Math_Mat3_Mat3MulVec2
(UMat3 *mat, UVec2 *vec);

AX_API
UVec2
AX_Math_Mat3_Vec2MulMat3
(UVec2 *vec, UMat3 *mat);

AX_API
Bool
AX_Math_Mat3_Transpose
(UMat3 *mat);

AX_API AX_INLINE
UMat3
AX_Math_Mat3_Transposed
(UMat3 *mat);

AX_API AX_INLINE
Float
AX_Math_Mat3_Determinant
(UMat3 *mat);

AX_API
Bool
AX_Math_Mat3_Inverse
(UMat3 *mat);

AX_API AX_INLINE
UMat3
AX_Math_Mat3_Inversed
(UMat3 *mat);

AX_API AX_INLINE
UMat3
AX_Math_Mat3_MakeTranslation
(UVec2 *position);

AX_API AX_INLINE
UMat3
AX_Math_Mat3_MakeRotation
(Float angleRadian);

AX_API AX_INLINE
UMat3
AX_Math_Mat3_MakeScale
(UVec2 *scale);

AX_API AX_INLINE
UVec3
AX_Math_Mat3_GetRow
(UMat3 *mat, Int32 row);

AX_API AX_INLINE
UVec3
AX_Math_Mat3_GetColumn
(UMat3 *mat, Int32 column);

AX_API
Bool
AX_Math_Mat3_SetRow
(UMat3 *mat, Int32 row, UVec3 *data);

AX_API
Bool
AX_Math_Mat3_SetColumn
(UMat3 *mat, Int32 column, UVec3 *data);

AX_API AX_INLINE
Bool
AX_Math_Mat3_Equals
(UMat3 *left, UMat3 *right);

AX_API AX_INLINE
Bool
AX_Math_Mat3_ApproxEquals
(UMat3 *left, UMat3 *right, Float limit);

AX_API AX_INLINE
Bool
AX_Math_Mat3_IsZero
(UMat3 *mat);

AX_API AX_INLINE
Bool
AX_Math_Mat3_IsIdentity
(UMat3 *mat);

