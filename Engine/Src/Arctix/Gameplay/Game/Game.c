#include "Arctix/Gameplay/Game/Game.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Logging/Logging.h"
#include "Arctix/Core/Math/Helper/MathHelper.h"
#include "Arctix/Core/Math/Matrix/Mat4.h"
#include "Arctix/Core/Math/Vector/Vec3.h"
#include "Arctix/Core/Math/Vector/Vec4.h"

#include "Arctix/Core/HAL/Input/AXInput.h"

#include "Arctix/Renderer/Module/RenderModule.h"

#include <SDL2/SDL.h>


typedef
struct AX_Gameplay_Game_State
{
	UMat4		projection;
	UMat4		view;

	UVec3		cameraPosition;
	UVec3		cameraEulerRotation;

	Bool		cameraViewOutdated;
}
SGameState;

static SGameState *gameState;

static Bool firstMouse = true;
static Float lastMouseX = 0.0f;
static Float lastMouseY = 0.0f;
static Float mouseSensitivity = 0.25f;

static Float fov = 45.0f;

Bool
_AX_Gameplay_Game_RecalculateViewMatrix
(void)
{
	if (!gameState->cameraViewOutdated)
		return false;

	UMat4 rotation = AX_Math_Mat4_MakeRotationXYZ(
		gameState->cameraEulerRotation.x,
		gameState->cameraEulerRotation.y,
		gameState->cameraEulerRotation.z
	);

	UVec3 targetPosition = AX_Math_Vec3_Add(gameState->cameraPosition, AX_Math_Vec3_Forward());
	UMat4 translation = AX_Math_Mat4_MakeTranslation(&(targetPosition));

	gameState->view = AX_Math_Mat4_Mul(&rotation, &translation);
	AX_Math_Mat4_Inverse(&(gameState->view));

	gameState->cameraViewOutdated = false;
	return true;
}

void
_AX_Gameplay_Game_CameraYaw
(const Float value)
{
	gameState->cameraEulerRotation.y += value;
	gameState->cameraViewOutdated = true;
}

void
_AX_Gameplay_Game_CameraPitch
(const Float value)
{
	gameState->cameraEulerRotation.x += value;

	const Float limit = AX_MATH_DEG_TO_RAD(89.0f);
	gameState->cameraEulerRotation.x = AX_MATH_CLAMP(gameState->cameraEulerRotation.x, limit * -1.0f, limit);

	gameState->cameraViewOutdated = true;
}


AX_API
Bool
AX_Gameplay_Game_Init
(SGame *game)
{
	if (!game)
		return false;

	gameState = AX_CAST(SGameState *, game->gameState);

	gameState->cameraPosition = AX_Math_Vec3_Construct(0, 0, 20);
	gameState->cameraEulerRotation = AX_Math_Vec3_Zero();

	gameState->view = AX_Math_Mat4_MakeTranslation(&(gameState->cameraPosition));
	AX_Math_Mat4_Inverse(&(gameState->view));

	gameState->cameraViewOutdated = true;

	return true;
}

AX_API
Bool
AX_Gameplay_Game_Render
(SGame *game, const Float deltaTime)
{
	if (!game)
		return false;

	gameState = AX_CAST(SGameState *, game->gameState);


	return true;
}

AX_API
Bool
AX_Gameplay_Game_Update
(SGame *game, const Float deltaTime)
{
	if (!game)
		return false;

	gameState = AX_CAST(SGameState *, game->gameState);

	// TODO: temp - remove this
	{
		const Float moveSpeed = 20.0f;
		UVec3 velocity = AX_Math_Vec3_Zero();

		if (AX_HAL_Input_IsMouseButtonDown(AX_MOUSECODE_RIGHT)) {
			SDL_ShowCursor(SDL_DISABLE);

			// movement
			{
				if (AX_HAL_Input_IsKeyDown(AX_KEYCODE_W)) {
					UVec3 forward = AX_Math_Mat4_MakeForwardVector(&(gameState->view));
					velocity = AX_Math_Vec3_Add(velocity, forward);
				}

				if (AX_HAL_Input_IsKeyDown(AX_KEYCODE_S)) {
					UVec3 backward = AX_Math_Mat4_MakeBackwardVector(&(gameState->view));
					velocity = AX_Math_Vec3_Add(velocity, backward);
				}

				if (AX_HAL_Input_IsKeyDown(AX_KEYCODE_A)) {
					UVec3 left = AX_Math_Mat4_MakeLeftVector(&(gameState->view));
					velocity = AX_Math_Vec3_Add(velocity, left);
				}

				if (AX_HAL_Input_IsKeyDown(AX_KEYCODE_D)) {
					UVec3 right = AX_Math_Mat4_MakeRightVector(&(gameState->view));
					velocity = AX_Math_Vec3_Add(velocity, right);
				}
			}

			// rotation
			{
				Float mouseX = AX_CAST(Float, AX_HAL_Input_GetMouseX());
				Float mouseY = AX_CAST(Float, AX_HAL_Input_GetMouseY());

				if (firstMouse) {
					lastMouseX = mouseX;
					lastMouseY = mouseY;
					firstMouse = false;
				}

				Float xOffset = mouseX - lastMouseX;
				Float yOffset = lastMouseY - mouseY;

				if ((xOffset != 0) || (yOffset != 0)) {
					lastMouseX = mouseX;
					lastMouseY = mouseY;

					_AX_Gameplay_Game_CameraYaw(xOffset * mouseSensitivity * deltaTime);
					_AX_Gameplay_Game_CameraPitch(yOffset * mouseSensitivity * deltaTime);
				}
			}
		}
		else {
			SDL_ShowCursor(SDL_ENABLE);
			firstMouse = true;
		}

		UVec3 z = AX_Math_Vec3_Zero();
		if (!AX_Math_Vec3_ApproxEquals(z, velocity, 0.0002f)) {
			AX_Math_Vec3_Normalize(&velocity);
			gameState->cameraPosition.x += velocity.x * moveSpeed * deltaTime;
			gameState->cameraPosition.y += velocity.y * moveSpeed * deltaTime;
			gameState->cameraPosition.z += velocity.z * moveSpeed * deltaTime;

			gameState->cameraViewOutdated = true;
		}

		gameState->projection = AX_Math_Mat4_Perspective(
			AX_CAST(Float, AX_MATH_DEG_TO_RAD(fov)),
			AX_CAST(Float, game->gameConfig.winConfig.width) / AX_CAST(Float, game->gameConfig.winConfig.height),
			0.1f,
			1000.f
		);

		AX_Module_Render_SetProjection(gameState->projection);

		if (!_AX_Gameplay_Game_RecalculateViewMatrix())
			return false;
		
		AX_Module_Render_SetView(gameState->view);
	}

	return true;
}

AX_API
Bool
AX_Gameplay_Game_OnResize
(SGame *game, const Int32 width, const Int32 height)
{
	if (!game)
		return false;

	gameState = AX_CAST(SGameState *, game->gameState);


	return true;
}

AX_API
Bool
AX_Gameplay_Game_OnScroll
(SGame *game, const Int8 scrollValue)
{
	if (!game)
		return false;

	fov = AX_MATH_CLAMP(fov - scrollValue, 1.0f, 45.0f);

	return true;
}

AX_API AX_INLINE
ByteSize
AX_Gameplay_Game_GetStateSize
(void)
{
	return sizeof(SGameState);
}

AX_API
Bool
AX_Gameplay_Game_SetState
(SGame *game, const VoidPtr gameState)
{
	if (!game || !gameState)
		return false;

	return (AX_HAL_Memory_Memcpy(game->gameState, gameState, sizeof(SGameState)) != NULL);
}


