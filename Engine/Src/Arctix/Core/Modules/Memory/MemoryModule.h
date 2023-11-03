#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Memory.h"


AX_API AX_INLINE
ByteSize
AX_Module_Memory_GetModuleSize
(void);

AX_API
Bool
AX_Module_Memory_Startup
(VoidPtr module);

AX_API
Bool
AX_Module_Memory_Shutdown
(void);

AX_API
VoidPtr
AX_Module_Memory_Allocate
(ByteSize size, RawString tagKey);

AX_API
Bool
AX_Module_Memory_Deallocate
(VoidPtr ptr, ByteSize size, RawString tagKey);

AX_API
void
AX_Module_Memory_ReportMemoryUsage
(void);

AX_API AX_INLINE
UInt64
AX_Module_Memory_GetAllocationsCount
(void);

AX_API AX_INLINE
RawString
AX_Module_Memory_GetEngineMemoryTagKey
(EMemTag tag);





