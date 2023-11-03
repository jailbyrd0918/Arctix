#include "Arctix/Core/Misc/Console/Console.h"

#include "Arctix/Core/HAL/Memory/AXMemory.h"
#include "Arctix/Core/Misc/String/AXString.h"

#include "Arctix/Core/Containers/String/String.h"

#include <stdio.h>

#if	defined(_WIN32) || defined(_WIN64)
#	include <Windows.h>
#endif


#define	OUTPUT_BUFFER_MAX_SIZE		512


#if	defined(_WIN32) || defined(_WIN64)
#	if	!defined(ENABLE_VIRTUAL_TERMINAL_PROCESSING)
#	define ENABLE_VIRTUAL_TERMINAL_PROCESSING  0x0004
#	endif

static	HANDLE	outHandle = NULL;
static	DWORD	consoleOutMode = 0;


AX_API
void
AX_Misc_Console_Startup
(void)
{
	outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (outHandle == INVALID_HANDLE_VALUE)
		exit(GetLastError());

	if (!GetConsoleMode(outHandle, &consoleOutMode))
		exit(GetLastError());

	// set the console mode to enable ANSI escape codes
	DWORD outMode = consoleOutMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(outHandle, outMode))
		exit(GetLastError());
}

AX_API
void
AX_Misc_Console_Shutdown
(void)
{
	// restore console to default state
	AX_Misc_Console_Reset();

	// reset the console mode
	if (!SetConsoleMode(outHandle, consoleOutMode))
		exit(GetLastError());
}


#else

AX_API
void
AX_Misc_Console_Startup
(void)
{
	// do nothing
	return;
}

AX_API
void
AX_Misc_Console_Shutdown
(void)
{
	// restore console to default state
	AX_Misc_Console_Reset();
}


#endif


AX_API AX_INLINE
void
AX_Misc_Console_Reset
(void)
{
	fprintf(stdout, "\x1b[0m");
}


// -- color -- //

AX_API AX_INLINE
void
AX_Misc_Console_SetForegroundColor
(Int32 code)
{
	fprintf(stdout, "\x1b[38;5;%dm", code);
}

AX_API AX_INLINE
void
AX_Misc_Console_SetBackgroundColor
(Int32 code)
{
	fprintf(stdout, "\x1b[48;5;%dm", code);
}

AX_API AX_INLINE
void
AX_Misc_Console_SetForegroundColorRGB
(Int32 r, Int32 g, Int32 b)
{
	fprintf(stdout, "\x1b[38;2;%d;%d;%dm", r, g, b);
}

AX_API AX_INLINE
void
AX_Misc_Console_SetBackgroundColorRGB
(Int32 r, Int32 g, Int32 b)
{
	fprintf(stdout, "\x1b[48;2;%d;%d;%dm", r, g, b);
}


// -- clear -- //

enum ClearMode {
	CLEARMODE_FROM_CURSOR_TO_END,
	CLEARMODE_FROM_CURSOR_TO_BEGIN,
	CLEARMODE_ALL
};


AX_API
void
AX_Misc_Console_ClearScreen
(void)
{
	fprintf(stdout, "\x1b[%dJ", CLEARMODE_ALL);
}

AX_API
void
AX_Misc_Console_ClearScreenToTop
(void)
{
	fprintf(stdout, "\x1b[%dJ", CLEARMODE_FROM_CURSOR_TO_BEGIN);
}

AX_API
void
AX_Misc_Console_ClearScreenToBottom
(void)
{
	fprintf(stdout, "\x1b[%dJ", CLEARMODE_FROM_CURSOR_TO_END);
}

AX_API
void
AX_Misc_Console_ClearLine
(void)
{
	fprintf(stdout, "\x1b[%dK", CLEARMODE_ALL);
}

AX_API
void
AX_Misc_Console_ClearLineToLeft
(void)
{
	fprintf(stdout, "\x1b[%dK", CLEARMODE_FROM_CURSOR_TO_BEGIN);
}

AX_API
void
AX_Misc_Console_ClearLineToRight
(void)
{
	fprintf(stdout, "\x1b[%dK", CLEARMODE_FROM_CURSOR_TO_END);
}


// -- cursor -- //

AX_API
void
AX_Misc_Console_MoveCursorUp
(Int32 positions)
{
	fprintf(stdout, "\x1b[%dA", positions);
}

AX_API
void
AX_Misc_Console_MoveCursorDown
(Int32 positions)
{
	fprintf(stdout, "\x1b[%dB", positions);
}

AX_API
void
AX_Misc_Console_MoveCursorLeft
(Int32 positions)
{
	fprintf(stdout, "\x1b[%dD", positions);
}

AX_API
void
AX_Misc_Console_MoveCursorRight
(Int32 positions)
{
	fprintf(stdout, "\x1b[%dC", positions);
}

AX_API
void
AX_Misc_Console_MoveCursorTo
(Int32 row, Int32 column)
{
	fprintf(stdout, "\x1b[%d;%df", row, column);
}


// -- input -- //

AX_API
SString
AX_Misc_Console_ReadLine
(void)
{
	Char buffer[OUTPUT_BUFFER_MAX_SIZE] = { '\0' };
	fgets(buffer, OUTPUT_BUFFER_MAX_SIZE, stdin);
	fflush(stdin);

	// replace newline character with null-terminator
	{
		ByteSize length = AX_Misc_String_Strlen(buffer);
		buffer[length - 1] = '\0';
	}

	SString ret = AX_String_Construct(buffer);
	return ret;
}


// -- output -- //

AX_API
void
AX_Misc_Console_Write
(ReadOnlyString format, ...)
{
	VaList argList;
	Char buffer[OUTPUT_BUFFER_MAX_SIZE] = { '\0' };
	
	va_start(argList, format);
	vsnprintf(buffer, OUTPUT_BUFFER_MAX_SIZE, format, argList);
	va_end(argList);

	fprintf(stdout, "%s", buffer);
}

AX_API
void
AX_Misc_Console_WriteLine
(ReadOnlyString format, ...)
{
	VaList argList;
	Char buffer[OUTPUT_BUFFER_MAX_SIZE] = { '\0' };

	va_start(argList, format);
	vsnprintf(buffer, OUTPUT_BUFFER_MAX_SIZE, format, argList);
	va_end(argList);

	fprintf(stdout, "%s\n", buffer);
	fflush(stdout);
}


// -- miscellaneous -- //

AX_API AX_INLINE
void
AX_Misc_Console_SetBold
(void)
{
	fprintf(stdout, "\x1b[1m");
}

AX_API AX_INLINE
void
AX_Misc_Console_SetFaint
(void)
{
	fprintf(stdout, "\x1b[2m");
}

AX_API AX_INLINE
void
AX_Misc_Console_SetItalic
(void)
{
	fprintf(stdout, "\x1b[2m");
}

AX_API AX_INLINE
void
AX_Misc_Console_SetUnderline
(void)
{
	fprintf(stdout, "\x1b[4m");
}

AX_API AX_INLINE
void
AX_Misc_Console_SetDoubleUnderline
(void)
{
	fprintf(stdout, "\x1b[21m");
}

AX_API AX_INLINE
void
AX_Misc_Console_SetOverline
(void)
{
	fprintf(stdout, "\x1b[53m");
}

AX_API AX_INLINE
void
AX_Misc_Console_SetReverse
(void)
{
	fprintf(stdout, "\x1b[7m");
}

AX_API AX_INLINE
void
AX_Misc_Console_SetStrike
(void)
{
	fprintf(stdout, "\x1b[9m");
}

AX_API AX_INLINE
void
AX_Misc_Console_Conceal
(void)
{
	fprintf(stdout, "\x1b[8m");
}

AX_API AX_INLINE
void
AX_Misc_Console_Reveal
(void)
{
	fprintf(stdout, "\x1b[28m");
}


