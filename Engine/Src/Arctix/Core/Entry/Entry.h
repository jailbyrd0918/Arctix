#pragma once

#include "Arctix/Core/Application/Application.h"
#include "Arctix/Core/Misc/Assertions/Assertions.h"

#include <stdio.h>


extern
void
AX_Game_Configure
(SGame *game);


int main(int argc, char **argv)
{
	SGame game;
	AX_Game_Configure(&game);

	AX_ASSERT_MESSAGE(AX_Application_PreConstruct(&game), "application pre_construct stage failed!");
	AX_ASSERT_MESSAGE(AX_Application_Construct(), "application construct stage failed!");
	AX_ASSERT_MESSAGE(AX_Application_Update(), "application update stage failed!");
	AX_ASSERT_MESSAGE(AX_Application_Destruct(), "application destruct stage failed!");

	return 0;
}

