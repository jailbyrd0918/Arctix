#pragma once

#include "Arctix/Defines/Global.h"
#include "Arctix/Defines/Core/Window.h"


AX_API AX_INLINE
SWindowConfig
AX_Window_GetProperties
(VoidPtr window);

AX_API AX_INLINE
ReadOnlyString
AX_Window_GetTitle
(VoidPtr window);

AX_API AX_INLINE
Int32
AX_Window_GetPosX
(VoidPtr window);

AX_API AX_INLINE
Int32
AX_Window_GetPosY
(VoidPtr window);

AX_API AX_INLINE
Int32
AX_Window_GetWidth
(VoidPtr window);

AX_API AX_INLINE
Int32
AX_Window_GetHeight
(VoidPtr window);

AX_API
Bool
Ax_Window_SetProperties
(VoidPtr window, SWindowConfig properties);

AX_API
Bool
AX_Window_SetTitle
(VoidPtr window, ReadOnlyString newTitle);

AX_API
Bool
AX_Window_SetPosX
(VoidPtr window, Int32 newX);

AX_API
Bool
AX_Window_SetPosY
(VoidPtr window, Int32 newY);

AX_API
Bool
AX_Window_SetPosition
(VoidPtr window, Int32 newX, Int32 newY);

AX_API
Bool
AX_Window_SetWidth
(VoidPtr window, Int32 newWidth);

AX_API
Bool
AX_Window_SetHeight
(VoidPtr window, Int32 newHeight);

AX_API
Bool
AX_Window_SetSize
(VoidPtr window, Int32 newWidth, Int32 newHeight);

struct SDL_Window;

AX_API AX_INLINE
VoidPtr
AX_Window_GetSDLWindow
(VoidPtr window);



