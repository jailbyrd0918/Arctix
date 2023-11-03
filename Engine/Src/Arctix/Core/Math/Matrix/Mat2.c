#include "Arctix/Core/Math/Matrix/Mat2.h"


AX_API AX_INLINE
UMat2
AX_Math_Mat2_Construct
(
	Float x1y1, Float x2y1,
	Float x1y2, Float x2y2
)
{
	return AX_STRUCT(
		UMat2,
		x1y1, x2y1,
		x1y2, x2y2
	);
}

AX_API AX_INLINE
UMat2
AX_Math_Mat2_Zero
(void)
{
	return AX_STRUCT(UMat2, 0);
}

AX_API AX_INLINE
UMat2
AX_Math_Mat2_Identity
(void)
{
	return AX_STRUCT(
		UMat2,
		1, 0,
		0, 1
	);
}

AX_API AX_INLINE
UMat2
AX_Math_Mat2_Copy
(UMat2 *mat)
{
	return (!mat) ? AX_Math_Mat2_Zero() : (*mat);
}

AX_API AX_INLINE
UMat2
AX_Math_Mat2_Mul
(UMat2 left, UMat2 right)
{
	return AX_STRUCT(
		UMat2,
		left.x1y1 * right.x1y1 + left.x2y1 * right.x1y2,
		left.x1y1 * right.x2y1 + left.x2y1 * right.x2y2,
		left.x1y2 * right.x1y1 + left.x2y2 * right.x1y2,
		left.x1y2 * right.x2y1 + left.x2y2 * right.x2y2
	);
}

