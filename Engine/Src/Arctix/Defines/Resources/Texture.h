#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/DataTypes.h"
#include "Arctix/Defines/Core/Hash.h"


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




