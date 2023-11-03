#pragma once

#include "Arctix/Defines/Core/DataTypes.h"

#include <stdio.h>


typedef
enum AX_Filesystem_File_IO_Mode
{
	AX_FILE_IO_MODE_READ,
	AX_FILE_IO_MODE_WRITE,
	AX_FILE_IO_MODE_READ_WRITE,
	AX_FILE_IO_MODE_APPEND
}
EFileIOMode;

typedef
enum AX_Filesystem_File_Mode
{
	AX_FILE_MODE_TEXT,
	AX_FILE_MODE_BINARY
}
EFileMode;


typedef	FILE *	SFileStream;

typedef
struct AX_Filesystem_File_Handle
{
	SFileStream	stream;
	Bool		isActive;
}
SFileHandle;

