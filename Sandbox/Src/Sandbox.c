#include <Arctix.h>

#define	RANDOM_WINDOW_POS	(0x1fff0000u | (0))

#include <stdio.h>


void OnStart (SGame *gameInst)
{
	AX_Gameplay_Game_Init(gameInst);
}

void OnUpdate (SGame *gameInst, const float deltaTime)
{
	AX_Gameplay_Game_Update(gameInst, deltaTime);
	AX_Gameplay_Game_Render(gameInst, deltaTime);
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

	SResourcesConfig resourcesConfig = {
		.fileDir = "Assets/Textures", // TODO: remove the 'Texture' part (since the file is about all assets in general
		.maxTextureCount = 5
	};

	SGameConfig gameConfig = {
		.winConfig = winConfig,
		.backendConfig = backendConfig,
		.resourcesConfig = resourcesConfig,

		.maxFrameRate = 120,

		.onStart = OnStart,
		.onUpdate = OnUpdate
	};

	game->gameConfig = gameConfig;
}


