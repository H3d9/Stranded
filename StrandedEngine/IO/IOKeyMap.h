// Stranded Engine I/O
// 按键映射

#pragma once


// 鼠标映射
enum IOMouseMap {
	MOUSE_LB = 0,
	MOUSE_RB,
};

// 键盘映射
// 等价于direct_input的DIK定义
enum IOKeyBoardMap {
	KBD_ESCAPE = 0x01,
	KBD_1,
	KBD_2,
	KBD_3,
	KBD_4,
	KBD_5,
	KBD_6,
	KBD_7,
	KBD_8,
	KBD_9,
	KBD_0,
	KBD_MINUS,   /* - on main keyboard */
	KBD_EQUALS,
	KBD_BACKSPACE,
	KBD_TAB,
	KBD_Q,
	KBD_W,
	KBD_E,
	KBD_R,
	KBD_T,
	KBD_Y,
	KBD_U,
	KBD_I,
	KBD_O,
	KBD_P,
	KBD_LBRACKET,
	KBD_RBRACKET,
	KBD_RETURN,   /* Enter on main keyboard */
	KBD_LCONTROL,
	KBD_A,
	KBD_S,
	KBD_D,
	KBD_F,
	KBD_G,
	KBD_H,
	KBD_J,
	KBD_K,
	KBD_L,
	KBD_SEMICOLON,
	KBD_APOSTROPHE,
	KBD_GRAVE,   /* accent grave */
	KBD_LSHIFT,
	KBD_BACKSLASH,
	KBD_Z,
	KBD_X,
	KBD_C,
	KBD_V,
	KBD_B,
	KBD_N,
	KBD_M,
	KBD_COMMA,
	KBD_PERIOD,   /* . on main keyboard */
	KBD_SLASH,   /* / on main keyboard */
	KBD_RSHIFT,
	KBD_MULTIPLY,   /* * on numeric keypad */
	KBD_LALT,
	KBD_SPACE,
	KBD_CAPITAL,
	KBD_F1,
	KBD_F2,
	KBD_F3,
	KBD_F4,
	KBD_F5,
	KBD_F6,
	KBD_F7,
	KBD_F8,
	KBD_F9,
	KBD_F10,
	KBD_NUMLOCK,
	KBD_SCROLL,   /* Scroll Lock */
	KBD_NUMPAD7,
	KBD_NUMPAD8,
	KBD_NUMPAD9,
	KBD_SUBTRACT,   /* - on numeric keypad */
	KBD_NUMPAD4,
	KBD_NUMPAD5,
	KBD_NUMPAD6,
	KBD_ADD,   /* + on numeric keypad */
	KBD_NUMPAD1,
	KBD_NUMPAD2,
	KBD_NUMPAD3,
	KBD_NUMPAD0,
	KBD_DECIMAL,   /* . on numeric keypad */
	KBD_OEM_102,   /* <> or \| on RT 102-key keyboard (Non-U.S.) */
	KBD_F11,
	KBD_F12,
	KBD_F13,   /*                     (NEC PC98) */
	KBD_F14,   /*                     (NEC PC98) */
	KBD_F15,   /*                     (NEC PC98) */
	KBD_KANA,   /* (Japanese keyboard)            */
	KBD_ABNT_C1,   /* /? on Brazilian keyboard */
	KBD_CONVERT,   /* (Japanese keyboard)            */
	KBD_NOCONVERT,   /* (Japanese keyboard)            */
	KBD_YEN,   /* (Japanese keyboard)            */
	KBD_ABNT_C2,   /* Numpad . on Brazilian keyboard */
	KBD_NUMPADEQUALS,   /* = on numeric keypad (NEC PC98) */
	KBD_PREVTRACK,   /* Previous Track (KBD_CIRCUMFLEX on Japanese keyboard) */
	KBD_AT,   /*                     (NEC PC98) */
	KBD_COLON,   /*                     (NEC PC98) */
	KBD_UNDERLINE,   /*                     (NEC PC98) */
	KBD_KANJI,   /* (Japanese keyboard)            */
	KBD_STOP,   /*                     (NEC PC98) */
	KBD_AX,   /*                     (Japan AX) */
	KBD_UNLABELED,   /*                        (J3100) */
	KBD_NEXTTRACK,   /* Next Track */
	KBD_NUMPADENTER,   /* Enter on numeric keypad */
	KBD_RCONTROL,
	KBD_MUTE,   /* Mute */
	KBD_CALCULATOR,   /* Calculator */
	KBD_PLAYPAUSE,   /* Play / Pause */
	KBD_MEDIASTOP,   /* Media Stop */
	KBD_VOLUMEDOWN,   /* Volume - */
	KBD_VOLUMEUP,   /* Volume + */
	KBD_WEBHOME,   /* Web home */
	KBD_NUMPADCOMMA,   /* , on numeric keypad (NEC PC98) */
	KBD_DIVIDE,   /* / on numeric keypad */
	KBD_SYSRQ,
	KBD_RALT,
	KBD_PAUSE,   /* Pause */
	KBD_HOME,   /* Home on arrow keypad */
	KBD_UP,   /* UpArrow on arrow keypad */
	KBD_PRIOR,   /* PgUp on arrow keypad */
	KBD_LEFT,   /* LeftArrow on arrow keypad */
	KBD_RIGHT,   /* RightArrow on arrow keypad */
	KBD_END,   /* End on arrow keypad */
	KBD_DOWN,   /* DownArrow on arrow keypad */
	KBD_NEXT,   /* PgDn on arrow keypad */
	KBD_INSERT,   /* Insert on arrow keypad */
	KBD_DELETE,   /* Delete on arrow keypad */
	KBD_LWIN,   /* Left Windows key */
	KBD_RWIN,   /* Right Windows key */
	KBD_APPS,   /* AppMenu key */
	KBD_POWER,   /* System Power */
	KBD_SLEEP,   /* System Sleep */
	KBD_WAKE,   /* System Wake */
};
