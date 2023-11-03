#include "Arctix/Core/Math/Vector/Vec2.h"

#include "Arctix/Core/Math/Helper/MathHelper.h"


AX_API AX_INLINE
UVec2
AX_Math_Vec2_Construct
(Float x, Float y)
{
	return AX_STRUCT(UVec2, x, y);
}

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Zero
(void)
{
	return AX_STRUCT(UVec2, 0.0f);
}

AX_API AX_INLINE
UVec2
AX_Math_Vec2_One
(void)
{
	return AX_STRUCT(UVec2, 1.0f);
}

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Scale
(Float scale)
{
	return AX_STRUCT(UVec2, scale);
}

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Up
(void)
{
	return AX_STRUCT(UVec2, 0.0f, 1.0f);
}

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Down
(void)
{
	return AX_STRUCT(UVec2, 0.0f, -1.0f);
}

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Left
(void)
{
	return AX_STRUCT(UVec2, -1.0f, 0.0f);
}

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Right
(void)
{
	return AX_STRUCT(UVec2, 1.0f, 0.0f);
}

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Copy
(UVec2 *vec)
{
	return (!vec) ? AX_Math_Vec2_Zero() : (*vec);
}

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Add
(UVec2 left, UVec2 right)
{
	return AX_STRUCT(UVec2, left.x + right.x, left.y + right.y);
}

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Sub
(UVec2 left, UVec2 right)
{
	return AX_STRUCT(UVec2, left.x - right.x, left.y - right.y);
}

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Mul
(UVec2 left, UVec2 right)
{
	return AX_STRUCT(UVec2, left.x * right.x, left.y * right.y);
}

AX_API AX_INLINE
UVec2
AX_Math_Vec2_MulScalar
(UVec2 vec, Float scalar)
{
	return AX_STRUCT(UVec2, vec.x * scalar, vec.y * scalar);
}

AX_API AX_INLINE
UVec2
AX_Math_Vec2_MulAdd
(UVec2 vec1, UVec2 vec2, UVec2 vec3)
{
	return AX_STRUCT(
		UVec2,
		vec1.x * vec2.x + vec3.x,
		vec1.y * vec2.y + vec3.y
	);
}

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Div
(UVec2 left, UVec2 right)
{
	return
		((right.x == 0) || (right.y == 0)) ?
		left :
		AX_STRUCT(UVec2, left.x / right.x, left.y / right.y);
}

AX_API AX_INLINE
UVec2
AX_Math_Vec2_DivScalar
(UVec2 vec, Float scalar)
{
	return
		(scalar == 0) ?
		vec :
		AX_STRUCT(UVec2, vec.x / scalar, vec.y / scalar);
}

AX_API AX_INLINE
Float
AX_Math_Vec2_MagnitudeSquared
(UVec2 vec)
{
	return (AX_Math_Pow(vec.x, 2.0f) + AX_Math_Pow(vec.y, 2.0f));
}

AX_API AX_INLINE
Float
AX_Math_Vec2_Magnitude
(UVec2 vec)
{
	return AX_Math_Sqrt(AX_Math_Vec2_MagnitudeSquared(vec));
}

AX_API
Bool
AX_Math_Vec2_Normalize
(UVec2 *vec)
{
	if (!vec)
		return false;

	Float length = AX_Math_Vec2_Magnitude(*vec);

	vec->x /= length;
	vec->y /= length;

	return true;
}

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Normalized
(UVec2 vec)
{
	AX_Math_Vec2_Normalize(&vec);
	return vec;
}

AX_API AX_INLINE
Bool
AX_Math_Vec2_Equals
(UVec2 left, UVec2 right)
{
	return (((left.x - right.x) < AX_MATH_EPSILON) && ((left.y - right.y) < AX_MATH_EPSILON));
}

AX_API AX_INLINE
Bool
AX_Math_Vec2_ApproxEquals
(UVec2 left, UVec2 right, Float limit)
{
	return (((left.x - right.x) <= limit) && ((left.y - right.y) <= limit));
}

AX_API AX_INLINE
Float
AX_Math_Vec2_Distance
(UVec2 left, UVec2 right)
{
	return AX_Math_Vec2_Magnitude(AX_Math_Vec2_Sub(left, right));
}

AX_API AX_INLINE
Float
AX_Math_Vec2_DistanceSquared
(UVec2 left, UVec2 right)
{
	return AX_Math_Vec2_MagnitudeSquared(AX_Math_Vec2_Sub(left, right));
}

AX_API AX_INLINE
Float
AX_Math_Vec2_Dot
(UVec2 left, UVec2 right)
{
	return ((left.x * right.x) + (left.y * right.y));
}

AX_API AX_INLINE
Float
AX_Math_Vec2_Cross
(UVec2 left, UVec2 right)
{
	return ((left.x * right.y) - (left.y * right.x));
}

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Lerp
(UVec2 start, UVec2 end, Float time)
{
	return AX_Math_Vec2_Add(AX_Math_Vec2_MulScalar(start, 1.0f - time), AX_Math_Vec2_MulScalar(end, time));
}

AX_API
UVec2
AX_Math_Vec2_Slerp
(UVec2 start, UVec2 end, Float time)
{
	Float dot = AX_MATH_CLAMP(AX_Math_Vec2_Dot(start, end), -1.f, 1.f);
	Float theta = AX_Math_Cos(dot) * time;
	UVec2 relVec = AX_Math_Vec2_Sub(end, AX_Math_Vec2_MulScalar(start, dot));
	return AX_Math_Vec2_Add(AX_Math_Vec2_MulScalar(start, AX_Math_Cos(theta)), AX_Math_Vec2_MulScalar(relVec, AX_Math_Sin(theta)));
}

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Project
(UVec2 vec, UVec2 normal)
{
	return AX_Math_Vec2_MulScalar(normal, AX_Math_Vec2_Dot(vec, normal) / AX_Math_Vec2_Dot(normal, normal));
}

AX_API AX_INLINE
UVec2
AX_Math_Vec2_Reflect
(UVec2 vec, UVec2 normal)
{
	return AX_Math_Vec2_Sub(vec, AX_Math_Vec2_MulScalar(normal, AX_Math_Vec2_Dot(vec, normal) * 2.0f));
}





