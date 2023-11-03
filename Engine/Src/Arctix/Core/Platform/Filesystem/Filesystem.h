#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Filesystem.h"


Bool
AX_Filesystem_OpenFile
(ReadOnlyString filepath, const EFileIOMode ioMode, const EFileMode mode, SFileHandle *handle);

Bool
AX_Filesystem_CloseFile
(SFileHandle *handle);

Bool
AX_Filesystem_FileExists
(ReadOnlyString filepath);

ByteSize
AX_Filesystem_GetFileSize
(SFileHandle *handle);

Bool
AX_Filesystem_ReadLineFromFile
(SFileHandle *handle, const Int32 bufferMaxLength, RawString *lineBuffer, ByteSize *lineBufferLength);

Bool
AX_Filesystem_WriteLineToFile
(SFileHandle *handle, ReadOnlyString text);

RawString
AX_Filesystem_ReadAllTextFromFile
(SFileHandle *handle, ByteSize *readBytes);

VoidPtr
AX_Filesystem_ReadDataFromFile
(SFileHandle *handle, ByteSize dataSize, ByteSize *readBytes);

Bool
AX_Filesystem_WriteDataToFile
(SFileHandle *handle, const VoidPtr data, const ByteSize dataSize);

AX_API AX_INLINE
ReadOnlyString
AX_Filesystem_ExtractFilename
(ReadOnlyString filepath);

AX_API AX_INLINE
Bool
AX_Filesystem_CreateDirectory
(ReadOnlyString dirName);




