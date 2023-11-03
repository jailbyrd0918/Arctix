#include <Arctix.h>

#define	RANDOM_WINDOW_POS	(0x1fff0000u | (0))

#include <stdio.h>


void OnStart (SGame *gameInst)
{
}

void OnUpdate (SGame *gameInst, const float deltaTime)
{
}


void
AX_Game_Configure
(SGame *game)
{
	SWindowConfig winConfig = {
		.x = RANDOM_WINDOW_POS,
		.y = RANDOM_WINDOW_POS,
		.width = 1200,
		.height = 800,
		.title = "Arctix_Engine_Sandbox"
	};

	SRenderBackendConfig backendConfig = {
		.appName = "Arctix",
		.mode = AX_RENDER_BACKEND_VULKAN
	};

	SGameConfig gameConfig = {
		.winConfig = winConfig,
		.backendConfig = backendConfig,
		.maxFrameRate = 120,
		.onStart = OnStart,
		.onUpdate = OnUpdate
	};

	game->gameConfig = gameConfig;
}


