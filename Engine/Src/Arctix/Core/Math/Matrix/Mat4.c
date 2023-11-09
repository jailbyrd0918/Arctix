#include "Arctix/Core/Math/Matrix/Mat4.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"

#include "Arctix/Core/Math/Helper/MathHelper.h"
#include "Arctix/Core/Math/Vector/Vec3.h"
#include "Arctix/Core/Math/Vector/Vec4.h"
#include "Arctix/Core/Math/Matrix/Mat3.h"


AX_API AX_INLINE
UMat4
AX_Math_Mat4_Construct
(
	Float x1y1, Float x2y1, Float x3y1, Float x4y1,
	Float x1y2, Float x2y2, Float x3y2, Float x4y2,
	Float x1y3, Float x2y3, Float x3y3, Float x4y3,
	Float x1y4, Float x2y4, Float x3y4, Float x4y4
)
{
	return AX_STRUCT(
		UMat4,
		.elements[0] = x1y1, .elements[1] = x2y1, .elements[2] = x3y1, .elements[3] = x4y1,
		.elements[4] = x1y2, .elements[5] = x2y2, .elements[6] = x3y2, .elements[7] = x4y2,
		.elements[8] = x1y3, .elements[9] = x2y3, .elements[10] = x3y3, .elements[11] = x4y3,
		.elements[12] = x1y4, .elements[13] = x2y4, .elements[14] = x3y4, .elements[15] = x4y4
	);
}

AX_API AX_INLINE
UMat4
AX_Math_Mat4_Zero
(void)
{
	return AX_STRUCT(UMat4, 0.0f);
}

AX_API AX_INLINE
UMat4
AX_Math_Mat4_Identity
(void)
{
	return AX_STRUCT(
		UMat4,
		.elements[0] = 1.0f, .elements[1] = 0.0f, .elements[2] = 0.0f, .elements[3] = 0.0f,
		.elements[4] = 0.0f, .elements[5] = 1.0f, .elements[6] = 0.0f, .elements[7] = 0.0f,
		.elements[8] = 0.0f, .elements[9] = 0.0f, .elements[10] = 1.0f, .elements[11] = 0.0f,
		.elements[12] = 0.0f, .elements[13] = 0.0f, .elements[14] = 0.0f, .elements[15] = 1.0f
	);
}

AX_API AX_INLINE
UMat4
AX_Math_Mat4_Copy
(UMat4 *mat)
{
	return (!mat) ? AX_Math_Mat4_Identity() : (*mat);
}

AX_API AX_INLINE
UMat4
AX_Math_Mat4_FromVectors
(UVec3 *vec1, UVec3 *vec2)
{
	return AX_STRUCT(
		UMat4,
		.elements[0] = vec1->x * vec2->x,
		.elements[1] = vec1->x * vec2->y,
		.elements[2] = vec1->x * vec2->z,
		.elements[3] = 0.0f,

		.elements[4] = vec1->y * vec2->x,
		.elements[5] = vec1->y * vec2->y,
		.elements[6] = vec1->y * vec2->z,
		.elements[7] = 0.0f,

		.elements[8] = vec1->z * vec2->x,
		.elements[9] = vec1->z * vec2->y,
		.elements[10] = vec1->z * vec2->z,
		.elements[11] = 0.0f,

		.elements[12] = 0.0f,
		.elements[13] = 0.0f,
		.elements[14] = 0.0f,
		.elements[15] = 1.0f
	);
}

AX_API
UMat4
AX_Math_Mat4_Add
(UMat4 *left, UMat4 *right)
{
	return
		(!left || !right) ?
		AX_Math_Mat4_Identity() :
		AX_STRUCT(
			UMat4,
			.elements[0] = left->elements[0] + right->elements[0],
			.elements[1] = left->elements[1] + right->elements[1],
			.elements[2] = left->elements[2] + right->elements[2],
			.elements[3] = left->elements[3] + right->elements[3],

			.elements[4] = left->elements[4] + right->elements[4],
			.elements[5] = left->elements[5] + right->elements[5],
			.elements[6] = left->elements[6] + right->elements[6],
			.elements[7] = left->elements[7] + right->elements[7],

			.elements[8] = left->elements[8] + right->elements[8],
			.elements[9] = left->elements[9] + right->elements[9],
			.elements[10] = left->elements[10] + right->elements[10],
			.elements[11] = left->elements[11] + right->elements[11],

			.elements[12] = left->elements[12] + right->elements[12],
			.elements[13] = left->elements[13] + right->elements[13],
			.elements[14] = left->elements[14] + right->elements[14],
			.elements[15] = left->elements[15] + right->elements[15]
		);
}

AX_API AX_INLINE
UMat4
AX_Math_Mat4_Sub
(UMat4 *left, UMat4 *right)
{
	return
		(!left || !right) ?
		AX_Math_Mat4_Identity() :
		AX_STRUCT(
			UMat4,
			.elements[0] = left->elements[0] - right->elements[0],
			.elements[1] = left->elements[1] - right->elements[1],
			.elements[2] = left->elements[2] - right->elements[2],
			.elements[3] = left->elements[3] - right->elements[3],

			.elements[4] = left->elements[4] - right->elements[4],
			.elements[5] = left->elements[5] - right->elements[5],
			.elements[6] = left->elements[6] - right->elements[6],
			.elements[7] = left->elements[7] - right->elements[7],

			.elements[8] = left->elements[8] - right->elements[8],
			.elements[9] = left->elements[9] - right->elements[9],
			.elements[10] = left->elements[10] - right->elements[10],
			.elements[11] = left->elements[11] - right->elements[11],

			.elements[12] = left->elements[12] - right->elements[12],
			.elements[13] = left->elements[13] - right->elements[13],
			.elements[14] = left->elements[14] - right->elements[14],
			.elements[15] = left->elements[15] - right->elements[15]
		);
}

AX_API AX_INLINE
UMat4
AX_Math_Mat4_Mul
(UMat4 *left, UMat4 *right)
{
	return
		(!left || !right) ?
		AX_Math_Mat4_Identity() :
		AX_STRUCT(
			UMat4,
			.elements[0] = (left->elements[0] * right->elements[0]) + (left->elements[1] * right->elements[4]) + (left->elements[2] * right->elements[8]) + (left->elements[3] * right->elements[12]),
			.elements[1] = (left->elements[0] * right->elements[1]) + (left->elements[1] * right->elements[5]) + (left->elements[2] * right->elements[9]) + (left->elements[3] * right->elements[13]),
			.elements[2] = (left->elements[0] * right->elements[2]) + (left->elements[1] * right->elements[6]) + (left->elements[2] * right->elements[10]) + (left->elements[3] * right->elements[14]),
			.elements[3] = (left->elements[0] * right->elements[3]) + (left->elements[1] * right->elements[7]) + (left->elements[2] * right->elements[11]) + (left->elements[3] * right->elements[15]),

			.elements[4] = (left->elements[4] * right->elements[0]) + (left->elements[5] * right->elements[4]) + (left->elements[6] * right->elements[8]) + (left->elements[7] * right->elements[12]),
			.elements[5] = (left->elements[4] * right->elements[1]) + (left->elements[5] * right->elements[5]) + (left->elements[6] * right->elements[9]) + (left->elements[7] * right->elements[13]),
			.elements[6] = (left->elements[4] * right->elements[2]) + (left->elements[5] * right->elements[6]) + (left->elements[6] * right->elements[10]) + (left->elements[7] * right->elements[14]),
			.elements[7] = (left->elements[4] * right->elements[3]) + (left->elements[5] * right->elements[7]) + (left->elements[6] * right->elements[11]) + (left->elements[7] * right->elements[15]),

			.elements[8] = (left->elements[8] * right->elements[0]) + (left->elements[9] * right->elements[4]) + (left->elements[10] * right->elements[8]) + (left->elements[11] * right->elements[12]),
			.elements[9] = (left->elements[8] * right->elements[1]) + (left->elements[9] * right->elements[5]) + (left->elements[10] * right->elements[9]) + (left->elements[11] * right->elements[13]),
			.elements[10] = (left->elements[8] * right->elements[2]) + (left->elements[9] * right->elements[6]) + (left->elements[10] * right->elements[10]) + (left->elements[11] * right->elements[14]),
			.elements[11] = (left->elements[8] * right->elements[3]) + (left->elements[9] * right->elements[7]) + (left->elements[10] * right->elements[11]) + (left->elements[11] * right->elements[15]),

			.elements[12] = (left->elements[12] * right->elements[0]) + (left->elements[13] * right->elements[4]) + (left->elements[14] * right->elements[8]) + (left->elements[15] * right->elements[12]),
			.elements[13] = (left->elements[12] * right->elements[1]) + (left->elements[13] * right->elements[5]) + (left->elements[14] * right->elements[9]) + (left->elements[15] * right->elements[13]),
			.elements[14] = (left->elements[12] * right->elements[2]) + (left->elements[13] * right->elements[6]) + (left->elements[14] * right->elements[10]) + (left->elements[15] * right->elements[14]),
			.elements[15] = (left->elements[12] * right->elements[3]) + (left->elements[13] * right->elements[7]) + (left->elements[14] * right->elements[11]) + (left->elements[15] * right->elements[15])
		);
}

AX_API AX_INLINE
UMat4
AX_Math_Mat4_MulScalar
(UMat4 *mat, Float scalar)
{
	return
		(!mat) ?
		AX_Math_Mat4_Identity() :
		AX_STRUCT(
			UMat4,
			.elements[0] = mat->elements[0] * scalar,
			.elements[1] = mat->elements[1] * scalar,
			.elements[2] = mat->elements[2] * scalar,
			.elements[3] = mat->elements[3] * scalar,

			.elements[4] = mat->elements[4] * scalar,
			.elements[5] = mat->elements[5] * scalar,
			.elements[6] = mat->elements[6] * scalar,
			.elements[7] = mat->elements[7] * scalar,

			.elements[8] = mat->elements[8] * scalar,
			.elements[9] = mat->elements[9] * scalar,
			.elements[10] = mat->elements[10] * scalar,
			.elements[11] = mat->elements[11] * scalar,

			.elements[12] = mat->elements[12] * scalar,
			.elements[13] = mat->elements[13] * scalar,
			.elements[14] = mat->elements[14] * scalar,
			.elements[15] = mat->elements[15] * scalar
		);
}

AX_API AX_INLINE
UMat4
AX_Math_Mat4_Orthographic
(Float top, Float bottom, Float left, Float right, Float near, Float far)
{
	return AX_STRUCT(
		UMat4,
		.elements[0] = (2.0f / (right - left)),
		.elements[1] = 0.0f,
		.elements[2] = 0.0f,
		.elements[3] = 0.0f,

		.elements[4] = 0.0f,
		.elements[5] = (2.0f / (top - bottom)),
		.elements[6] = 0.0f,
		.elements[7] = 0.0f,

		.elements[8] = 0.0f,
		.elements[9] = 0.0f,
		.elements[10] = (-2.0f / (far - near)),
		.elements[11] = 0.0f,

		.elements[12] = (((left + right) * -1.0f) / (right - left)),
		.elements[13] = (((top + bottom) * -1.0f) / (top - bottom)),
		.elements[14] = (((near + far) * -1.0f) / (far - near)),
		.elements[15] = 1.0f
	);
}

AX_API AX_INLINE
UMat4
AX_Math_Mat4_Perspective
(Float fovRadian, Float ratio, Float near, Float far)
{
	return AX_STRUCT(
		UMat4,
		.elements[0] = (1.0f / (ratio * AX_Math_Tan(fovRadian * 0.5f))),
		.elements[1] = 0.0f,
		.elements[2] = 0.0f,
		.elements[3] = 0.0f,

		.elements[4] = 0.0f,
		.elements[5] = (1.0f / AX_Math_Tan(fovRadian * 0.5f)),
		.elements[6] = 0.0f,
		.elements[7] = 0.0f,

		.elements[8] = 0.0f,
		.elements[9] = 0.0f,
		.elements[10] = ((near + far) / (near - far)),
		.elements[11] = -1.0f,

		.elements[12] = 0.0f,
		.elements[13] = 0.0f,
		.elements[14] = ((2.0f * far * near) / (near - far)),
		.elements[15] = 0.0f
	);
}

AX_API AX_INLINE
UMat4
AX_Math_Mat4_LookAt
(UVec3 *position, UVec3 *target, UVec3 *up)
{
	#define Z_AXIS	AX_Math_Vec3_Normalized(AX_Math_Vec3_Sub(*position, *target))
	#define	X_AXIS	AX_Math_Vec3_Normalized(AX_Math_Vec3_Cross(*up, Z_AXIS))
	#define	Y_AXIS	AX_Math_Vec3_Cross(Z_AXIS, X_AXIS)

	return
		(!position || !target || !up) ?
		AX_Math_Mat4_Identity() :
		AX_STRUCT(
			UMat4,
			.elements[0] = X_AXIS.x,
			.elements[1] = X_AXIS.y,
			.elements[2] = X_AXIS.z,
			.elements[3] = 0.0f,

			.elements[4] = Y_AXIS.x,
			.elements[5] = Y_AXIS.y,
			.elements[6] = Y_AXIS.z,
			.elements[7] = 0.0f,

			.elements[8] = (Z_AXIS.x * -1.0f),
			.elements[9] = (Z_AXIS.y * -1.0f),
			.elements[10] = (Z_AXIS.z * -1.0f),
			.elements[11] = 0.0f,

			.elements[12] = position->x,
			.elements[13] = position->y,
			.elements[14] = position->z,
			.elements[15] = 1.0f
		);
}

AX_API
Bool
AX_Math_Mat4_Transpose
(UMat4 *mat)
{
	if (!mat)
		return false;

	AX_HAL_Memory_Memswap(&(mat->elements[1]), &(mat->elements[4]), sizeof(Float));
	AX_HAL_Memory_Memswap(&(mat->elements[2]), &(mat->elements[8]), sizeof(Float));
	AX_HAL_Memory_Memswap(&(mat->elements[3]), &(mat->elements[12]), sizeof(Float));
	AX_HAL_Memory_Memswap(&(mat->elements[6]), &(mat->elements[9]), sizeof(Float));
	AX_HAL_Memory_Memswap(&(mat->elements[7]), &(mat->elements[13]), sizeof(Float));
	AX_HAL_Memory_Memswap(&(mat->elements[11]), &(mat->elements[14]), sizeof(Float));

	return true;
}

AX_API AX_INLINE
UMat4
AX_Math_Mat4_Transposed
(UMat4 *mat)
{
	return (!mat) ?
		AX_Math_Mat4_Identity() :
		AX_STRUCT(
			UMat4,
			.elements[0] = mat->elements[0],
			.elements[1] = mat->elements[4],
			.elements[2] = mat->elements[8],
			.elements[3] = mat->elements[12],

			.elements[4] = mat->elements[1],
			.elements[5] = mat->elements[5],
			.elements[6] = mat->elements[9],
			.elements[7] = mat->elements[13],

			.elements[8] = mat->elements[2],
			.elements[9] = mat->elements[6],
			.elements[10] = mat->elements[10],
			.elements[11] = mat->elements[14],

			.elements[12] = mat->elements[3],
			.elements[13] = mat->elements[7],
			.elements[14] = mat->elements[11],
			.elements[15] = mat->elements[15]
		);
}

AX_API AX_INLINE
Float
AX_Math_Mat4_Determinant
(UMat4 *mat)
{
	return (!mat) ?
		0.0f :
		mat->elements[0] * ((mat->elements[5] * (mat->elements[10] * mat->elements[15] - mat->elements[11] * mat->elements[14])) -
			(mat->elements[6] * (mat->elements[9] * mat->elements[15] - mat->elements[11] * mat->elements[13])) +
			(mat->elements[7] * (mat->elements[9] * mat->elements[14] - mat->elements[10] * mat->elements[13]))) -
		mat->elements[1] * ((mat->elements[4] * (mat->elements[10] * mat->elements[15] - mat->elements[11] * mat->elements[14])) -
			(mat->elements[6] * (mat->elements[8] * mat->elements[15] - mat->elements[11] * mat->elements[12])) +
			(mat->elements[7] * (mat->elements[8] * mat->elements[14] - mat->elements[10] * mat->elements[12]))) +
		mat->elements[2] * ((mat->elements[4] * (mat->elements[9] * mat->elements[15] - mat->elements[11] * mat->elements[13])) -
			(mat->elements[5] * (mat->elements[8] * mat->elements[15] - mat->elements[11] * mat->elements[12])) +
			(mat->elements[7] * (mat->elements[8] * mat->elements[13] - mat->elements[9] * mat->elements[12]))) -
		mat->elements[3] * ((mat->elements[4] * (mat->elements[9] * mat->elements[14] - mat->elements[10] * mat->elements[13])) -
			(mat->elements[5] * (mat->elements[8] * mat->elements[14] - mat->elements[10] * mat->elements[12])) +
			(mat->elements[6] * (mat->elements[8] * mat->elements[13] - mat->elements[9] * mat->elements[12])));
}

AX_API
Bool
AX_Math_Mat4_Inverse
(UMat4 *mat)
{
	if (!mat || (AX_Math_Mat4_Determinant(mat) == 0))
		return false;

	UMat4 adjMat = AX_STRUCT(
		UMat4,
		.elements[0] = (mat->elements[5] * mat->elements[10] * mat->elements[15] - mat->elements[5] * mat->elements[11] * mat->elements[14] -
			mat->elements[9] * mat->elements[6] * mat->elements[15] + mat->elements[9] * mat->elements[7] * mat->elements[14] +
			mat->elements[13] * mat->elements[6] * mat->elements[11] - mat->elements[13] * mat->elements[7] * mat->elements[10]),

		.elements[1] = (-mat->elements[1] * mat->elements[10] * mat->elements[15] + mat->elements[1] * mat->elements[11] * mat->elements[14] +
			mat->elements[9] * mat->elements[2] * mat->elements[15] - mat->elements[9] * mat->elements[3] * mat->elements[14] -
			mat->elements[13] * mat->elements[2] * mat->elements[11] + mat->elements[13] * mat->elements[3] * mat->elements[10]),

		.elements[2] = (mat->elements[1] * mat->elements[6] * mat->elements[15] - mat->elements[1] * mat->elements[7] * mat->elements[14] -
			mat->elements[5] * mat->elements[2] * mat->elements[15] + mat->elements[5] * mat->elements[3] * mat->elements[14] +
			mat->elements[13] * mat->elements[2] * mat->elements[7] - mat->elements[13] * mat->elements[3] * mat->elements[6]),

		.elements[3] = (-mat->elements[1] * mat->elements[6] * mat->elements[11] + mat->elements[1] * mat->elements[7] * mat->elements[10] +
			mat->elements[5] * mat->elements[2] * mat->elements[11] - mat->elements[5] * mat->elements[3] * mat->elements[10] -
			mat->elements[9] * mat->elements[2] * mat->elements[7] + mat->elements[9] * mat->elements[3] * mat->elements[6]),

		.elements[4] = (-mat->elements[4] * mat->elements[10] * mat->elements[15] + mat->elements[4] * mat->elements[11] * mat->elements[14] +
			mat->elements[8] * mat->elements[6] * mat->elements[15] - mat->elements[8] * mat->elements[7] * mat->elements[14] -
			mat->elements[12] * mat->elements[6] * mat->elements[11] + mat->elements[12] * mat->elements[7] * mat->elements[10]),

		.elements[5] = (mat->elements[0] * mat->elements[10] * mat->elements[15] - mat->elements[0] * mat->elements[11] * mat->elements[14] -
			mat->elements[8] * mat->elements[2] * mat->elements[15] + mat->elements[8] * mat->elements[3] * mat->elements[14] +
			mat->elements[12] * mat->elements[2] * mat->elements[11] - mat->elements[12] * mat->elements[3] * mat->elements[10]),

		.elements[6] = (-mat->elements[0] * mat->elements[6] * mat->elements[15] + mat->elements[0] * mat->elements[7] * mat->elements[14] +
			mat->elements[4] * mat->elements[2] * mat->elements[15] - mat->elements[4] * mat->elements[3] * mat->elements[14] -
			mat->elements[12] * mat->elements[2] * mat->elements[7] + mat->elements[12] * mat->elements[3] * mat->elements[6]),

		.elements[7] = (mat->elements[0] * mat->elements[6] * mat->elements[11] - mat->elements[0] * mat->elements[7] * mat->elements[10] -
			mat->elements[4] * mat->elements[2] * mat->elements[11] + mat->elements[4] * mat->elements[3] * mat->elements[10] +
			mat->elements[8] * mat->elements[2] * mat->elements[7] - mat->elements[8] * mat->elements[3] * mat->elements[6]),

		.elements[8] = (mat->elements[4] * mat->elements[9] * mat->elements[15] - mat->elements[4] * mat->elements[11] * mat->elements[13] -
			mat->elements[8] * mat->elements[5] * mat->elements[15] + mat->elements[8] * mat->elements[7] * mat->elements[13] +
			mat->elements[12] * mat->elements[5] * mat->elements[11] - mat->elements[12] * mat->elements[7] * mat->elements[9]),

		.elements[9] = (-mat->elements[0] * mat->elements[9] * mat->elements[15] + mat->elements[0] * mat->elements[11] * mat->elements[13] +
			mat->elements[8] * mat->elements[1] * mat->elements[15] - mat->elements[8] * mat->elements[3] * mat->elements[13] -
			mat->elements[12] * mat->elements[1] * mat->elements[11] + mat->elements[12] * mat->elements[3] * mat->elements[9]),

		.elements[10] = (mat->elements[0] * mat->elements[5] * mat->elements[15] - mat->elements[0] * mat->elements[7] * mat->elements[13] -
			mat->elements[4] * mat->elements[1] * mat->elements[15] + mat->elements[4] * mat->elements[3] * mat->elements[13] +
			mat->elements[12] * mat->elements[1] * mat->elements[7] - mat->elements[12] * mat->elements[3] * mat->elements[5]),

		.elements[11] = (-mat->elements[0] * mat->elements[5] * mat->elements[11] + mat->elements[0] * mat->elements[7] * mat->elements[9] +
			mat->elements[4] * mat->elements[1] * mat->elements[11] - mat->elements[4] * mat->elements[3] * mat->elements[9] -
			mat->elements[8] * mat->elements[1] * mat->elements[7] + mat->elements[8] * mat->elements[3] * mat->elements[5]),

		.elements[12] = (-mat->elements[4] * mat->elements[9] * mat->elements[14] + mat->elements[4] * mat->elements[10] * mat->elements[13] +
			mat->elements[8] * mat->elements[5] * mat->elements[14] - mat->elements[8] * mat->elements[6] * mat->elements[13] -
			mat->elements[12] * mat->elements[5] * mat->elements[10] + mat->elements[12] * mat->elements[6] * mat->elements[9]),

		.elements[13] = (mat->elements[0] * mat->elements[9] * mat->elements[14] - mat->elements[0] * mat->elements[10] * mat->elements[13] -
			mat->elements[8] * mat->elements[1] * mat->elements[14] + mat->elements[8] * mat->elements[2] * mat->elements[13] +
			mat->elements[12] * mat->elements[1] * mat->elements[10] - mat->elements[12] * mat->elements[2] * mat->elements[9]),

		.elements[14] = (-mat->elements[0] * mat->elements[5] * mat->elements[14] + mat->elements[0] * mat->elements[6] * mat->elements[13] +
			mat->elements[4] * mat->elements[1] * mat->elements[14] - mat->elements[4] * mat->elements[2] * mat->elements[13] -
			mat->elements[12] * mat->elements[1] * mat->elements[6] + mat->elements[12] * mat->elements[2] * mat->elements[5]),

		.elements[15] = (mat->elements[0] * mat->elements[5] * mat->elements[10] - mat->elements[0] * mat->elements[6] * mat->elements[9] -
			mat->elements[4] * mat->elements[1] * mat->elements[10] + mat->elements[4] * mat->elements[2] * mat->elements[9] +
			mat->elements[8] * mat->elements[1] * mat->elements[6] - mat->elements[8] * mat->elements[2] * mat->elements[5])
	);

	*mat = AX_Math_Mat4_MulScalar(&adjMat, 1.0f / AX_Math_Mat4_Determinant(mat));
	return true;
}

AX_API AX_INLINE
UMat4
AX_Math_Mat4_Inversed
(UMat4 *mat)
{
	return
		(!mat || (AX_Math_Mat4_Determinant(mat) == 0)) ?
		AX_Math_Mat4_Identity() :
		AX_STRUCT(
			UMat4,
			.elements[0] = (mat->elements[5] * mat->elements[10] - mat->elements[6] * mat->elements[9]) * (1.0f / AX_Math_Mat4_Determinant(mat)),
			.elements[1] = (mat->elements[2] * mat->elements[9] - mat->elements[1] * mat->elements[10]) * (1.0f / AX_Math_Mat4_Determinant(mat)),
			.elements[2] = (mat->elements[1] * mat->elements[6] - mat->elements[2] * mat->elements[5]) * (1.0f / AX_Math_Mat4_Determinant(mat)),
			.elements[3] = 0.0f,

			.elements[4] = (mat->elements[6] * mat->elements[8] - mat->elements[4] * mat->elements[10]) * (1.0f / AX_Math_Mat4_Determinant(mat)),
			.elements[5] = (mat->elements[0] * mat->elements[10] - mat->elements[2] * mat->elements[8]) * (1.0f / AX_Math_Mat4_Determinant(mat)),
			.elements[6] = (mat->elements[2] * mat->elements[4] - mat->elements[0] * mat->elements[6]) * (1.0f / AX_Math_Mat4_Determinant(mat)),
			.elements[7] = 0.0f,

			.elements[8] = (mat->elements[4] * mat->elements[9] - mat->elements[5] * mat->elements[8]) * (1.0f / AX_Math_Mat4_Determinant(mat)),
			.elements[9] = (mat->elements[1] * mat->elements[8] - mat->elements[0] * mat->elements[9]) * (1.0f / AX_Math_Mat4_Determinant(mat)),
			.elements[10] = (mat->elements[0] * mat->elements[5] - mat->elements[1] * mat->elements[4]) * (1.0f / AX_Math_Mat4_Determinant(mat)),
			.elements[11] = 0.0f,

			.elements[12] = 0.0f,
			.elements[13] = 0.0f,
			.elements[14] = 0.0f,
			.elements[15] = 1.0f
		);
}

AX_API AX_INLINE
UMat4
AX_Math_Mat4_MakeTranslation
(UVec3 *position)
{
	return
		(!position) ?
		AX_Math_Mat4_Identity() :
		AX_STRUCT(
			UMat4,
			.elements[0] = 1.0f,
			.elements[1] = 0.0f,
			.elements[2] = 0.0f,
			.elements[3] = 0.0f,

			.elements[4] = 0.0f,
			.elements[5] = 1.0f,
			.elements[6] = 0.0f,
			.elements[7] = 0.0f,

			.elements[8] = 0.0f,
			.elements[9] = 0.0f,
			.elements[10] = 1.0f,
			.elements[11] = 0.0f,

			.elements[12] = position->x,
			.elements[13] = position->y,
			.elements[14] = position->z,
			.elements[15] = 1.0f
		);
}

AX_API AX_INLINE
UMat4
AX_Math_Mat4_MakeRotationX
(Float angleRadian)
{
	return AX_STRUCT(
		UMat4,
		.elements[0] = 1.0f,
		.elements[1] = 0.0f,
		.elements[2] = 0.0f,
		.elements[3] = 0.0f,

		.elements[4] = 0.0f,
		.elements[5] = AX_Math_Cos(angleRadian),
		.elements[6] = AX_Math_Sin(angleRadian),
		.elements[7] = 0.0f,

		.elements[8] = 0.0f,
		.elements[9] = (AX_Math_Sin(angleRadian) * -1.0f),
		.elements[10] = AX_Math_Cos(angleRadian),
		.elements[11] = 0.0f,

		.elements[12] = 0.0f,
		.elements[13] = 0.0f,
		.elements[14] = 0.0f,
		.elements[15] = 1.0f
	);
}

AX_API AX_INLINE
UMat4
AX_Math_Mat4_MakeRotationY
(Float angleRadian)
{
	return AX_STRUCT(
		UMat4,
		.elements[0] = AX_Math_Cos(angleRadian),
		.elements[1] = 0.0f,
		.elements[2] = (AX_Math_Sin(angleRadian) * -1.0f),
		.elements[3] = 0.0f,

		.elements[4] = 0.0f,
		.elements[5] = 1.0f,
		.elements[6] = 0.0f,
		.elements[7] = 0.0f,

		.elements[8] = AX_Math_Sin(angleRadian),
		.elements[9] = 0.0f,
		.elements[10] = AX_Math_Cos(angleRadian),
		.elements[11] = 0.0f,

		.elements[12] = 0.0f,
		.elements[13] = 0.0f,
		.elements[14] = 0.0f,
		.elements[15] = 1.0f
	);
}

AX_API AX_INLINE
UMat4
AX_Math_Mat4_MakeRotationZ
(Float angleRadian)
{
	return AX_STRUCT(
		UMat4,
		.elements[0] = AX_Math_Cos(angleRadian),
		.elements[1] = AX_Math_Sin(angleRadian),
		.elements[2] = 0.0f,
		.elements[3] = 0.0f,

		.elements[4] = (AX_Math_Sin(angleRadian) * -1.0f),
		.elements[5] = AX_Math_Cos(angleRadian),
		.elements[6] = 0.0f,
		.elements[7] = 0.0f,

		.elements[8] = 0.0f,
		.elements[9] = 0.0f,
		.elements[10] = 1.0f,
		.elements[11] = 0.0f,

		.elements[12] = 0.0f,
		.elements[13] = 0.0f,
		.elements[14] = 0.0f,
		.elements[15] = 1.0f
	);
}

AX_API
UMat4
AX_Math_Mat4_MakeRotationXYZ
(Float xRadian, Float yRadian, Float zRadian)
{
	UMat4
		rotX = AX_Math_Mat4_MakeRotationX(xRadian),
		rotY = AX_Math_Mat4_MakeRotationY(yRadian),
		rotZ = AX_Math_Mat4_MakeRotationZ(zRadian),
		rotXY = AX_Math_Mat4_Mul(&rotX, &rotY),
		rotXYZ = AX_Math_Mat4_Mul(&rotXY, &rotZ);

	return rotXYZ;
}

AX_API AX_INLINE
UMat4
AX_Math_Mat4_MakeScale
(UVec3 *scale)
{
	return
		(!scale) ?
		AX_Math_Mat4_Identity() :
		AX_STRUCT(
			UMat4,
			.elements[0] = scale->x,
			.elements[1] = 0.0f,
			.elements[2] = 0.0f,
			.elements[3] = 0.0f,

			.elements[4] = 0.0f,
			.elements[5] = scale->y,
			.elements[6] = 0.0f,
			.elements[7] = 0.0f,

			.elements[8] = 0.0f,
			.elements[9] = 0.0f,
			.elements[10] = scale->z,
			.elements[11] = 0.0f,

			.elements[12] = 0.0f,
			.elements[13] = 0.0f,
			.elements[14] = 0.0f,
			.elements[15] = 1.0f
		);
}

AX_API
UMat4
AX_Math_Mat4_Transform
(UVec3 *translation, UVec3 *rotation, UVec3 *scale)
{
	if (!translation || !rotation || !scale)
		return AX_Math_Mat4_Identity();

	UMat4
		matTrans = AX_Math_Mat4_MakeTranslation(translation),
		matRot = AX_Math_Mat4_MakeRotationXYZ(rotation->x, rotation->y, rotation->z),
		matScale = AX_Math_Mat4_MakeScale(scale),
		matRotScale = AX_Math_Mat4_Mul(&matRot, &matScale),
		matRotScaleTrans = AX_Math_Mat4_Mul(&matRotScale, &matTrans);

	return matRotScaleTrans;
}

AX_API AX_INLINE
UVec3
AX_Math_Mat4_MakeForwardVector
(UMat4 *mat)
{
	return
		(!mat) ?
		AX_Math_Vec3_Zero() :
		AX_Math_Vec3_Normalized(
			AX_Math_Vec3_Construct(
				mat->elements[2] * -1.0f,
				mat->elements[6] * -1.0f,
				mat->elements[10] * -1.0f
			)
		);
}

AX_API AX_INLINE
UVec3
AX_Math_Mat4_MakeBackwardVector
(UMat4 *mat)
{
	return
		(!mat) ?
		AX_Math_Vec3_Zero() :
		AX_Math_Vec3_Normalized(
			AX_Math_Vec3_Construct(
				mat->elements[2],
				mat->elements[6],
				mat->elements[10]
			)
		);
}

AX_API AX_INLINE
UVec3
AX_Math_Mat4_MakeUpVector
(UMat4 *mat)
{
	return
		(!mat) ?
		AX_Math_Vec3_Zero() :
		AX_Math_Vec3_Normalized(
			AX_Math_Vec3_Construct(
				mat->elements[1],
				mat->elements[5],
				mat->elements[9]
			)
		);
}

AX_API AX_INLINE
UVec3
AX_Math_Mat4_MakeDownVector
(UMat4 *mat)
{
	return
		(!mat) ?
		AX_Math_Vec3_Zero() :
		AX_Math_Vec3_Normalized(
			AX_Math_Vec3_Construct(
				mat->elements[1] * -1.0f,
				mat->elements[5] * -1.0f,
				mat->elements[9] * -1.0f
			)
		);
}

AX_API AX_INLINE
UVec3
AX_Math_Mat4_MakeLeftVector
(UMat4 *mat)
{
	return
		(!mat) ?
		AX_Math_Vec3_Zero() :
		AX_Math_Vec3_Normalized(
			AX_Math_Vec3_Construct(
				mat->elements[0] * -1.0f,
				mat->elements[4] * -1.0f,
				mat->elements[8] * -1.0f
			)
		);
}

AX_API AX_INLINE
UVec3
AX_Math_Mat4_MakeRightVector
(UMat4 *mat)
{
	return
		(!mat) ?
		AX_Math_Vec3_Zero() :
		AX_Math_Vec3_Normalized(
			AX_Math_Vec3_Construct(
				mat->elements[0],
				mat->elements[4],
				mat->elements[8]
			)
		);
}

AX_API AX_INLINE
UVec3
AX_Math_Mat4_Mat4MulVec3
(UMat4 *mat, UVec3 *vec)
{
	return
		(!mat || !vec) ?
		AX_Math_Vec3_Zero() :
		AX_STRUCT(
			UVec3,
			(mat->elements[0] * vec->x) + (mat->elements[1] * vec->y) + (mat->elements[2] * vec->z) + mat->elements[3],
			(mat->elements[4] * vec->x) + (mat->elements[5] * vec->y) + (mat->elements[6] * vec->z) + mat->elements[7],
			(mat->elements[8] * vec->x) + (mat->elements[9] * vec->y) + (mat->elements[10] * vec->z) + mat->elements[11]
		);
}

AX_API AX_INLINE
UVec3
AX_Math_Mat4_Vec3MulMat4
(UVec3 *vec, UMat4 *mat)
{
	return
		(!mat || !vec) ?
		AX_Math_Vec3_Zero() :
		AX_STRUCT(
			UVec3,
			(vec->x * mat->elements[0]) + (vec->y * mat->elements[4]) + (vec->z * mat->elements[8]) + mat->elements[12],
			(vec->x * mat->elements[1]) + (vec->y * mat->elements[5]) + (vec->z * mat->elements[9]) + mat->elements[13],
			(vec->x * mat->elements[2]) + (vec->y * mat->elements[6]) + (vec->z * mat->elements[10]) + mat->elements[14]
		);
}

AX_API AX_INLINE
UVec4
AX_Math_Mat4_Mat4MulVec4
(UMat4 *mat, UVec4 *vec)
{
	return
		(!mat || !vec) ?
		AX_Math_Vec4_Zero() :
		AX_STRUCT(
			UVec4,
			(mat->elements[0] * vec->x) + (mat->elements[1] * vec->y) + (mat->elements[2] * vec->z) + (mat->elements[3] * vec->w),
			(mat->elements[4] * vec->x) + (mat->elements[5] * vec->y) + (mat->elements[6] * vec->z) + (mat->elements[7] * vec->w),
			(mat->elements[8] * vec->x) + (mat->elements[9] * vec->y) + (mat->elements[10] * vec->z) + (mat->elements[11] * vec->w),
			(mat->elements[12] * vec->x) + (mat->elements[13] * vec->y) + (mat->elements[14] * vec->z) + (mat->elements[15] * vec->w)
		);
}

AX_API AX_INLINE
UVec4
AX_Math_Mat4_Vec4MulMat4
(UVec4 *vec, UMat4 *mat)
{
	return
		(!mat || !vec) ?
		AX_Math_Vec4_Zero() :
		AX_STRUCT(
			UVec4,
			(vec->x * mat->elements[0]) + (vec->y * mat->elements[4]) + (vec->z * mat->elements[8]) + (vec->w * mat->elements[12]),
			(vec->x * mat->elements[1]) + (vec->y * mat->elements[5]) + (vec->z * mat->elements[9]) + (vec->w * mat->elements[13]),
			(vec->x * mat->elements[2]) + (vec->y * mat->elements[6]) + (vec->z * mat->elements[10]) + (vec->w * mat->elements[14]),
			(vec->x * mat->elements[3]) + (vec->y * mat->elements[7]) + (vec->z * mat->elements[11]) + (vec->w * mat->elements[15])
		);
}

AX_API AX_INLINE
UVec4
AX_Math_Mat4_GetRow
(UMat4 *mat, Int32 row)
{
	return
		(!mat || ((row < 0) || (row > 3))) ?
		AX_Math_Vec4_Zero() :
		AX_STRUCT(
			UVec4,
			mat->elements[(row * 4) + 0],
			mat->elements[(row * 4) + 1],
			mat->elements[(row * 4) + 2],
			mat->elements[(row * 4) + 3]
		);
}

AX_API AX_INLINE
UVec4
AX_Math_Mat4_GetColumn
(UMat4 *mat, Int32 column)
{
	return
		(!mat || ((column < 0) || (column > 3))) ?
		AX_Math_Vec4_Zero() :
		AX_STRUCT(
			UVec4,
			mat->elements[column + 0],
			mat->elements[column + 4],
			mat->elements[column + 8],
			mat->elements[column + 12]
		);
}

AX_API
Bool
AX_Math_Mat4_SetRow
(UMat4 *mat, Int32 row, UVec4 *data)
{
	if (!mat || !mat->elements || ((row < 0) || (row > 3)))
		return false;

	AX_HAL_Memory_Memcpy(&mat->elements[row * 4], data, sizeof(UVec4));
	return true;
}

AX_API
Bool
AX_Math_Mat4_SetColumn
(UMat4 *mat, Int32 column, UVec4 *data)
{
	if (!mat || !data || ((column < 0) || (column > 3)))
		return false;

	mat->elements[column + 0] = data->x;
	mat->elements[column + 4] = data->y;
	mat->elements[column + 8] = data->z;
	mat->elements[column + 12] = data->w;

	return true;
}

AX_API AX_INLINE
UMat3
AX_Math_Mat4_GetTopLeftMat3
(UMat4 *mat)
{
	return
		(!mat) ?
		AX_Math_Mat3_Identity() :
		AX_STRUCT(
			UMat3,
			.elements[0] = mat->elements[0],
			.elements[1] = mat->elements[1],
			.elements[2] = mat->elements[2],

			.elements[3] = mat->elements[4],
			.elements[4] = mat->elements[5],
			.elements[5] = mat->elements[6],

			.elements[6] = mat->elements[8],
			.elements[7] = mat->elements[9],
			.elements[8] = mat->elements[10]
		);
}

AX_API
Bool
AX_Math_Mat4_SetTopLeftMat3
(UMat4 *mat, UMat3 *data)
{
	if (!mat || !mat->elements)
		return false;

	mat->elements[0] = data->elements[0];
	mat->elements[1] = data->elements[1];
	mat->elements[2] = data->elements[2];
	
	mat->elements[4] = data->elements[3];
	mat->elements[5] = data->elements[4];
	mat->elements[6] = data->elements[5];
	
	mat->elements[8] = data->elements[6];
	mat->elements[9] = data->elements[7];
	mat->elements[10] = data->elements[8];

	return true;
}

AX_API AX_INLINE
Bool
AX_Math_Mat4_Equals
(UMat4 *left, UMat4 *right)
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
		((left->elements[8] - right->elements[8]) < AX_MATH_EPSILON) &&
		((left->elements[9] - right->elements[9]) < AX_MATH_EPSILON) &&
		((left->elements[10] - right->elements[10]) < AX_MATH_EPSILON) &&
		((left->elements[11] - right->elements[11]) < AX_MATH_EPSILON) &&
		((left->elements[12] - right->elements[12]) < AX_MATH_EPSILON) &&
		((left->elements[13] - right->elements[13]) < AX_MATH_EPSILON) &&
		((left->elements[14] - right->elements[14]) < AX_MATH_EPSILON) &&
		((left->elements[15] - right->elements[15]) < AX_MATH_EPSILON);
}

AX_API AX_INLINE
Bool
AX_Math_Mat4_ApproxEquals
(UMat4 *left, UMat4 *right, Float limit)
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
		((left->elements[8] - right->elements[8]) < limit) &&
		((left->elements[9] - right->elements[9]) < limit) &&
		((left->elements[10] - right->elements[10]) < limit) &&
		((left->elements[11] - right->elements[11]) < limit) &&
		((left->elements[12] - right->elements[12]) < limit) &&
		((left->elements[13] - right->elements[13]) < limit) &&
		((left->elements[14] - right->elements[14]) < limit) &&
		((left->elements[15] - right->elements[15]) < limit);
}

AX_API AX_INLINE
Bool
AX_Math_Mat4_IsZero
(UMat4 *mat)
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
		(mat->elements[8] == 0.0f) &&
		(mat->elements[9] == 0.0f) &&
		(mat->elements[10] == 0.0f) &&
		(mat->elements[11] == 0.0f) &&
		(mat->elements[12] == 0.0f) &&
		(mat->elements[13] == 0.0f) &&
		(mat->elements[14] == 0.0f) &&
		(mat->elements[15] == 0.0f);
}

AX_API AX_INLINE
Bool
AX_Math_Mat4_IsIdentity
(UMat4 *mat)
{
	return
		(!mat) ?
		false :
		(mat->elements[0] == 1.0f) &&
		(mat->elements[1] == 0.0f) &&
		(mat->elements[2] == 0.0f) &&
		(mat->elements[3] == 0.0f) &&
		(mat->elements[4] == 0.0f) &&
		(mat->elements[5] == 1.0f) &&
		(mat->elements[6] == 0.0f) &&
		(mat->elements[7] == 0.0f) &&
		(mat->elements[8] == 0.0f) &&
		(mat->elements[9] == 0.0f) &&
		(mat->elements[10] == 1.0f) &&
		(mat->elements[11] == 0.0f) &&
		(mat->elements[12] == 0.0f) &&
		(mat->elements[13] == 0.0f) &&
		(mat->elements[14] == 0.0f) &&
		(mat->elements[15] == 1.0f);
}



