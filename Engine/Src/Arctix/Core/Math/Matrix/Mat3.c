#include "Arctix/Core/Math/Matrix/Mat3.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Math/Helper/MathHelper.h"
#include "Arctix/Core/Math/Vector/Vec2.h"
#include "Arctix/Core/Math/Vector/Vec3.h"


AX_API AX_INLINE
UMat3
AX_Math_Mat3_Construct
(
	Float x1y1, Float x2y1, Float x3y1,
	Float x1y2, Float x2y2, Float x3y2,
	Float x1y3, Float x2y3, Float x3y3
)
{
	return AX_STRUCT(
		UMat3,
		.elements[0] = x1y1, .elements[1] = x2y1, .elements[2] = x3y1,
		.elements[3] = x1y2, .elements[4] = x2y2, .elements[5] = x3y2,
		.elements[6] = x1y3, .elements[7] = x2y3, .elements[8] = x3y3
	);
}

AX_API AX_INLINE
UMat3
AX_Math_Mat3_Zero
(void)
{
	return AX_STRUCT(UMat3, 0.0f);
}

AX_API AX_INLINE
UMat3
AX_Math_Mat3_Identity
(void)
{
	return AX_STRUCT(
		UMat3,
		.elements[0] = 1.0f, .elements[1] = 0.0f, .elements[2] = 0.0f,
		.elements[3] = 0.0f, .elements[4] = 1.0f, .elements[5] = 0.0f,
		.elements[6] = 0.0f, .elements[7] = 0.0f, .elements[8] = 1.0f
	);
}

AX_API AX_INLINE
UMat3
AX_Math_Mat3_Copy
(UMat3 *mat)
{
	return (!mat) ? AX_Math_Mat3_Identity() : (*mat);
}

AX_API AX_INLINE
UMat3
AX_Math_Mat3_FromVectors
(UVec2 *vec1, UVec2 *vec2)
{
	return
		(!vec1 || !vec2) ?
		AX_Math_Mat3_Identity() :
		AX_STRUCT(
			UMat3,
			.elements[0] = vec1->x * vec2->x,
			.elements[1] = vec1->x * vec2->y,
			.elements[2] = 0.0f,

			.elements[3] = vec1->y * vec2->x,
			.elements[4] = vec1->y * vec2->y,
			.elements[5] = 0.0f,

			.elements[6] = 0.0f,
			.elements[7] = 0.0f,
			.elements[8] = 1.0f
		);
}

AX_API
UMat3
AX_Math_Mat3_Add
(UMat3 *left, UMat3 *right)
{
	return
		(!left || !right) ?
		AX_Math_Mat3_Identity() :
		AX_STRUCT(
			UMat3,
			.elements[0] = left->elements[0] + right->elements[0],
			.elements[1] = left->elements[1] + right->elements[1],
			.elements[2] = left->elements[2] + right->elements[2],

			.elements[3] = left->elements[3] + right->elements[3],
			.elements[4] = left->elements[4] + right->elements[4],
			.elements[5] = left->elements[5] + right->elements[5],

			.elements[6] = left->elements[6] + right->elements[6],
			.elements[7] = left->elements[7] + right->elements[7],
			.elements[8] = left->elements[8] + right->elements[8]
		);
}

AX_API
UMat3
AX_Math_Mat3_Sub
(UMat3 *left, UMat3 *right)
{
	return
		(!left || !right) ?
		AX_Math_Mat3_Identity() :
		AX_STRUCT(
			UMat3,
			.elements[0] = left->elements[0] - right->elements[0],
			.elements[1] = left->elements[1] - right->elements[1],
			.elements[2] = left->elements[2] - right->elements[2],

			.elements[3] = left->elements[3] - right->elements[3],
			.elements[4] = left->elements[4] - right->elements[4],
			.elements[5] = left->elements[5] - right->elements[5],

			.elements[6] = left->elements[6] - right->elements[6],
			.elements[7] = left->elements[7] - right->elements[7],
			.elements[8] = left->elements[8] - right->elements[8]
		);
}

AX_API AX_INLINE
UMat3
AX_Math_Mat3_Mul
(UMat3 *left, UMat3 *right)
{
	return AX_STRUCT(
		UMat3,
		.elements[0] = (left->elements[0] * right->elements[0]) + (left->elements[1] * right->elements[3]) + (left->elements[2] * right->elements[6]),
		.elements[1] = (left->elements[0] * right->elements[1]) + (left->elements[1] * right->elements[4]) + (left->elements[2] * right->elements[7]),
		.elements[2] = (left->elements[0] * right->elements[2]) + (left->elements[1] * right->elements[5]) + (left->elements[2] * right->elements[8]),
		.elements[3] = (left->elements[3] * right->elements[0]) + (left->elements[4] * right->elements[3]) + (left->elements[5] * right->elements[6]),
		.elements[4] = (left->elements[3] * right->elements[1]) + (left->elements[4] * right->elements[4]) + (left->elements[5] * right->elements[7]),
		.elements[5] = (left->elements[3] * right->elements[2]) + (left->elements[4] * right->elements[5]) + (left->elements[5] * right->elements[8]),
		.elements[6] = (left->elements[6] * right->elements[0]) + (left->elements[7] * right->elements[3]) + (left->elements[8] * right->elements[6]),
		.elements[7] = (left->elements[6] * right->elements[1]) + (left->elements[7] * right->elements[4]) + (left->elements[8] * right->elements[7]),
		.elements[8] = (left->elements[6] * right->elements[2]) + (left->elements[7] * right->elements[5]) + (left->elements[8] * right->elements[8])
	);
}

AX_API AX_INLINE
UMat3
AX_Math_Mat3_MulScalar
(UMat3 *mat, Float scalar)
{
	return
		(!mat) ?
		AX_Math_Mat3_Identity() :
		AX_STRUCT(
			UMat3,
			.elements[0] = mat->elements[0] * scalar,
			.elements[1] = mat->elements[1] * scalar,
			.elements[2] = mat->elements[2] * scalar,

			.elements[3] = mat->elements[3] * scalar,
			.elements[4] = mat->elements[4] * scalar,
			.elements[5] = mat->elements[5] * scalar,

			.elements[6] = mat->elements[6] * scalar,
			.elements[7] = mat->elements[7] * scalar,
			.elements[8] = mat->elements[8] * scalar
		);
}

AX_API
UVec2
AX_Math_Mat3_Mat3MulVec2
(UMat3 *mat, UVec2 *vec)
{
	return
		(!mat || !vec) ?
		AX_Math_Vec2_Zero() :
		AX_STRUCT(
			UVec2,
			(mat->elements[0] * vec->x) + (mat->elements[1] * vec->y),
			(mat->elements[3] * vec->x) + (mat->elements[4] * vec->y)
		);
}

AX_API
UVec2
AX_Math_Mat3_Vec2MulMat3
(UVec2 *vec, UMat3 *mat)
{
	return
		(!mat || !vec) ?
		AX_Math_Vec2_Zero() :
		AX_STRUCT(
			UVec2,
			((vec->x * mat->elements[0]) + (vec->y * mat->elements[3]) + mat->elements[6]),
			((vec->x * mat->elements[1]) + (vec->y * mat->elements[4]) + mat->elements[6])
		);
}

AX_API
Bool
AX_Math_Mat3_Transpose
(UMat3 *mat)
{
	if (!mat)
		return false;

	AX_HAL_Memory_Memswap(&(mat->elements[1]), &(mat->elements[3]), sizeof(Float));
	AX_HAL_Memory_Memswap(&(mat->elements[2]), &(mat->elements[6]), sizeof(Float));
	AX_HAL_Memory_Memswap(&(mat->elements[5]), &(mat->elements[7]), sizeof(Float));

	return true;
}

AX_API AX_INLINE
UMat3
AX_Math_Mat3_Transposed
(UMat3 *mat)
{
	return AX_STRUCT(
		UMat3,
		.elements[0] = mat->elements[0],
		.elements[1] = mat->elements[3],
		.elements[2] = mat->elements[6],

		.elements[3] = mat->elements[1],
		.elements[4] = mat->elements[4],
		.elements[5] = mat->elements[7],

		.elements[6] = mat->elements[2],
		.elements[7] = mat->elements[5],
		.elements[8] = mat->elements[8]
	);
}

AX_API AX_INLINE
Float
AX_Math_Mat3_Determinant
(UMat3 *mat)
{
	return (!mat) ?
		0.0f :
		(mat->elements[0] * ((mat->elements[4] * mat->elements[8]) - (mat->elements[7] * mat->elements[5]))) -
		(mat->elements[1] * ((mat->elements[3] * mat->elements[8]) - (mat->elements[6] * mat->elements[5]))) +
		(mat->elements[2] * ((mat->elements[3] * mat->elements[7]) - (mat->elements[6] * mat->elements[4])));
}

AX_API
Bool
AX_Math_Mat3_Inverse
(UMat3 *mat)
{
	if (!mat || (AX_Math_Mat3_Determinant(mat) == 0))
		return false;

	Float det = AX_Math_Mat3_Determinant(mat);
	UMat3 transposed = AX_Math_Mat3_Transposed(mat);

	UMat3 adjMat = AX_STRUCT(
		UMat3,
		.elements[0] = transposed.elements[4] * transposed.elements[8] - transposed.elements[5] * transposed.elements[7],
		.elements[1] = -transposed.elements[3] * transposed.elements[8] + transposed.elements[5] * transposed.elements[6],
		.elements[2] = transposed.elements[3] * transposed.elements[7] - transposed.elements[4] * transposed.elements[6],
		.elements[3] = -transposed.elements[1] * transposed.elements[8] + transposed.elements[2] * transposed.elements[7],
		.elements[4] = transposed.elements[0] * transposed.elements[8] - transposed.elements[2] * transposed.elements[6],
		.elements[5] = -transposed.elements[0] * transposed.elements[7] + transposed.elements[1] * transposed.elements[6],
		.elements[6] = transposed.elements[1] * transposed.elements[5] - transposed.elements[2] * transposed.elements[4],
		.elements[7] = -transposed.elements[0] * transposed.elements[5] + transposed.elements[2] * transposed.elements[3],
		.elements[8] = transposed.elements[0] * transposed.elements[4] - transposed.elements[1] * transposed.elements[3]
	);

	*mat = AX_Math_Mat3_MulScalar(&adjMat, 1.0f / det);
	return true;
}

AX_API AX_INLINE
UMat3
AX_Math_Mat3_Inversed
(UMat3 *mat)
{
	AX_Math_Mat3_Inverse(mat);
	return *mat;
}

AX_API AX_INLINE
UMat3
AX_Math_Mat3_MakeTranslation
(UVec2 *position)
{
	return
		(!position) ?
		AX_Math_Mat3_Identity() :
		AX_STRUCT(
			UMat3,
			.elements[0] = 1.0f,
			.elements[1] = 0.0f,
			.elements[2] = 0.0f,
			.elements[3] = 0.0f,
			.elements[4] = 1.0f,
			.elements[5] = 0.0f,
			.elements[6] = position->x,
			.elements[7] = position->y,
			.elements[8] = 1.0f
		);
}

AX_API AX_INLINE
UMat3
AX_Math_Mat3_MakeRotation
(Float angleRadian)
{
	return AX_STRUCT(
		UMat3,
		.elements[0] = AX_Math_Cos(angleRadian),
		.elements[1] = AX_Math_Sin(angleRadian),
		.elements[2] = 0.0f,
		.elements[3] = -AX_Math_Sin(angleRadian),
		.elements[4] = AX_Math_Cos(angleRadian),
		.elements[5] = 0.0f,
		.elements[6] = 0.0f,
		.elements[7] = 0.0f,
		.elements[8] = 1.0f
	);
}

AX_API AX_INLINE
UMat3
AX_Math_Mat3_MakeScale
(UVec2 *scale)
{
	return
		(!scale) ?
		AX_Math_Mat3_Identity() :
		AX_STRUCT(
			UMat3,
			.elements[0] = scale->x,
			.elements[1] = 0.0f,
			.elements[2] = 0.0f,
			.elements[3] = 0.0f,
			.elements[4] = scale->y,
			.elements[5] = 0.0f,
			.elements[6] = 0.0f,
			.elements[7] = 0.0f,
			.elements[8] = 1.0f
		);
}

AX_API AX_INLINE
UVec3
AX_Math_Mat3_GetRow
(UMat3 *mat, Int32 row)
{
	return
		(!mat || ((row < 0) || (row > 2))) ?
		AX_Math_Vec3_Zero() :
		AX_STRUCT(
			UVec3,
			mat->elements[(row * 3) + 0],
			mat->elements[(row * 3) + 1],
			mat->elements[(row * 3) + 2]
		);
}

AX_API AX_INLINE
UVec3
AX_Math_Mat3_GetColumn
(UMat3 *mat, Int32 column)
{
	return
		(!mat || ((column < 0) || (column > 2))) ?
		AX_Math_Vec3_Zero() :
		AX_STRUCT(
			UVec3,
			mat->elements[column + 0],
			mat->elements[column + 3],
			mat->elements[column + 6]
		);
}

AX_API
Bool
AX_Math_Mat3_SetRow
(UMat3 *mat, Int32 row, UVec3 *data)
{
	if (!mat || !data || ((row < 0) || (row > 2)))
		return false;

	AX_HAL_Memory_Memcpy(&mat->elements[row * 3], data, sizeof(UVec3));
	return true;
}

AX_API
Bool
AX_Math_Mat3_SetColumn
(UMat3 *mat, Int32 column, UVec3 *data)
{
	if (!mat || !data || ((column < 0) || (column > 2)))
		return false;

	mat->elements[column + 0] = data->x;
	mat->elements[column + 3] = data->y;
	mat->elements[column + 6] = data->z;

	return true;
}

AX_API AX_INLINE
Bool
AX_Math_Mat3_Equals
(UMat3 *left, UMat3 *right)
{
	return
		(!left || !right) ?
		false :
		((left->elements[0] - right->elements[0]) < AX_MATH_EPSILON) &&
		((left->elements[1] - right->elements[1]) < AX_MATH_EPSILON) &&
		((left->elements[2] - right->elements[2]) < AX_MATH_EPSILON) &&
		((left->elements[3] - right->elements[3]) < AX_MATH_EPSILON) &&
		((left->elements[4] - right->elements[4]) < AX_MATH_EPSILON) &&
		((left->elements[5] - right->elements[5]) < AX_MATH_EPSILON) &&
		((left->elements[6] - right->elements[6]) < AX_MATH_EPSILON) &&
		((left->elements[7] - right->elements[7]) < AX_MATH_EPSILON) &&
		((left->elements[8] - right->elements[8]) < AX_MATH_EPSILON);
}

AX_API AX_INLINE
Bool
AX_Math_Mat3_ApproxEquals
(UMat3 *left, UMat3 *right, Float limit)
{
	return
		(!left || !right) ?
		false :
		((left->elements[0] - right->elements[0]) < limit) &&
		((left->elements[1] - right->elements[1]) < limit) &&
		((left->elements[2] - right->elements[2]) < limit) &&
		((left->elements[3] - right->elements[3]) < limit) &&
		((left->elements[4] - right->elements[4]) < limit) &&
		((left->elements[5] - right->elements[5]) < limit) &&
		((left->elements[6] - right->elements[6]) < limit) &&
		((left->elements[7] - right->elements[7]) < limit) &&
		((left->elements[8] - right->elements[8]) < limit);
}

AX_API AX_INLINE
Bool
AX_Math_Mat3_IsZero
(UMat3 *mat)
{
	return
		(!mat) ?
		false :
		(mat->elements[0] == 0.0f) &&
		(mat->elements[1] == 0.0f) &&
		(mat->elements[2] == 0.0f) &&
		(mat->elements[3] == 0.0f) &&
		(mat->elements[4] == 0.0f) &&
		(mat->elements[5] == 0.0f) &&
		(mat->elements[6] == 0.0f) &&
		(mat->elements[7] == 0.0f) &&
		(mat->elements[8] == 0.0f);
}

AX_API AX_INLINE
Bool
AX_Math_Mat3_IsIdentity
(UMat3 *mat)
{
	return
		(!mat) ?
		false :
		(mat->elements[0] == 1.0f) &&
		(mat->elements[1] == 0.0f) &&
		(mat->elements[2] == 0.0f) &&
		(mat->elements[3] == 0.0f) &&
		(mat->elements[4] == 1.0f) &&
		(mat->elements[5] == 0.0f) &&
		(mat->elements[6] == 0.0f) &&
		(mat->elements[7] == 0.0f) &&
		(mat->elements[8] == 1.0f);
}


