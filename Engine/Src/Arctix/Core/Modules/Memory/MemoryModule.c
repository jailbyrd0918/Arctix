#include "Arctix/Core/Modules/Memory/MemoryModule.h"

#include "Arctix/Core/Misc/String/AXString.h"
#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Hash/AXHash.h"
#include "Arctix/Core/Containers/Array/Array.h"
#include "Arctix/Core/Containers/Map/Map.h"
#include "Arctix/Core/Misc/Console/Console.h"


#define		KILOBYTES_IN_BYTES		(1024)
#define		MEGABYTES_IN_BYTES		(1024 * 1024)
#define		GIGABYTES_IN_BYTES		(1024 * 1024 * 1024)


typedef
struct AX_Module_Memory_State
{
	SMap		allocMap;
	RawString *	allocKeys;

	ByteSize	usedMemory;
	UInt64		numAllocs;

	Bool		isInitialized;
}
AX_Module_Memory_State;

static AX_Module_Memory_State *state;

static RawString memoryTagKeys[AX_MEMORY_TAGS_TOTAL] = {
	"Temp       ",
	"Application",
	"Game       ",
	"Containers ",
	"Allocators ",
	"Delegates  ",
	"Renderer   ",
	"Resources  "
};


AX_API AX_INLINE
ByteSize
AX_Module_Memory_GetModuleSize
(void)
{
	return sizeof(AX_Module_Memory_State);
}

AX_API
Bool
AX_Module_Memory_Startup
(VoidPtr module)
{
	if (!module)
		return false;

	state = AX_CAST(AX_Module_Memory_State *, module);

	// init module state
	{
		state->usedMemory = 0;
		state->numAllocs = 0;

		// allocation map
		{
			state->allocMap = AX_Map_Construct(ByteSize);
			if (!state->allocMap) {
				AX_Map_Destruct(state->allocMap);
				return false;
			}

			// populate allocation usage by engine memory tags
			{
				ByteSize defaultUsedMem = 0;
				for (UInt32 i = 0; i < AX_MEMORY_TAGS_TOTAL; ++i)
					AX_Map_Insert(state->allocMap, memoryTagKeys[i], defaultUsedMem);
			}
		}

		// allocation keys
		{
			state->allocKeys = AX_Array_Construct(RawString);

			// populate engine tag keys
			for (UInt32 i = 0; i < AX_MEMORY_TAGS_TOTAL; ++i)
				AX_Array_Push(state->allocKeys, AX_Misc_String_Strdup(memoryTagKeys[i]));
		}

		state->isInitialized = true;
	}

	return state->isInitialized;
}

AX_API
Bool
AX_Module_Memory_Shutdown
(void)
{
	if (!state || !(state->isInitialized))
		return false;

	// deallocate and set properties to zero
	{
		for (UInt32 i = 0; i < AX_Array_GetLength(state->allocKeys); ++i)
			AX_HAL_Memory_Free(state->allocKeys[i]);
		AX_Array_Destruct(state->allocKeys);

		AX_Map_Destruct(state->allocMap);
		
		state->numAllocs = 0;
		state->usedMemory = 0;
		state->isInitialized = false;
	}

	return true;
}

AX_API
VoidPtr
AX_Module_Memory_Allocate
(ByteSize size, RawString tagKey)
{
	if (state && (state->isInitialized)) {
		state->usedMemory += size;
		state->numAllocs++;

		if (!AX_Map_Contains(state->allocMap, tagKey)) {
			ByteSize newUsedMem = size;
			AX_Map_Insert(state->allocMap, tagKey, newUsedMem);
			AX_Array_Push(state->allocKeys, AX_Misc_String_Strdup(tagKey));
		}
		else {
			ByteSize newUsedMem = AX_Map_GetValue(state->allocMap, ByteSize, tagKey);
			newUsedMem += size;
			AX_Map_SetValue(state->allocMap, tagKey, newUsedMem);
		}
	}

	return AX_HAL_Memory_Malloc(size);
}

AX_API
Bool
AX_Module_Memory_Deallocate
(VoidPtr ptr, ByteSize size, RawString tagKey)
{
	if (!ptr || (size == 0))
		return false;

	if (state && (state->isInitialized)) {
		state->usedMemory -= size;
		state->numAllocs--;

		ByteSize newUsedMem = AX_Map_GetValue(state->allocMap, ByteSize, tagKey);
		newUsedMem -= size;
		AX_Map_SetValue(state->allocMap, tagKey, newUsedMem);
	}

	AX_HAL_Memory_Free(ptr);
	return true;
}

AX_API
void
AX_Module_Memory_ReportMemoryUsage
(void)
{
	AX_Misc_Console_SetForegroundColorRGB(255, 255, 0);
	AX_Misc_Console_WriteLine("============= Memory Usage =============");

	for (UInt32 i = 0; i < AX_Array_GetLength(state->allocKeys); ++i) {
		Char unit[] = "_iB";
		Float fUsedMem = 0.f;

		ByteSize usedMem = AX_Map_GetValue(state->allocMap, ByteSize, state->allocKeys[i]);

		if (usedMem >= GIGABYTES_IN_BYTES) {
			unit[0] = 'G';
			fUsedMem = AX_CAST(Float, usedMem) / GIGABYTES_IN_BYTES;
		}
		else if (usedMem >= MEGABYTES_IN_BYTES) {
			unit[0] = 'M';
			fUsedMem = AX_CAST(Float, usedMem) / MEGABYTES_IN_BYTES;
		}
		else if (usedMem >= KILOBYTES_IN_BYTES) {
			unit[0] = 'K';
			fUsedMem = AX_CAST(Float, usedMem) / KILOBYTES_IN_BYTES;
		}
		else {
			unit[0] = 'B';
			unit[1] = '\0';
			fUsedMem = AX_CAST(Float, usedMem);
		}

		AX_Misc_Console_SetForegroundColorRGB((usedMem == 0) ? 255 : 0, (usedMem == 0) ? 0 : 255, (usedMem == 0) ? 0 : 255);
		AX_Misc_Console_WriteLine("%s: %.2f %s", state->allocKeys[i], fUsedMem, unit);
	}

	AX_Misc_Console_SetForegroundColorRGB(255, 255, 0);
	AX_Misc_Console_WriteLine("========================================");
	AX_Misc_Console_Reset();
}

AX_API AX_INLINE
UInt64
AX_Module_Memory_GetAllocationsCount
(void)
{
	return (state && (state->isInitialized)) ? state->numAllocs : 0;
}

AX_API AX_INLINE
RawString
AX_Module_Memory_GetEngineMemoryTagKey
(EMemTag tag)
{
	return memoryTagKeys[tag];
}



