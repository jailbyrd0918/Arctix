#pragma once

#include "Arctix/Defines/Core/DataTypes.h"
#include "Arctix/Defines/Core/Window.h"
#include "Arctix/Defines/Resources/Resources.h"
#include "Arctix/Defines/Renderer/Backend.h"


typedef	struct AX_Game	SGame;

typedef
struct AX_Game_Configuration
{
	SWindowConfig
		winConfig;

	SRenderBackendConfig
		backendConfig;

	SResourcesConfig
		resourcesConfig;

	UInt32
		maxFrameRate;

	void	(*onStart)(SGame *self),
		(*onUpdate)(SGame *self, const Float deltaTime);
}
SGameConfig;

struct AX_Game
{
	SGameConfig
		gameConfig;

	VoidPtr
		gameState,
		appState;
};



