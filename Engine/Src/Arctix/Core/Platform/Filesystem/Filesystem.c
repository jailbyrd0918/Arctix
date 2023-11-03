#include "Arctix/Core/Platform/Filesystem/Filesystem.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Misc/String/AXString.h"

#include <stdio.h>

#if	defined(_WIN32) || defined(_WIN64)
#	include <Windows.h>

#else
#	include <sys/stat.h>

#endif


Bool
AX_Filesystem_OpenFile
(ReadOnlyString filepath, const EFileIOMode ioMode, const EFileMode mode, SFileHandle *handle)
{
	ReadOnlyString fileMode;

	switch (ioMode) {
		case AX_FILE_IO_MODE_READ_WRITE:
			fileMode = (mode == AX_FILE_MODE_BINARY) ? "w+b" : "w+";
			break;

		case AX_FILE_IO_MODE_READ:
			fileMode = (mode == AX_FILE_MODE_BINARY) ? "rb" : "r";
			break;

		case AX_FILE_IO_MODE_WRITE:
			fileMode = (mode == AX_FILE_MODE_BINARY) ? "wb" : "w";
			break;

		case AX_FILE_IO_MODE_APPEND:
			fileMode = (mode == AX_FILE_MODE_BINARY) ? "a+b" : "a";
			break;

		default:
			return false;
	}

	SFileStream inFile = fopen(filepath, fileMode);
	if (!inFile)
		return false;

	handle->stream = inFile;
	handle->isActive = true;

	return true;
}

Bool
AX_Filesystem_CloseFile
(SFileHandle *handle)
{
	if (!handle || !(handle->stream))
		return false;

	fclose(handle->stream);
	handle->isActive = false;

	return true;
}

Bool
AX_Filesystem_FileExists
(ReadOnlyString filepath)
{
	Bool fileExists = false;
	SFileStream stream;

	if ((stream = fopen(filepath, "r")) != NULL) {
		fileExists = true;
		fclose(stream);
	}

	return fileExists;
}

ByteSize
AX_Filesystem_GetFileSize
(SFileHandle *handle)
{
	if (!handle || !(handle->stream))
		return false;

	ByteSize fileSize = 0;
	SFileStream file = handle->stream;

	fseek(file, 0, SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	return fileSize;
}

Bool
AX_Filesystem_ReadLineFromFile
(SFileHandle *handle, const Int32 bufferMaxLength, RawString *lineBuffer, ByteSize *lineBufferLength)
{
	if (!handle || !(handle->stream))
		return false;

	if ((lineBuffer != NULL) && (lineBufferLength != NULL) && (bufferMaxLength > 0)) {
		if (fgets(*lineBuffer, bufferMaxLength, handle->stream) != NULL) {
			*lineBufferLength = AX_Misc_String_Strlen(*lineBuffer);
			return true;
		}
	}

	return false;
}

Bool
AX_Filesystem_WriteLineToFile
(SFileHandle *handle, ReadOnlyString text)
{
	if (!handle || !(handle->stream))
		return false;

	SFileStream file = handle->stream;

	Int32 result = fprintf(file, "%s", text);
	if (result != EOF)
		result = fputc('\n', file);

	fflush(file);
	return (result != EOF);
}

RawString
AX_Filesystem_ReadAllTextFromFile
(SFileHandle *handle, ByteSize *readBytes)
{
	if (!handle || !(handle->stream))
		return false;

	RawString outText = AX_HAL_Memory_Malloc(AX_Filesystem_GetFileSize(handle));
	if (!outText) {
		AX_HAL_Memory_Free(outText);
		return NULL;
	}

	if (readBytes)
		*readBytes = fread(outText, sizeof(Char), AX_Filesystem_GetFileSize(handle), handle->stream);
	else
		fread(outText, sizeof(Char), AX_Filesystem_GetFileSize(handle), handle->stream);

	return outText;
}

VoidPtr
AX_Filesystem_ReadDataFromFile
(SFileHandle *handle, ByteSize dataSize, ByteSize *readBytes)
{
	if (!handle || !(handle->stream))
		return false;

	VoidPtr outData = AX_HAL_Memory_Malloc(AX_Filesystem_GetFileSize(handle));
	if (!outData) {
		AX_HAL_Memory_Free(outData);
		return NULL;
	}

	if (readBytes)
		*readBytes = fread(outData, dataSize, AX_Filesystem_GetFileSize(handle) / dataSize, handle->stream);
	else
		fread(outData, dataSize, AX_Filesystem_GetFileSize(handle) / dataSize, handle->stream);

	return outData;
}

Bool
AX_Filesystem_WriteDataToFile
(SFileHandle *handle, const VoidPtr data, const ByteSize dataSize)
{
	if (!handle || !(handle->stream))
		return false;

	SFileStream file = handle->stream;
	fwrite(data, sizeof(Char), dataSize, file);

	fflush(file);
	return true;
}

AX_API AX_INLINE
ReadOnlyString
AX_Filesystem_ExtractFilename
(ReadOnlyString filepath)
{
	return AX_CAST(
		ReadOnlyString,
		AX_Misc_String_Strrchr(AX_CAST(RawString, filepath), '\\') ?
		AX_Misc_String_Strrchr(AX_CAST(RawString, filepath), '\\') + 1 :
		AX_Misc_String_Strrchr(AX_CAST(RawString, filepath), '/') ?
		AX_Misc_String_Strrchr(AX_CAST(RawString, filepath), '/') + 1 : filepath
	);
}

AX_API AX_INLINE
Bool
AX_Filesystem_CreateDirectory
(ReadOnlyString dirName)
{
	#if	defined(_WIN32) || defined(_WIN64)
	return CreateDirectoryA(dirName, NULL);

	#else
	return (mkdir(dirName, 0777) == 0);

	#endif
}
