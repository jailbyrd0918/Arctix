#pragma once

#include "Arctix/Defines/Core/DataTypes.h"


typedef
enum AX_Input_Key_Code
{
	AX_KEYCODE_UNKNOWN = 0,

	AX_KEYCODE_A = 4,
	AX_KEYCODE_B = 5,
	AX_KEYCODE_C = 6,
	AX_KEYCODE_D = 7,
	AX_KEYCODE_E = 8,
	AX_KEYCODE_F = 9,
	AX_KEYCODE_G = 10,
	AX_KEYCODE_H = 11,
	AX_KEYCODE_I = 12,
	AX_KEYCODE_J = 13,
	AX_KEYCODE_K = 14,
	AX_KEYCODE_L = 15,
	AX_KEYCODE_M = 16,
	AX_KEYCODE_N = 17,
	AX_KEYCODE_O = 18,
	AX_KEYCODE_P = 19,
	AX_KEYCODE_Q = 20,
	AX_KEYCODE_R = 21,
	AX_KEYCODE_S = 22,
	AX_KEYCODE_T = 23,
	AX_KEYCODE_U = 24,
	AX_KEYCODE_V = 25,
	AX_KEYCODE_W = 26,
	AX_KEYCODE_X = 27,
	AX_KEYCODE_Y = 28,
	AX_KEYCODE_Z = 29,

	AX_KEYCODE_1 = 30,
	AX_KEYCODE_2 = 31,
	AX_KEYCODE_3 = 32,
	AX_KEYCODE_4 = 33,
	AX_KEYCODE_5 = 34,
	AX_KEYCODE_6 = 35,
	AX_KEYCODE_7 = 36,
	AX_KEYCODE_8 = 37,
	AX_KEYCODE_9 = 38,
	AX_KEYCODE_0 = 39,

	AX_KEYCODE_RETURN = 40,
	AX_KEYCODE_ESCAPE = 41,
	AX_KEYCODE_BACKSPACE = 42,
	AX_KEYCODE_TAB = 43,
	AX_KEYCODE_SPACE = 44,

	AX_KEYCODE_MINUS = 45,
	AX_KEYCODE_EQUALS = 46,
	AX_KEYCODE_LEFTBRACKET = 47,
	AX_KEYCODE_RIGHTBRACKET = 48,
	AX_KEYCODE_BACKSLASH = 49,
	AX_KEYCODE_NONUSHASH = 50,
	AX_KEYCODE_SEMICOLON = 51,
	AX_KEYCODE_APOSTROPHE = 52,
	AX_KEYCODE_GRAVE = 53,
	AX_KEYCODE_COMMA = 54,
	AX_KEYCODE_PERIOD = 55,
	AX_KEYCODE_SLASH = 56,

	AX_KEYCODE_CAPSLOCK = 57,

	AX_KEYCODE_F1 = 58,
	AX_KEYCODE_F2 = 59,
	AX_KEYCODE_F3 = 60,
	AX_KEYCODE_F4 = 61,
	AX_KEYCODE_F5 = 62,
	AX_KEYCODE_F6 = 63,
	AX_KEYCODE_F7 = 64,
	AX_KEYCODE_F8 = 65,
	AX_KEYCODE_F9 = 66,
	AX_KEYCODE_F10 = 67,
	AX_KEYCODE_F11 = 68,
	AX_KEYCODE_F12 = 69,

	AX_KEYCODE_PRINTSCREEN = 70,
	AX_KEYCODE_SCROLLLOCK = 71,
	AX_KEYCODE_PAUSE = 72,
	AX_KEYCODE_INSERT = 73,
	AX_KEYCODE_HOME = 74,
	AX_KEYCODE_PAGEUP = 75,
	AX_KEYCODE_DELETE = 76,
	AX_KEYCODE_END = 77,
	AX_KEYCODE_PAGEDOWN = 78,
	AX_KEYCODE_RIGHT = 79,
	AX_KEYCODE_LEFT = 80,
	AX_KEYCODE_DOWN = 81,
	AX_KEYCODE_UP = 82,

	AX_KEYCODE_NUMLOCKCLEAR = 83,
	AX_KEYCODE_KP_DIVIDE = 84,
	AX_KEYCODE_KP_MULTIPLY = 85,
	AX_KEYCODE_KP_MINUS = 86,
	AX_KEYCODE_KP_PLUS = 87,
	AX_KEYCODE_KP_ENTER = 88,
	AX_KEYCODE_KP_1 = 89,
	AX_KEYCODE_KP_2 = 90,
	AX_KEYCODE_KP_3 = 91,
	AX_KEYCODE_KP_4 = 92,
	AX_KEYCODE_KP_5 = 93,
	AX_KEYCODE_KP_6 = 94,
	AX_KEYCODE_KP_7 = 95,
	AX_KEYCODE_KP_8 = 96,
	AX_KEYCODE_KP_9 = 97,
	AX_KEYCODE_KP_0 = 98,
	AX_KEYCODE_KP_PERIOD = 99,

	AX_KEYCODE_NONUSBACKSLASH = 100,
	AX_KEYCODE_APPLICATION = 101,
	AX_KEYCODE_POWER = 102,
	AX_KEYCODE_KP_EQUALS = 103,
	AX_KEYCODE_F13 = 104,
	AX_KEYCODE_F14 = 105,
	AX_KEYCODE_F15 = 106,
	AX_KEYCODE_F16 = 107,
	AX_KEYCODE_F17 = 108,
	AX_KEYCODE_F18 = 109,
	AX_KEYCODE_F19 = 110,
	AX_KEYCODE_F20 = 111,
	AX_KEYCODE_F21 = 112,
	AX_KEYCODE_F22 = 113,
	AX_KEYCODE_F23 = 114,
	AX_KEYCODE_F24 = 115,
	AX_KEYCODE_EXECUTE = 116,
	AX_KEYCODE_HELP = 117,
	AX_KEYCODE_MENU = 118,
	AX_KEYCODE_SELECT = 119,
	AX_KEYCODE_STOP = 120,
	AX_KEYCODE_AGAIN = 121,
	AX_KEYCODE_UNDO = 122,
	AX_KEYCODE_CUT = 123,
	AX_KEYCODE_COPY = 124,
	AX_KEYCODE_PASTE = 125,
	AX_KEYCODE_FIND = 126,
	AX_KEYCODE_MUTE = 127,
	AX_KEYCODE_VOLUMEUP = 128,
	AX_KEYCODE_VOLUMEDOWN = 129,

	AX_KEYCODE_KP_COMMA = 133,
	AX_KEYCODE_KP_EQUALSAS400 = 134,

	AX_KEYCODE_INTERNATIONAL1 = 135,
	AX_KEYCODE_INTERNATIONAL2 = 136,
	AX_KEYCODE_INTERNATIONAL3 = 137,
	AX_KEYCODE_INTERNATIONAL4 = 138,
	AX_KEYCODE_INTERNATIONAL5 = 139,
	AX_KEYCODE_INTERNATIONAL6 = 140,
	AX_KEYCODE_INTERNATIONAL7 = 141,
	AX_KEYCODE_INTERNATIONAL8 = 142,
	AX_KEYCODE_INTERNATIONAL9 = 143,
	AX_KEYCODE_LANG1 = 144,
	AX_KEYCODE_LANG2 = 145,
	AX_KEYCODE_LANG3 = 146,
	AX_KEYCODE_LANG4 = 147,
	AX_KEYCODE_LANG5 = 148,
	AX_KEYCODE_LANG6 = 149,
	AX_KEYCODE_LANG7 = 150,
	AX_KEYCODE_LANG8 = 151,
	AX_KEYCODE_LANG9 = 152,

	AX_KEYCODE_ALTERASE = 153,
	AX_KEYCODE_SYSREQ = 154,
	AX_KEYCODE_CANCEL = 155,
	AX_KEYCODE_CLEAR = 156,
	AX_KEYCODE_PRIOR = 157,
	AX_KEYCODE_RETURN2 = 158,
	AX_KEYCODE_SEPARATOR = 159,
	AX_KEYCODE_OUT = 160,
	AX_KEYCODE_OPER = 161,
	AX_KEYCODE_CLEARAGAIN = 162,
	AX_KEYCODE_CRSEL = 163,
	AX_KEYCODE_EXSEL = 164,

	AX_KEYCODE_KP_00 = 176,
	AX_KEYCODE_KP_000 = 177,
	AX_KEYCODE_THOUSANDSSEPARATOR = 178,
	AX_KEYCODE_DECIMALSEPARATOR = 179,
	AX_KEYCODE_CURRENCYUNIT = 180,
	AX_KEYCODE_CURRENCYSUBUNIT = 181,
	AX_KEYCODE_KP_LEFTPAREN = 182,
	AX_KEYCODE_KP_RIGHTPAREN = 183,
	AX_KEYCODE_KP_LEFTBRACE = 184,
	AX_KEYCODE_KP_RIGHTBRACE = 185,
	AX_KEYCODE_KP_TAB = 186,
	AX_KEYCODE_KP_BACKSPACE = 187,
	AX_KEYCODE_KP_A = 188,
	AX_KEYCODE_KP_B = 189,
	AX_KEYCODE_KP_C = 190,
	AX_KEYCODE_KP_D = 191,
	AX_KEYCODE_KP_E = 192,
	AX_KEYCODE_KP_F = 193,
	AX_KEYCODE_KP_XOR = 194,
	AX_KEYCODE_KP_POWER = 195,
	AX_KEYCODE_KP_PERCENT = 196,
	AX_KEYCODE_KP_LESS = 197,
	AX_KEYCODE_KP_GREATER = 198,
	AX_KEYCODE_KP_AMPERSAND = 199,
	AX_KEYCODE_KP_DBLAMPERSAND = 200,
	AX_KEYCODE_KP_VERTICALBAR = 201,
	AX_KEYCODE_KP_DBLVERTICALBAR = 202,
	AX_KEYCODE_KP_COLON = 203,
	AX_KEYCODE_KP_HASH = 204,
	AX_KEYCODE_KP_SPACE = 205,
	AX_KEYCODE_KP_AT = 206,
	AX_KEYCODE_KP_EXCLAM = 207,
	AX_KEYCODE_KP_MEMSTORE = 208,
	AX_KEYCODE_KP_MEMRECALL = 209,
	AX_KEYCODE_KP_MEMCLEAR = 210,
	AX_KEYCODE_KP_MEMADD = 211,
	AX_KEYCODE_KP_MEMSUBTRACT = 212,
	AX_KEYCODE_KP_MEMMULTIPLY = 213,
	AX_KEYCODE_KP_MEMDIVIDE = 214,
	AX_KEYCODE_KP_PLUSMINUS = 215,
	AX_KEYCODE_KP_CLEAR = 216,
	AX_KEYCODE_KP_CLEARENTRY = 217,
	AX_KEYCODE_KP_BINARY = 218,
	AX_KEYCODE_KP_OCTAL = 219,
	AX_KEYCODE_KP_DECIMAL = 220,
	AX_KEYCODE_KP_HEXADECIMAL = 221,

	AX_KEYCODE_LCTRL = 224,
	AX_KEYCODE_LSHIFT = 225,
	AX_KEYCODE_LALT = 226,
	AX_KEYCODE_LGUI = 227,
	AX_KEYCODE_RCTRL = 228,
	AX_KEYCODE_RSHIFT = 229,
	AX_KEYCODE_RALT = 230,
	AX_KEYCODE_RGUI = 231,

	AX_KEYCODE_MODE = 257,

	AX_KEYCODE_AUDIONEXT = 258,
	AX_KEYCODE_AUDIOPREV = 259,
	AX_KEYCODE_AUDIOSTOP = 260,
	AX_KEYCODE_AUDIOPLAY = 261,
	AX_KEYCODE_AUDIOMUTE = 262,
	AX_KEYCODE_MEDIASELECT = 263,
	AX_KEYCODE_WWW = 264,
	AX_KEYCODE_MAIL = 265,
	AX_KEYCODE_CALCULATOR = 266,
	AX_KEYCODE_COMPUTER = 267,
	AX_KEYCODE_AC_SEARCH = 268,
	AX_KEYCODE_AC_HOME = 269,
	AX_KEYCODE_AC_BACK = 270,
	AX_KEYCODE_AC_FORWARD = 271,
	AX_KEYCODE_AC_STOP = 272,
	AX_KEYCODE_AC_REFRESH = 273,
	AX_KEYCODE_AC_BOOKMARKS = 274,

	AX_KEYCODE_BRIGHTNESSDOWN = 275,
	AX_KEYCODE_BRIGHTNESSUP = 276,
	AX_KEYCODE_DISPLAYSWITCH = 277,
	AX_KEYCODE_KBDILLUMTOGGLE = 278,
	AX_KEYCODE_KBDILLUMDOWN = 279,
	AX_KEYCODE_KBDILLUMUP = 280,
	AX_KEYCODE_EJECT = 281,
	AX_KEYCODE_SLEEP = 282,

	AX_KEYCODE_APP1 = 283,
	AX_KEYCODE_APP2 = 284,

	AX_KEYCODE_AUDIOREWIND = 285,
	AX_KEYCODE_AUDIOFASTFORWARD = 286,

	AX_KEYCODES_TOTAL = 512

}
EKeyCode;

#define	AX_Input_Get_KeyCode(keycode)	\
	SDL_GetKeyFromScancode(keycode)

typedef
enum AX_Input_Mouse_Code
{
	AX_MOUSECODE_INVALID = 0x00,
	AX_MOUSECODE_LEFT = 0x01,
	AX_MOUSECODE_MIDDLE = 0x02,
	AX_MOUSECODE_RIGHT = 0x03,
	AX_MOUSECODES_TOTAL
}
EMouseCode;

typedef
enum AX_Input_Event_Code
{
	AX_EVENTCODE_APP_QUIT = 0x01,
	AX_EVENTCODE_KEY_PRESSED = 0x02,
	AX_EVENTCODE_KEY_RELEASED = 0x03,
	AX_EVENTCODE_MOUSE_BTN_PRESSED = 0x04,
	AX_EVENTCODE_MOUSE_BTN_RELEASED = 0x05,
	AX_EVENTCODE_MOUSE_MOVED = 0x06,
	AX_EVENTCODE_MOUSE_SCROLLED = 0x07,
	AX_EVENTCODE_RESIZED = 0x08,
	AX_EVENTCODES_TOTAL
}
EEventCode;

typedef
struct AX_Input_Event_Data
{
	EEventCode	eventCode;
}
SInputEventData;

typedef
struct AX_Input_Key_Data
{
	EEventCode	eventCode;
	UInt16		key;
}
SInputKeyData;

typedef
struct AX_Input_Mouse_Data
{
	EEventCode	eventCode;
	UInt16		mouseCode;
}
SInputMouseData;

typedef
struct AX_Input_Mouse_Move_Data
{
	EEventCode	eventCode;
	UInt16		x, y;
}
SInputMouseMoveData;

typedef
struct AX_Input_Mouse_Scroll_Data
{
	EEventCode	eventCode;
	Int8		value;
}
SInputMouseScrollData;

typedef
struct AX_Window_Resize_Data
{
	EEventCode	eventCode;
	UInt16		width, height;
}
SWinResizeData;


