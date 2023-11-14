#pragma once

#include "Arctix/Defines/Resources/Resources.h"
#include "Arctix/Defines/Core/Hash.h"


#define	AX_RESOURCE_TEXTURE_DEFAULT_NAME	"texture_default"

typedef
enum AX_Resource_Texture_Image_Type
{
	AX_RESOURCE_TEXTURE_IMAGE_TYPE_JPEG,
	AX_RESOURCE_TEXTURE_IMAGE_TYPE_PNG,

	AX_RESOURCE_TEXTURE_IMAGE_TYPES_TOTAL
}
STextureImageType;

typedef
struct AX_Resource_Texture
{
	HashedInt	id;

	UInt32		width, height;

	UInt8		channelCount;
	Bool		hasTransparency;
	UInt32		generation;

	VoidPtr		data;
}
STexture;




