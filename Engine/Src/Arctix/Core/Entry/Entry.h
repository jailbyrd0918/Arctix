#pragma once

#include "Arctix/Core/Application/Application.h"
#include "Arctix/Gameplay/Game/Game.h"
#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Misc/Assertions/Assertions.h"

#include <stdio.h>


extern
void
AX_Game_Configure
(SGame *game);


int main(int argc, char **argv)
{
	SGame game;
	
	// TODO: temp - remove this 
	game.gameState = AX_HAL_Memory_Malloc(AX_Gameplay_Game_GetStateSize());
	
	AX_Game_Configure(&game);

	AX_ASSERT_MESSAGE(AX_Application_PreConstruct(&game), "application pre_construct stage failed!");
	AX_ASSERT_MESSAGE(AX_Application_Construct(), "application construct stage failed!");
	AX_ASSERT_MESSAGE(AX_Application_Update(), "application update stage failed!");
	AX_ASSERT_MESSAGE(AX_Application_Destruct(), "application destruct stage failed!");

	// TODO: temp - remove this 
	AX_HAL_Memory_Free(game.gameState);

	return 0;
}

