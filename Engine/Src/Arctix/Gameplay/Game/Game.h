#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/DataTypes.h"
#include "Arctix/Defines/Core/Math.h"
#include "Arctix/Defines/Core/Game.h" // TODO: relocate this def file to gameplay folder


AX_API
Bool
AX_Gameplay_Game_Init
(SGame *game);

AX_API
Bool
AX_Gameplay_Game_Render
(SGame *game, const Float deltaTime);

AX_API
Bool
AX_Gameplay_Game_Update
(SGame *game, const Float deltaTime);

AX_API
Bool
AX_Gameplay_Game_OnResize
(SGame *game, const Int32 width, const Int32 height);

AX_API
Bool
AX_Gameplay_Game_OnScroll
(SGame *game, const Int8 scrollValue);

AX_API AX_INLINE
ByteSize
AX_Gameplay_Game_GetStateSize
(void);

AX_API
Bool
AX_Gameplay_Game_SetState
(SGame *game, const VoidPtr gameState);



