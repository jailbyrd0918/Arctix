#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/DataTypes.h"
#include "Arctix/Defines/Core/Math.h"


AX_API AX_INLINE
UQuat
AX_Math_Quat_Construct
(Float x, Float y, Float z, Float w);

AX_API AX_INLINE
UQuat
AX_Math_Quat_Identity
(void);

AX_API AX_INLINE
UQuat
AX_Math_Quat_Copy
(UQuat *quat);

AX_API AX_INLINE
UQuat
AX_Math_Quat_ConvertFromAngles
(Float x, Float y, Float z);

AX_API AX_INLINE
UQuat
AX_Math_Quat_ConvertFromRotation
(UVec3 rotation);

AX_API AX_INLINE
UQuat
AX_Math_Quat_ConvertFromAxisAngle
(UVec3 axis, Float angle, Bool normalize);

AX_API AX_INLINE
UMat4
AX_Math_Quat_ConvertToMat4
(UQuat quat);

AX_API AX_INLINE
UMat4
AX_Math_Quat_ConvertToRotationMat
(UQuat quat);

AX_API AX_INLINE
UQuat
AX_Math_Quat_Add
(UQuat left, UQuat right);

AX_API AX_INLINE
UQuat
AX_Math_Quat_Sub
(UQuat left, UQuat right);

AX_API AX_INLINE
UQuat
AX_Math_Quat_Mul
(UQuat left, UQuat right);

AX_API AX_INLINE
UQuat
AX_Math_Quat_MulScalar
(UQuat quat, Float scalar);

AX_API AX_INLINE
Float
AX_Math_Quat_Dot
(UQuat left, UQuat right);

AX_API AX_INLINE
Bool
AX_Math_Quat_Equals
(UQuat left, UQuat right);

AX_API AX_INLINE
Bool
AX_Math_Quat_ApproxEquals
(UQuat left, UQuat right, Float limit);

AX_API AX_INLINE
Float
AX_Math_Quat_MagnitudeSquared
(UQuat quat);

AX_API AX_INLINE
Float
AX_Math_Quat_Magnitude
(UQuat quat);

AX_API
Bool
AX_Math_Quat_Normalize
(UQuat *quat);

AX_API AX_INLINE
UQuat
AX_Math_Quat_Normalized
(UQuat quat);

AX_API AX_INLINE
UQuat
AX_Math_Quat_Conjugate
(UQuat quat);

AX_API AX_INLINE
UQuat
AX_Math_Quat_Quotient
(UQuat left, UQuat right);

AX_API AX_INLINE
Bool
AX_Math_Quat_Inverse
(UQuat *quat);

AX_API AX_INLINE
UQuat
AX_Math_Quat_Inversed
(UQuat quat);

AX_API AX_INLINE
UQuat
AX_Math_Quat_Lerp
(UQuat start, UQuat end, Float time, Bool normalize);

AX_API AX_INLINE
UQuat
AX_Math_Quat_Slerp
(UQuat start, UQuat end, Float time, Bool normalize);


