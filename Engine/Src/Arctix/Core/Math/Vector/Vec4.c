#include "Arctix/Core/Math/Vector/Vec4.h"

#include "Arctix/Core/Math/Helper/MathHelper.h"


AX_API AX_INLINE
UVec4
AX_Math_Vec4_Construct
(Float x, Float y, Float z, Float w)
{
	return AX_STRUCT(UVec4, x, y, z, w);
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Zero
(void)
{
	return AX_STRUCT(UVec4, 0.0f);
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_One
(void)
{
	return AX_STRUCT(UVec4, 1.0f);
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Scale
(Float scale)
{
	return AX_STRUCT(UVec4, scale);
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Up
(void)
{
	return AX_STRUCT(UVec4, 0.0f, 1.0f, 0.0f, 0.0f);
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Down
(void)
{
	return AX_STRUCT(UVec4, 0.0f, -1.0f, 0.0f, 0.0f);
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Left
(void)
{
	return AX_STRUCT(UVec4, -1.0f, 0.0f, 0.0f, 0.0f);
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Right
(void)
{
	return AX_STRUCT(UVec4, 1.0f, 0.0f, 0.0f, 0.0f);
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Forward
(void)
{
	return AX_STRUCT(UVec4, 0.0f, 0.0f, 1.0f, 0.0f);
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Backward
(void)
{
	return AX_STRUCT(UVec4, 0.0f, 0.0f, 1.0f, 0.0f);
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Copy
(UVec4 *vec)
{
	return (!vec) ? AX_Math_Vec4_Zero() : (*vec);
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Add
(UVec4 left, UVec4 right)
{
	return AX_STRUCT(UVec4, left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Sub
(UVec4 left, UVec4 right)
{
	return AX_STRUCT(UVec4, left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Mul
(UVec4 left, UVec4 right)
{
	return AX_STRUCT(UVec4, left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w);
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_MulScalar
(UVec4 vec, Float scalar)
{
	return AX_STRUCT(UVec4, vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w * scalar);
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_MulAdd
(UVec4 vec1, UVec4 vec2, UVec4 vec3)
{
	return AX_STRUCT(
		UVec4,
		vec1.x * vec2.x + vec3.x,
		vec1.y * vec2.y + vec3.y,
		vec1.z * vec2.z + vec3.z,
		vec1.w * vec2.w + vec3.w
	);
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Div
(UVec4 left, UVec4 right)
{
	return
		((right.x == 0) || (right.y == 0) || (right.z == 0) || (right.w == 0)) ?
		left :
		AX_STRUCT(UVec4, left.x / right.x, left.y / right.y, left.z / right.z, left.w / right.w);
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_DivScalar
(UVec4 vec, Float scalar)
{
	return
		(scalar == 0) ?
		vec :
		AX_STRUCT(UVec4, vec.x / scalar, vec.y / scalar, vec.z / scalar, vec.w / scalar);
}

AX_API AX_INLINE
Float
AX_Math_Vec4_MagnitudeSquared
(UVec4 vec)
{
	return (AX_Math_Pow(vec.x, 2.0f) + AX_Math_Pow(vec.y, 2.0f) + AX_Math_Pow(vec.z, 2.0f) + AX_Math_Pow(vec.w, 2.0f));
}

AX_API AX_INLINE
Float
AX_Math_Vec4_Magnitude
(UVec4 vec)
{
	return AX_Math_Sqrt(AX_Math_Vec4_MagnitudeSquared(vec));
}

AX_API
Bool
AX_Math_Vec4_Normalize
(UVec4 *vec)
{
	if (!vec)
		return false;

	Float length = AX_Math_Vec4_Magnitude(*vec);

	vec->x /= length;
	vec->y /= length;
	vec->z /= length;
	vec->w /= length;

	return true;
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Normalized
(UVec4 vec)
{
	AX_Math_Vec4_Normalize(&vec);
	return vec;
}

AX_API AX_INLINE
Float
AX_Math_Vec4_Dot
(UVec4 left, UVec4 right)
{
	return ((left.x * right.x) + (left.y * right.y) + (left.z * right.z) + (left.w * right.w));
}

AX_API AX_INLINE
Bool
AX_Math_Vec4_Equals
(UVec4 left, UVec4 right)
{
	return	(AX_Math_Abs(left.x - right.x) < AX_MATH_EPSILON) &&
		(AX_Math_Abs(left.y - right.y) < AX_MATH_EPSILON) &&
		(AX_Math_Abs(left.z - right.z) < AX_MATH_EPSILON) &&
		(AX_Math_Abs(left.w - right.w) < AX_MATH_EPSILON);
}

AX_API AX_INLINE
Bool
AX_Math_Vec4_ApproxEquals
(UVec4 left, UVec4 right, Float limit)
{
	return	(AX_Math_Abs(left.x - right.x) <= limit) &&
		(AX_Math_Abs(left.y - right.y) <= limit) &&
		(AX_Math_Abs(left.z - right.z) <= limit) &&
		(AX_Math_Abs(left.w - right.w) <= limit);
}

AX_API AX_INLINE
Float
AX_Math_Vec4_Distance
(UVec4 left, UVec4 right)
{
	return AX_Math_Vec4_Magnitude(AX_Math_Vec4_Sub(left, right));
}

AX_API AX_INLINE
Float
AX_Math_Vec4_DistanceSquared
(UVec4 left, UVec4 right)
{
	return AX_Math_Vec4_MagnitudeSquared(AX_Math_Vec4_Sub(left, right));
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Transform
(UVec4 vec, Float w, UMat4 mat)
{
	return AX_STRUCT(
		UVec4,
		(vec.x * mat.elements[0 + 0]) + (vec.y * mat.elements[4 + 0]) + (vec.z * mat.elements[8 + 0]) + (w * mat.elements[12 + 0]),
		(vec.x * mat.elements[0 + 1]) + (vec.y * mat.elements[4 + 1]) + (vec.z * mat.elements[8 + 1]) + (w * mat.elements[12 + 1]),
		(vec.x * mat.elements[0 + 2]) + (vec.y * mat.elements[4 + 2]) + (vec.z * mat.elements[8 + 2]) + (w * mat.elements[12 + 2]),
		(vec.x * mat.elements[0 + 3]) + (vec.y * mat.elements[4 + 3]) + (vec.z * mat.elements[8 + 3]) + (w * mat.elements[12 + 3])
	);
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_ConvertFromVec3
(UVec3 vec, Float w)
{
	return AX_STRUCT(UVec4, vec.x, vec.y, vec.z, w);
}

AX_API AX_INLINE
UVec4
AX_Math_Vec4_Lerp
(UVec4 start, UVec4 end, Float time)
{
	return AX_Math_Vec4_Add(AX_Math_Vec4_MulScalar(start, 1.0f - time), AX_Math_Vec4_MulScalar(end, time));
}

AX_API
UVec4
AX_Math_Vec4_Slerp
(UVec4 start, UVec4 end, Float time)
{
	Float dot = AX_MATH_CLAMP(AX_Math_Vec4_Dot(start, end), -1.f, 1.f);
	Float theta = AX_Math_Cos(dot) * time;
	UVec4 relVec = AX_Math_Vec4_Sub(end, AX_Math_Vec4_MulScalar(start, dot));
	return AX_Math_Vec4_Add(AX_Math_Vec4_MulScalar(start, AX_Math_Cos(theta)), AX_Math_Vec4_MulScalar(relVec, AX_Math_Sin(theta)));
}

AX_API
UVec4
AX_Math_Vec4_Project
(UVec4 vec, UVec4 normal)
{
	return AX_Math_Vec4_MulScalar(normal, AX_Math_Vec4_Dot(vec, normal) / AX_Math_Vec4_Dot(normal, normal));
}




