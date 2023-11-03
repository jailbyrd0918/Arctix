#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/DataTypes.h"


AX_API AX_INLINE
ByteSize
AX_Module_Window_GetModuleSize
(void);

AX_API
Bool
AX_Module_Window_Startup
(VoidPtr module, ReadOnlyString title, const Int32 x, const Int32 y, const Int32 width, const Int32 height);

AX_API
Bool
AX_Module_Window_Shutdown
(void);

AX_API
Bool
AX_Module_Window_Update
(void);

AX_API AX_INLINE
VoidPtr
AX_Module_Window_GetModuleState
(void);

AX_API
Bool
AX_Module_Window_SetModuleState
(VoidPtr newState);



