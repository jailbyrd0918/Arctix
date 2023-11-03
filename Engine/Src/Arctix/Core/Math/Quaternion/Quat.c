#include "Arctix/Core/Math/Quaternion/Quat.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"

#include "Arctix/Core/Math/Helper/MathHelper.h"
#include "Arctix/Core/Math/Vector/Vec3.h"
#include "Arctix/Core/Math/Vector/Vec4.h"
#include "Arctix/Core/Math/Matrix/Mat3.h"
#include "Arctix/Core/Math/Matrix/Mat4.h"


AX_API AX_INLINE
UQuat
AX_Math_Quat_Construct
(Float x, Float y, Float z, Float w)
{
	return AX_STRUCT(UQuat, x, y, z, w);
}

AX_API AX_INLINE
UQuat
AX_Math_Quat_Identity
(void)
{
	return AX_STRUCT(UQuat, 0.0f, 0.0f, 0.0f, 1.0f);
}

AX_API AX_INLINE
UQuat
AX_Math_Quat_Copy
(UQuat *quat)
{
	return (!quat) ? AX_Math_Quat_Identity() : (*quat);
}

AX_API AX_INLINE
UQuat
AX_Math_Quat_ConvertFromAngles
(Float x, Float y, Float z)
{
	#define	ROLL	AX_Math_Quat_Construct(AX_Math_Sin(x * 0.5f), 0.0f, 0.0f, AX_Math_Cos(x * 0.5f))
	#define	PITCH	AX_Math_Quat_Construct(0.0f, AX_Math_Sin(y * 0.5f), 0.0f, AX_Math_Cos(y * 0.5f))
	#define	YAW	AX_Math_Quat_Construct(0.0f, 0.0f, AX_Math_Sin(z * 0.5f), AX_Math_Cos(z * 0.5f))

	return AX_Math_Quat_Normalized(AX_Math_Quat_Mul(AX_Math_Quat_Mul(YAW, PITCH), ROLL));
}

AX_API AX_INLINE
UQuat
AX_Math_Quat_ConvertFromRotation
(UVec3 rotation)
{
	return AX_Math_Quat_ConvertFromAngles(rotation.x, rotation.y, rotation.z);
}

AX_API AX_INLINE
UQuat
AX_Math_Quat_ConvertFromAxisAngle
(UVec3 axis, Float angle, Bool normalize)
{
	return
		(!normalize) ?
		AX_Math_Quat_Construct(
			axis.x * AX_Math_Sin(angle * 0.5f),
			axis.y * AX_Math_Sin(angle * 0.5f),
			axis.z * AX_Math_Sin(angle * 0.5f),
			AX_Math_Cos(angle * 0.5f)
		) :
		AX_Math_Quat_Normalized(
			AX_Math_Quat_Construct(
				axis.x * AX_Math_Sin(angle * 0.5f),
				axis.y * AX_Math_Sin(angle * 0.5f),
				axis.z * AX_Math_Sin(angle * 0.5f),
				AX_Math_Cos(angle * 0.5f)
			)
		);
}

AX_API AX_INLINE
UMat4
AX_Math_Quat_ConvertToMat4
(UQuat quat)
{
	#define	NORM	AX_Math_Quat_Normalized(quat)

	return AX_STRUCT(
		UMat4,
		.elements[0] = 1.0f - (2.0f * AX_Math_Pow(NORM.y, 2.0f)) - (2.0f * AX_Math_Pow(NORM.y, 2.0f)),
		.elements[1] = (2.0f * (NORM.x * NORM.y)) - (2.0f * (NORM.z * NORM.w)),
		.elements[2] = (2.0f * (NORM.x * NORM.z)) + (2.0f * (NORM.y * NORM.w)),
		.elements[3] = 0.0f,

		.elements[4] = (2.0f * (NORM.x * NORM.y) + 2.0f * (NORM.z * NORM.w)),
		.elements[5] = 1.0f - (2.0f * (NORM.x * NORM.x)) - (2.0f * (NORM.z * NORM.z)),
		.elements[6] = (2.0f * (NORM.y * NORM.z) - 2.0f * (NORM.x * NORM.w)),
		.elements[7] = 0.0f,

		.elements[8] = (2.0f * (NORM.x * NORM.z) - 2.0f * (NORM.y * NORM.w)),
		.elements[9] = (2.0f * (NORM.y * NORM.z) + 2.0f * (NORM.x * NORM.w)),
		.elements[10] = 1.0f - (2.0f * (NORM.x * NORM.x)) - (2.0f * (NORM.y * NORM.y)),
		.elements[11] = 0.0f,

		.elements[12] = 0.0f,
		.elements[13] = 0.0f,
		.elements[14] = 0.0f,
		.elements[15] = 1.0f
	);
}

AX_API AX_INLINE
UMat4
AX_Math_Quat_ConvertToRotationMat
(UQuat quat)
{
	return AX_STRUCT(
		UMat4,
		.elements[0] = 1.0f - (2.0f * AX_Math_Pow(quat.y, 2.0f)) - (2.0f * AX_Math_Pow(quat.z, 2.0f)),
		.elements[1] = (2.0f * (quat.x * quat.y) + 2.0f * (quat.w * quat.z)),
		.elements[2] = (2.0f * (quat.x * quat.z) - 2.0f * (quat.w * quat.y)),
		.elements[3] = 0.0f,

		.elements[4] = (2.0f * (quat.x * quat.y) - 2.0f * (quat.w * quat.z)),
		.elements[5] = 1.0f - (2.0f * AX_Math_Pow(quat.x, 2.0f)) - (2.0f * AX_Math_Pow(quat.z, 2.0f)),
		.elements[6] = (2.0f * (quat.y * quat.z) + 2.0f * (quat.w * quat.x)),
		.elements[7] = 0.0f,

		.elements[8] = (2.0f * (quat.x * quat.z) + 2.0f * (quat.w * quat.y)),
		.elements[9] = (2.0f * (quat.y * quat.z) - 2.0f * (quat.w * quat.x)),
		.elements[10] = 1.0f - (2.0f * AX_Math_Pow(quat.x, 2.0f)) - (2.0f * AX_Math_Pow(quat.y, 2.0f)),
		.elements[11] = 0.0f,

		.elements[12] = 0.0f,
		.elements[13] = 0.0f,
		.elements[14] = 0.0f,
		.elements[15] = 1.0f
	);
}

AX_API AX_INLINE
UQuat
AX_Math_Quat_Add
(UQuat left, UQuat right)
{
	return AX_STRUCT(
		UQuat,
		left.x + right.x,
		left.y + right.y,
		left.z + right.z,
		left.w + right.w
	);
}

AX_API AX_INLINE
UQuat
AX_Math_Quat_Sub
(UQuat left, UQuat right)
{
	return AX_STRUCT(
		UQuat,
		left.x - right.x,
		left.y - right.y,
		left.z - right.z,
		left.w - right.w
	);
}

AX_API AX_INLINE
UQuat
AX_Math_Quat_Mul
(UQuat left, UQuat right)
{
	return AX_STRUCT(
		UQuat,
		(left.w * right.w) - (left.x * right.x) - (left.y * right.y) - (left.z * right.z),
		(left.w * right.x) + (left.x * right.w) + (left.y * right.z) - (left.z * right.y),
		(left.w * right.y) - (left.x * right.z) + (left.y * right.w) + (left.z * right.x),
		(left.w * right.z) + (left.x * right.y) - (left.y * right.x) + (left.z * right.w)
	);
}

AX_API AX_INLINE
UQuat
AX_Math_Quat_MulScalar
(UQuat quat, Float scalar)
{
	return AX_STRUCT(
		UQuat,
		quat.x * scalar,
		quat.y * scalar,
		quat.z * scalar,
		quat.w * scalar
	);
}

AX_API AX_INLINE
Float
AX_Math_Quat_Dot
(UQuat left, UQuat right)
{
	return ((left.x * right.x) + (left.y * right.y) + (left.z * right.z) + (left.w * right.w));
}

AX_API AX_INLINE
Bool
AX_Math_Quat_Equals
(UQuat left, UQuat right)
{
	return
		((left.x - right.x) < AX_MATH_EPSILON) &&
		((left.y - right.y) < AX_MATH_EPSILON) &&
		((left.z - right.z) < AX_MATH_EPSILON) &&
		((left.w - right.w) < AX_MATH_EPSILON);
}

AX_API AX_INLINE
Bool
AX_Math_Quat_ApproxEquals
(UQuat left, UQuat right, Float limit)
{
	return
		((left.x - right.x) < limit) &&
		((left.y - right.y) < limit) &&
		((left.z - right.z) < limit) &&
		((left.w - right.w) < limit);
}

AX_API AX_INLINE
Float
AX_Math_Quat_MagnitudeSquared
(UQuat quat)
{
	return AX_Math_Pow(quat.x, 2.0f) + AX_Math_Pow(quat.y, 2.0f) + AX_Math_Pow(quat.z, 2.0f) + AX_Math_Pow(quat.w, 2.0f);
}

AX_API AX_INLINE
Float
AX_Math_Quat_Magnitude
(UQuat quat)
{
	return AX_Math_Sqrt(AX_Math_Quat_MagnitudeSquared(quat));
}

AX_API
Bool
AX_Math_Quat_Normalize
(UQuat *quat)
{
	if (!quat)
		return false;

	Float magnitude = AX_Math_Quat_Magnitude(*quat);

	quat->x /= magnitude;
	quat->y /= magnitude;
	quat->z /= magnitude;
	quat->w /= magnitude;

	return true;
}

AX_API AX_INLINE
UQuat
AX_Math_Quat_Normalized
(UQuat quat)
{
	AX_Math_Quat_Normalize(&quat);
	return quat;
}

AX_API AX_INLINE
UQuat
AX_Math_Quat_Conjugate
(UQuat quat)
{
	return AX_STRUCT(
		UQuat,
		quat.x * -1.0f,
		quat.y * -1.0f,
		quat.z * -1.0f,
		quat.w
	);
}

AX_API AX_INLINE
UQuat
AX_Math_Quat_Quotient
(UQuat left, UQuat right)
{
	return AX_Math_Quat_Mul(AX_Math_Quat_Inversed(left), AX_Math_Quat_Inversed(right));
}

AX_API
Bool
AX_Math_Quat_Inverse
(UQuat *quat)
{
	if (!quat)
		return false;

	*quat = AX_Math_Quat_Normalized(AX_Math_Quat_Conjugate(*quat));
	return true;
}

AX_API AX_INLINE
UQuat
AX_Math_Quat_Inversed
(UQuat quat)
{
	AX_Math_Quat_Inverse(&quat);
	return quat;
}

AX_API AX_INLINE
UQuat
AX_Math_Quat_Lerp
(UQuat start, UQuat end, Float time, Bool normalize)
{
	return
		(normalize) ?
		AX_Math_Quat_Add(
			AX_Math_Quat_MulScalar(start, 1.0f - time),
			AX_Math_Quat_MulScalar(end, time)
		) :

		AX_Math_Quat_Normalized(
			AX_Math_Quat_Add(
				AX_Math_Quat_MulScalar(start, 1.0f - time),
				AX_Math_Quat_MulScalar(end, time)
			)
		);
}

AX_API AX_INLINE
UQuat
AX_Math_Quat_Slerp
(UQuat start, UQuat end, Float time, Bool normalize)
{
	#define	THETA	AX_Math_Abs(AX_Math_Acos(AX_Math_Quat_Dot(start, end)))
	#define	WP	AX_Math_Sin((1 - time) * THETA) / AX_Math_Sin(THETA)
	#define	WQ	AX_Math_Sin(time * THETA) / AX_Math_Sin(THETA)

	return
		(normalize) ?
		(
			AX_Math_Quat_Equals(start, end) ?
			AX_Math_Quat_Normalized(start) :

			AX_Math_Quat_Normalized(
				AX_Math_Quat_Add(
					AX_Math_Quat_MulScalar(start, WP),
					AX_Math_Quat_MulScalar(end, WQ)
				)
			)) :
		(
			AX_Math_Quat_Equals(start, end) ?
			start :
			
			AX_Math_Quat_Add(
				AX_Math_Quat_MulScalar(start, WP),
				AX_Math_Quat_MulScalar(end, WQ)
			));
}


