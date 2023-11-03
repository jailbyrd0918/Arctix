#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Containers.h"
#include "Arctix/Defines/Core/Window.h"
#include "Arctix/Defines/Core/Game.h"
#include "Arctix/Defines/Renderer/Backend.h"


AX_API
Bool
AX_Application_PreConstruct
(SGame *game);

AX_API
Bool
AX_Application_Construct
(void);

AX_API
Bool
AX_Application_Update
(void);

AX_API
Bool
AX_Application_Destruct
(void);


