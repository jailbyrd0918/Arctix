#pragma once

#include "Arctix/Defines/Resources/Texture.h"


AX_API AX_INLINE
ByteSize
AX_Module_Texture_GetModuleSize
(void);

AX_API
Bool
AX_Module_Texture_Startup
(VoidPtr module, const SResourcesConfig *config);

AX_API
Bool
AX_Module_Texture_Shutdown
(void);

AX_API
STexture *
AX_Module_Texture_Acquire
(ReadOnlyString textureName, const Bool autoRelease);

AX_API
Bool
AX_Module_Texture_Release
(ReadOnlyString textureName);

AX_API
STexture *
AX_Module_Texture_GetDefault
(void);


