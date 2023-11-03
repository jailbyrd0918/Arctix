#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/DataTypes.h"
#include "Arctix/Defines/Core/Containers.h"


AX_API
void
AX_Misc_Console_Startup
(void);

AX_API
void
AX_Misc_Console_Shutdown
(void);

AX_API AX_INLINE
void
AX_Misc_Console_Reset
(void);


// -- color -- //

AX_API AX_INLINE
void
AX_Misc_Console_SetForegroundColor
(Int32 code);

AX_API AX_INLINE
void
AX_Misc_Console_SetBackgroundColor
(Int32 code);

AX_API AX_INLINE
void
AX_Misc_Console_SetForegroundColorRGB
(Int32 r, Int32 g, Int32 b);

AX_API AX_INLINE
void
AX_Misc_Console_SetBackgroundColorRGB
(Int32 r, Int32 g, Int32 b);


// -- clear -- //

AX_API
void
AX_Misc_Console_ClearScreen
(void);

AX_API
void
AX_Misc_Console_ClearScreenToTop
(void);

AX_API
void
AX_Misc_Console_ClearScreenToBottom
(void);

AX_API
void
AX_Misc_Console_ClearLine
(void);

AX_API
void
AX_Misc_Console_ClearLineToLeft
(void);

AX_API
void
AX_Misc_Console_ClearLineToRight
(void);


// -- cursor -- //

AX_API
void
AX_Misc_Console_MoveCursorUp
(Int32 positions);

AX_API
void
AX_Misc_Console_MoveCursorDown
(Int32 positions);

AX_API
void
AX_Misc_Console_MoveCursorLeft
(Int32 positions);

AX_API
void
AX_Misc_Console_MoveCursorRight
(Int32 positions);

AX_API
void
AX_Misc_Console_MoveCursorTo
(Int32 row, Int32 column);


// -- input -- //

AX_API
SString
AX_Misc_Console_ReadLine
(void);


// -- output -- //

AX_API
void
AX_Misc_Console_Write
(ReadOnlyString format, ...);

AX_API
void
AX_Misc_Console_WriteLine
(ReadOnlyString format, ...);


// -- miscellaneous -- //

AX_API AX_INLINE
void
AX_Misc_Console_SetUnderline
(void);

AX_API AX_INLINE
void
AX_Misc_Console_SetDoubleUnderline
(void);

AX_API AX_INLINE
void
AX_Misc_Console_SetOverline
(void);

AX_API AX_INLINE
void
AX_Misc_Console_SetBold
(void);

AX_API AX_INLINE
void
AX_Misc_Console_SetItalic
(void);

AX_API AX_INLINE
void
AX_Misc_Console_SetReverse
(void);

AX_API AX_INLINE
void
AX_Misc_Console_SetStrike
(void);

AX_API AX_INLINE
void
AX_Misc_Console_Conceal
(void);

AX_API AX_INLINE
void
AX_Misc_Console_Reveal
(void);


