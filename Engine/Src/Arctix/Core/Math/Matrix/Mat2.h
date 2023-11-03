#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/DataTypes.h"
#include "Arctix/Defines/Core/Math.h"


AX_API AX_INLINE
UMat2
AX_Math_Mat2_Construct
(
	Float x1y1, Float x2y1,
	Float x1y2, Float x2y2
);

AX_API AX_INLINE
UMat2
AX_Math_Mat2_Zero
(void);

AX_API AX_INLINE
UMat2
AX_Math_Mat2_Identity
(void);

AX_API AX_INLINE
UMat2
AX_Math_Mat2_Copy
(UMat2 *mat);

AX_API AX_INLINE
UMat2
AX_Math_Mat2_Mul
(UMat2 left, UMat2 right);




