#include "Arctix/Core/Math/Vector/Vec3.h"

#include "Arctix/Core/Math/Helper/MathHelper.h"


AX_API AX_INLINE
UVec3
AX_Math_Vec3_Construct
(Float x, Float y, Float z)
{
	return AX_STRUCT(UVec3, x, y, z);
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Zero
(void)
{
	return AX_STRUCT(UVec3, 0.0f);
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_One
(void)
{
	return AX_STRUCT(UVec3, 1.0f, 1.0f, 1.0f);
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Scale
(Float scale)
{
	return AX_STRUCT(UVec3, scale);
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Up
(void)
{
	return AX_STRUCT(UVec3, 0.0f, 1.0f, 0.0f);
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Down
(void)
{
	return AX_STRUCT(UVec3, 0.0f, -1.0f, 0.0f);
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Left
(void)
{
	return AX_STRUCT(UVec3, -1.0f, 0.0f, 0.0f);
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Right
(void)
{
	return AX_STRUCT(UVec3, 1.0f, 0.0f, 0.0f);
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Forward
(void)
{
	return AX_STRUCT(UVec3, 0.0f, 0.0f, 1.0f);
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Backward
(void)
{
	return AX_STRUCT(UVec3, 0.0f, 0.0f, -1.0f);
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Copy
(UVec3 *vec)
{
	return (!vec) ? AX_Math_Vec3_Zero() : (*vec);
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Add
(UVec3 left, UVec3 right)
{
	return AX_STRUCT(UVec3, left.x + right.x, left.y + right.y, left.z + right.z);
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Sub
(UVec3 left, UVec3 right)
{
	return AX_STRUCT(UVec3, left.x - right.x, left.y - right.y, left.z - right.z);
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Mul
(UVec3 left, UVec3 right)
{
	return AX_STRUCT(UVec3, left.x * right.x, left.y * right.y, left.z * right.z);
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_MulScalar
(UVec3 vec, Float scalar)
{
	return AX_STRUCT(UVec3, vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_MulAdd
(UVec3 vec1, UVec3 vec2, UVec3 vec3)
{
	return AX_STRUCT(
		UVec3,
		vec1.x * vec2.x + vec3.x,
		vec1.y * vec2.y + vec3.y,
		vec1.z * vec2.z + vec3.z
	);
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Div
(UVec3 left, UVec3 right)
{
	return
		((right.x == 0) || (right.y == 0) || (right.z == 0)) ?
		left :
		AX_STRUCT(UVec3, left.x / right.x, left.y / right.y, left.z / right.z);
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_DivScalar
(UVec3 vec, Float scalar)
{
	return
		(scalar == 0) ?
		vec :
		AX_STRUCT(UVec3, vec.x / scalar, vec.y / scalar, vec.z / scalar);
}

AX_API AX_INLINE
Float
AX_Math_Vec3_MagnitudeSquared
(UVec3 vec)
{
	return (AX_Math_Pow(vec.x, 2.0f) + AX_Math_Pow(vec.y, 2.0f) + AX_Math_Pow(vec.z, 2.0f));
}

AX_API AX_INLINE
Float
AX_Math_Vec3_Magnitude
(UVec3 vec)
{
	return AX_Math_Sqrt(AX_Math_Vec3_MagnitudeSquared(vec));
}

AX_API
Bool
AX_Math_Vec3_Normalize
(UVec3 *vec)
{
	if (!vec)
		return false;

	Float length = AX_Math_Vec3_Magnitude(*vec);

	vec->x /= length;
	vec->y /= length;
	vec->z /= length;

	return true;
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Normalized
(UVec3 vec)
{
	AX_Math_Vec3_Normalize(&vec);
	return vec;
}

AX_API AX_INLINE
Float
AX_Math_Vec3_Dot
(UVec3 left, UVec3 right)
{
	return ((left.x * right.x) + (left.y * right.y) + (left.z * right.z));
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Cross
(UVec3 left, UVec3 right)
{
	return AX_STRUCT(
		UVec3,
		(left.y * right.z) - (left.z * right.y),
		(left.z * right.x) - (left.x * right.z),
		(left.x * right.y) - (left.y * right.x)
	);
}

AX_API AX_INLINE
Bool
AX_Math_Vec3_Equals
(UVec3 left, UVec3 right)
{
	return	(AX_Math_Abs(left.x - right.x) < AX_MATH_EPSILON) &&
		(AX_Math_Abs(left.y - right.y) < AX_MATH_EPSILON) &&
		(AX_Math_Abs(left.z - right.z) < AX_MATH_EPSILON);
}

AX_API AX_INLINE
Bool
AX_Math_Vec3_ApproxEquals
(UVec3 left, UVec3 right, Float limit)
{
	return	(AX_Math_Abs(left.x - right.x) <= limit) &&
		(AX_Math_Abs(left.y - right.y) <= limit) &&
		(AX_Math_Abs(left.z - right.z) <= limit);
}

AX_API AX_INLINE
Float
AX_Math_Vec3_Distance
(UVec3 left, UVec3 right)
{
	return AX_Math_Vec3_Magnitude(AX_Math_Vec3_Sub(left, right));
}

AX_API AX_INLINE
Float
AX_Math_Vec3_DistanceSquared
(UVec3 left, UVec3 right)
{
	return AX_Math_Vec3_MagnitudeSquared(AX_Math_Vec3_Sub(left, right));
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Transform
(UVec3 vec, Float w, UMat4 mat)
{
	return AX_STRUCT(
		UVec3,
		(vec.x * mat.elements[0 + 0]) + (vec.y * mat.elements[4 + 0]) + (vec.z * mat.elements[8 + 0]) + (w * mat.elements[12 + 0]),
		(vec.x * mat.elements[0 + 1]) + (vec.y * mat.elements[4 + 1]) + (vec.z * mat.elements[8 + 1]) + (w * mat.elements[12 + 1]),
		(vec.x * mat.elements[0 + 2]) + (vec.y * mat.elements[4 + 2]) + (vec.z * mat.elements[8 + 2]) + (w * mat.elements[12 + 2])
	);
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_ConvertFromVec4
(UVec4 vec)
{
	return AX_STRUCT(UVec3, vec.x, vec.y, vec.z);
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Lerp
(UVec3 start, UVec3 end, Float time)
{
	return AX_Math_Vec3_Add(AX_Math_Vec3_MulScalar(start, 1.0f - time), AX_Math_Vec3_MulScalar(end, time));
}

AX_API
UVec3
AX_Math_Vec3_Slerp
(UVec3 start, UVec3 end, Float time)
{
	Float dot = AX_MATH_CLAMP(AX_Math_Vec3_Dot(start, end), -1.f, 1.f);
	Float theta = AX_Math_Cos(dot) * time;
	UVec3 relVec = AX_Math_Vec3_Sub(end, AX_Math_Vec3_MulScalar(start, dot));
	return AX_Math_Vec3_Add(AX_Math_Vec3_MulScalar(start, AX_Math_Cos(theta)), AX_Math_Vec3_MulScalar(relVec, AX_Math_Sin(theta)));
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Project
(UVec3 vec, UVec3 normal)
{
	return AX_Math_Vec3_MulScalar(normal, AX_Math_Vec3_Dot(vec, normal) / AX_Math_Vec3_Dot(normal, normal));
}

AX_API AX_INLINE
UVec3
AX_Math_Vec3_Reflect
(UVec3 vec, UVec3 normal)
{
	return AX_Math_Vec3_Sub(vec, AX_Math_Vec3_MulScalar(normal, AX_Math_Vec3_Dot(vec, normal) * 2.0f));
}








