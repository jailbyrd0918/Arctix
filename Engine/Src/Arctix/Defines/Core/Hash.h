#pragma once

#include "Arctix/Defines/Core/DataTypes.h"


// -- hash value -- //

typedef		UInt64		HashedInt;


// -- hash mode enum -- //

typedef
enum AX_HashMode
{
	AX_HASH_MODE_CITY64,
	AX_HASH_MODE_FNVA64,
	AX_HASH_MODE_MURMUR64,
	AX_HASH_MODE_XX64,
	AX_HASH_MODE_TOTAL
}
EHashMode;


