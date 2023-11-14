#pragma once

#include "Arctix/Defines/Resources/Resources.h"


AX_API AX_INLINE
ByteSize
AX_Module_Resources_GetModuleSize
(void);

AX_API AX_INLINE
ByteSize
AX_Module_Resources_GetTotalSize
(void);

AX_API
Bool
AX_Module_Resources_Startup
(VoidPtr module, const SResourcesConfig config, const SMemArena moduleAllocator);

AX_API
Bool
AX_Module_Resources_Shutdown
(void);

