#include "Arctix/Core/Math/Helper/MathHelper.h"

#include <math.h>


#define	MATH_RSQRT_CONSTANT	1597463007U


AX_API AX_INLINE
Float
AX_Math_Sin
(Float value)
{
	return sinf(value);
}

AX_API AX_INLINE
Float
AX_Math_Cos
(Float value)
{
	return cosf(value);
}

AX_API AX_INLINE
Float
AX_Math_Tan
(Float value)
{
	return tanf(value);
}

AX_API AX_INLINE
Float
AX_Math_Asin
(Float value)
{
	return asinf(value);
}

AX_API AX_INLINE
Float
AX_Math_Acos
(Float value)
{
	return acosf(value);
}

AX_API AX_INLINE
Float
AX_Math_Atan
(Float value)
{
	return atanf(value);
}

AX_API AX_INLINE
Float
AX_Math_Atan2
(Float x, Float y)
{
	return atan2f(y, x);
}

AX_API AX_INLINE
Float
AX_Math_Sqrt
(Float value)
{
	return sqrtf(value);
}

AX_API AX_INLINE
Float
AX_Math_Rsqrt
(Float value)
{
	Int32 i = MATH_RSQRT_CONSTANT - (*AX_CAST(Int32 *, &value) >> 1);
	return *AX_CAST(Float *, &i) * (1.5f - (value * 0.5f) * AX_Math_Pow(*AX_CAST(Float *, &i), 2.0f));
}

AX_API AX_INLINE
Float
AX_Math_Abs
(Float value)
{
	return fabsf(value);
}

AX_API AX_INLINE
Float
AX_Math_Exp
(Float value)
{
	return expf(value);
}

AX_API AX_INLINE
Float
AX_Math_Pow
(Float value, Float power)
{
	return powf(value, power);
}

AX_API AX_INLINE
Float
AX_Math_Log
(Float value)
{
	return logf(value);
}

AX_API AX_INLINE
Float
AX_Math_Log2
(Float value)
{
	return log2f(value);
}

AX_API AX_INLINE
Float
AX_Math_Log10
(Float value)
{
	return log10f(value);
}

AX_API AX_INLINE
Float
AX_Math_Ceil
(Float value)
{
	return ceilf(value);
}

AX_API AX_INLINE
Float
AX_Math_Floor
(Float value)
{
	return floorf(value);
}

AX_API AX_INLINE
Float
AX_Math_Round
(Float value)
{
	return roundf(value);
}

AX_API AX_INLINE
Float
AX_Math_RoundTo
(Float value, Int32 decimalPlaces)
{
	return
		(decimalPlaces < 0) ?
		value :
		AX_CAST(Float, AX_CAST(Int32, value * AX_Math_Pow(10.f, AX_CAST(Float, decimalPlaces)) + 0.5f) / AX_Math_Pow(10.f, AX_CAST(Float, decimalPlaces)));
}

AX_API AX_INLINE
Float
AX_Math_Mod
(Float x, Float y)
{
	return fmodf(x, y);
}

AX_API AX_INLINE
Int32
AX_Math_Sign
(Float value)
{
	return ((value > 0) - (value < 0));
}

AX_API AX_INLINE
Bool
AX_Math_IsPowerOfTwo
(UInt64 value)
{
	return ((value > 0) && (!(value & (value - 1))));
}

AX_API AX_INLINE
Bool
AX_Math_EpsilonApprox
(Float left, Float right)
{
	return (AX_Math_Abs(left - right) < AX_MATH_EPSILON);
}

