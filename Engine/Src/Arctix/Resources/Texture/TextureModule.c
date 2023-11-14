#include "Arctix/Resources/Texture/TextureModule.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Misc/String/AXString.h"
#include "Arctix/Core/Containers/Map/Map.h"
#include "Arctix/Core/Containers/Array/Array.h"
#include "Arctix/Core/Containers/String/String.h"
#include "Arctix/Core/Modules/Memory/MemoryModule.h"
#include "Arctix/Core/Delegates/Unicast/Unicast.h"
#include "Arctix/Core/Logging/Logging.h"

#include "Arctix/Renderer/Module/RenderModule.h"

#include <SDL_image.h>


typedef
struct AX_Texture_Module_State
{
	SResourcesConfig *		resourcesConfig;

	STexture			defaultTexture;

	SMap				loadedTextureRefs;
	STexture *			loadedTextures;
}
AX_Texture_Module_State;

typedef
struct AX_Texture_Reference
{
	UInt32				textureID;

	UInt64				refCount;
	Bool				autoRelease;
}
STextureRef;


static AX_Texture_Module_State *state;

static RawString imageExtensions[AX_RESOURCE_TEXTURE_IMAGE_TYPES_TOTAL] = {
	"jpg",
	"png"
};


Bool
_AX_Module_Texture_CreateDefault
(void)
{
	AX_LOG_TRACE("Engine", "Creating default texture...");

	#define	DIMENSION	256
	#define	CHANNELS	4
	#define	GRID_SIZE	32

	AX_RESOURCES_ALLOCATE(UInt8, (DIMENSION * DIMENSION) * CHANNELS, pixels);
	AX_HAL_Memory_Memzero(pixels, (DIMENSION * DIMENSION) * CHANNELS);

	// black and purple checkered texture
	for (UInt64 row = 0; row < DIMENSION; ++row) {
		for (UInt64 col = 0; col < DIMENSION; ++col) {
			UInt64 index = (row * DIMENSION) + col;
			UInt64 bitmapIndex = index * CHANNELS;

			if ((((row / GRID_SIZE) + (col / GRID_SIZE)) % 2) == 0) {
				pixels[bitmapIndex + 0] = 128;
				pixels[bitmapIndex + 1] = 0;
				pixels[bitmapIndex + 2] = 128;
				pixels[bitmapIndex + 3] = 255;
			}
			else {
				pixels[bitmapIndex + 0] = 0;
				pixels[bitmapIndex + 1] = 0;
				pixels[bitmapIndex + 2] = 0;
				pixels[bitmapIndex + 3] = 255;
			}
		}
	}

	if (!AX_Module_Render_CreateTexture(
		AX_RESOURCE_TEXTURE_DEFAULT_NAME,
		DIMENSION,
		DIMENSION,
		CHANNELS,
		pixels,
		false,
		&(state->defaultTexture)
	))
		return false;

	// set id and generation invalid since this is default texture
	state->defaultTexture.generation = AX_INVALID_ID;

	AX_RESOURCES_DEALLOCATE(UInt8, (DIMENSION * DIMENSION) * CHANNELS, pixels);
	AX_LOG_TRACE("Engine", "Default texture created.");
	return true;
}

Bool
_AX_Module_Texture_DestroyDefault
(void)
{
	AX_LOG_TRACE("Engine", "Destroying default texture...");

	if (!AX_Module_Render_DestroyTexture(&(state->defaultTexture)))
		return false;

	AX_LOG_TRACE("Engine", "Default texture destroyed.");
	return true;
}

Bool
_AX_Module_Texture_LoadFromFile
(ReadOnlyString textureName, STexture *outTexture, STextureImageType imageType)
{
	if (!outTexture)
		return false;

	AX_LOG_TRACE("Engine", "Loading texture '%s'...", textureName);

	SString filepathStr = AX_String_ConstructFormatted(
		"%s/%s.%s",
		state->resourcesConfig->fileDir,
		textureName,
		imageExtensions[imageType]
	);

	STexture loadTexture;

	// load texture data
	{
		SDL_Surface *textureSurface = IMG_Load(AX_String_GetString(filepathStr));
		if (!textureSurface) {
			SDL_FreeSurface(textureSurface);
			return false;
		}

		outTexture->width = textureSurface->w;
		outTexture->height = textureSurface->h;
		outTexture->channelCount = textureSurface->format->BytesPerPixel;
		outTexture->hasTransparency = (textureSurface->format->Amask != 0);

		BytePtr pixelData = textureSurface->pixels;
		if (pixelData) {
			UInt32 currentGen = outTexture->generation;
			outTexture->generation = AX_INVALID_ID;

			if (!AX_Module_Render_CreateTexture(
				textureName,
				outTexture->width,
				outTexture->height,
				outTexture->channelCount,
				pixelData,
				outTexture->hasTransparency,
				&loadTexture
			))
				return false;

			// destroy old texture
			{
				STexture oldTexture = *outTexture;
				*outTexture = loadTexture;

				AX_Module_Render_DestroyTexture(&oldTexture);
			}

			// update generation
			{
				if (currentGen == AX_INVALID_ID)
					outTexture->generation = 0;
				else
					outTexture->generation = currentGen + 1;
			}
		}

		SDL_FreeSurface(textureSurface);
	}

	AX_String_Destruct(filepathStr);

	AX_LOG_TRACE("Engine", "Texture '%s' loaded.", textureName);
	return true;


}


AX_API AX_INLINE
ByteSize
AX_Module_Texture_GetModuleSize
(void)
{
	return sizeof(AX_Texture_Module_State);
}

AX_API
Bool
AX_Module_Texture_Startup
(VoidPtr module, const SResourcesConfig *config)
{
	if (!module)
		return false;

	state = AX_CAST(AX_Texture_Module_State *, module);
	state->resourcesConfig = config;

	// texture references
	state->loadedTextureRefs = AX_Map_ConstructReserved(STextureRef, config->maxTextureCount);

	// textures
	{
		state->loadedTextures = AX_Array_ConstructReserved(STexture, config->maxTextureCount);

		for (UInt32 i = 0; i < config->maxTextureCount; ++i) {
			state->loadedTextures[i].id = AX_INVALID_ID;
			state->loadedTextures[i].generation = AX_INVALID_ID;
		}
	}
	
	// create default texture
	if (!_AX_Module_Texture_CreateDefault())
		return false;

	return true;
}

AX_API
Bool
AX_Module_Texture_Shutdown
(void)
{
	if (!state)
		return false;

	// destroy default texture
	if (!_AX_Module_Texture_DestroyDefault())
		return false;

	// textures 
	{
		for (UInt32 i = 0; i < state->resourcesConfig->maxTextureCount; ++i) {
			STexture *texture = &(state->loadedTextures[i]);

			if (texture->generation != AX_INVALID_ID)
				if (!AX_Module_Render_DestroyTexture(texture))
					return false;
		}

		AX_Array_Destruct(state->loadedTextures);
	}

	// texture references
	AX_Map_Destruct(state->loadedTextureRefs);
	
	AX_HAL_Memory_Memzero(state, sizeof(AX_Texture_Module_State));

	return true;
}

AX_API
STexture *
AX_Module_Texture_Acquire
(ReadOnlyString textureName, const Bool autoRelease)
{
	if (!state)
		return NULL;

	if (AX_Misc_String_Strcmp(textureName, AX_RESOURCE_TEXTURE_DEFAULT_NAME, true) == 0) {
		return AX_Module_Texture_GetDefault();
	}

	STextureRef *textureRef = AX_Map_GetValuePtr(state->loadedTextureRefs, STextureRef, textureName);
	if (!textureRef) {
		STextureRef defaultRefValue = {
			.textureID = AX_INVALID_ID,
			.refCount = 0,
			.autoRelease = false
		};

		AX_Map_Insert(state->loadedTextureRefs, textureName, defaultRefValue);
		textureRef = AX_Map_GetValuePtr(state->loadedTextureRefs, STextureRef, textureName);
	}

	if (textureRef->refCount == 0)
		textureRef->autoRelease = autoRelease;

	textureRef->refCount++;

	if (textureRef->textureID == AX_INVALID_ID) {
		STexture *texture = NULL;

		for (UInt32 i = 0; i < state->resourcesConfig->maxTextureCount; ++i) {
			if (state->loadedTextures[i].id == AX_INVALID_ID) {
				textureRef->textureID = i;
				texture = &(state->loadedTextures[i]);
				break;
			}
		}

		if ((!texture) || (textureRef->textureID == AX_INVALID_ID))
			return NULL;

		// load texture
		if (!_AX_Module_Texture_LoadFromFile(textureName, texture, AX_RESOURCE_TEXTURE_IMAGE_TYPE_PNG))
			return NULL;

		texture->id = textureRef->textureID;

		// update the entry
		{
			STextureRef updateRef = *textureRef;
			AX_Map_SetValue(state->loadedTextureRefs, textureName, updateRef);
		}

		AX_LOG_TRACE("Engine", "Texture '%s' doesnt exist. Loaded from file - now ref count: %u", textureName, textureRef->refCount);
		return texture;
	}
		
	AX_LOG_TRACE("Engine", "Texture '%s' already exist. Now ref count: %u", textureName, textureRef->refCount);
	return NULL;
}

AX_API
Bool
AX_Module_Texture_Release
(ReadOnlyString textureName)
{
	if (!state)
		return false;

	if (AX_Misc_String_Strcmp(textureName, AX_RESOURCE_TEXTURE_DEFAULT_NAME, true) == 0)
		return true;

	STextureRef *textureRef = AX_Map_GetValuePtr(state->loadedTextureRefs, STextureRef, textureName);
	if (!textureRef) {
		AX_LOG_WARN("Engine", "Failed to acquire reference to texture '%s'!", textureName);
		return false;
	}

	textureRef->refCount--;

	if ((textureRef->refCount == 0) && (textureRef->autoRelease)) {
		STexture *texture = &(state->loadedTextures[textureRef->textureID]);

		// release texture
		if (!AX_Module_Render_DestroyTexture(texture))
			return false;

		//AX_HAL_Memory_Memzero(texture, sizeof(STexture));
		texture->id = AX_INVALID_ID;
		texture->generation = AX_INVALID_ID;

		textureRef->textureID = AX_INVALID_ID;
		textureRef->autoRelease = false;
	}

	// update the entry
	{
		STextureRef updateRef = *textureRef;
		AX_Map_SetValue(state->loadedTextureRefs, textureName, updateRef);
	}

	AX_LOG_TRACE("Engine", "Texture '%s' released - Now ref count: %u", textureName, textureRef->refCount);
	return true;
}

AX_API
STexture *
AX_Module_Texture_GetDefault
(void)
{
	return (!state) ? NULL : &(state->defaultTexture);
}



