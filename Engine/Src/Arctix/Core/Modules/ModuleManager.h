#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/DataTypes.h"
#include "Arctix/Defines/Core/Window.h"
#include "Arctix/Defines/Renderer/Backend.h"


AX_API
Bool
AX_ModuleManager_StartupModules
(SWindowConfig winConfig, SRenderBackendConfig backendConfig, SResourcesConfig resourcesConfig);

AX_API
Bool
AX_ModuleManager_UpdateModules
(void);

AX_API
Bool
AX_ModuleManager_ShutdownModules
(void);



