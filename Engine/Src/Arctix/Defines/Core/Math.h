#pragma once

#include "Arctix/Defines/Core/DataTypes.h"


// -- vectors -- //

typedef
union AX_Math_Vector2
{
	struct
	{
		// first component
		union {
			Float
				x,
				w,
				u;
		};

		// second component
		union {
			Float
				y,
				h,
				v;
		};
	};

	Float	elements[2];
}
UVec2;

typedef
union AX_Math_Vector3
{
	struct
	{
		// first component
		union {
			Float
				x,
				r;
		};

		// second component
		union {
			Float
				y,
				g;
		};

		// third component
		union {
			Float
				z,
				b;
		};
	};

	Float	elements[3];
}
UVec3;

typedef
union AX_Math_Vector4
{
	struct
	{
		// first component
		union {
			Float
				x,
				r;
		};

		// second component
		union {
			Float
				y,
				g;
		};

		// third component
		union {
			Float
				z,
				b;
		};

		// fourth component
		union {
			Float
				w,
				a;
		};
	};

	Float	elements[4];
}
UVec4;


// -- matrices -- //

typedef
union AX_Math_Matrix_2x2
{
	struct {
		Float	x1y1;
		Float	x2y1;

		Float	x1y2;
		Float	x2y2;
	};

	Float	elements[4];
}
UMat2;

typedef
union AX_Math_Matrix_3x3
{
	struct {
		Float	x1y1;
		Float	x2y1;
		Float	x3y1;

		Float	x1y2;
		Float	x2y2;
		Float	x3y2;

		Float	x1y3;
		Float	x2y3;
		Float	x3y3;
	};

	Float	elements[9];
}
UMat3;

typedef
union AX_Math_Matrix_4x4
{
	struct {
		Float	x1y1;
		Float	x2y1;
		Float	x3y1;
		Float	x4y1;

		Float	x1y2;
		Float	x2y2;
		Float	x3y2;
		Float	x4y2;

		Float	x1y3;
		Float	x2y3;
		Float	x3y3;
		Float	x4y3;

		Float	x1y4;
		Float	x2y4;
		Float	x3y4;
		Float	x4y4;
	};

	Float	elements[16];
}
UMat4;


// -- quaternion -- //

typedef		UVec4		UQuat;



// -- macro operations -- //

#if	defined(M_PI)
#	define	AX_MATH_PI		M_PI
#else
#	define	AX_MATH_PI		3.14159265358979323846264338327950288
#endif

#define AX_MATH_DEG_TO_RAD(degree)	((degree * AX_MATH_PI) / 180.0f)
#define AX_MATH_RAD_TO_DEG(radian)	((radian * 180.0f) / AX_MATH_PI)

#define	AX_MATH_LARGE_NUMBER		(1e30f * 1e30f)

#if	defined(FLT_EPSILON)
#	define	ARCITX_MATH_EPSILON	FLT_EPSILON
#else
#	define	AX_MATH_EPSILON		1.1920928955078125e-07f
#endif

#define	AX_MATH_MAX(left, right)	((left > right) ? left : right)
#define	AX_MATH_MIN(left, right)	((left < right) ? left : right)
#define	AX_MATH_CLAMP(value, min, max)	((value > max) ? max : ((value < min) ? min : value))



