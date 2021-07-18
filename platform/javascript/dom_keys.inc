/*************************************************************************/
/*  dom_keys.inc                                                         */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Fox ENGINE                                */
/*                      https://Foxengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Fox Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "core/os/keyboard.h"

// See https://w3c.github.io/uievents-code/#code-value-tables
int dom_code2Fox_scancode(EM_UTF8 const p_code[32], EM_UTF8 const p_key[32], bool p_physical) {
#define DOM2Fox(p_str, p_Fox_code)                                         \
	if (memcmp((const void *)p_str, (void *)p_code, strlen(p_str) + 1) == 0) { \
		return KEY_##p_Fox_code;                                             \
	}

	// Numpad section.
	DOM2Fox("NumLock", NUMLOCK);
	DOM2Fox("Numpad0", KP_0);
	DOM2Fox("Numpad1", KP_1);
	DOM2Fox("Numpad2", KP_2);
	DOM2Fox("Numpad3", KP_3);
	DOM2Fox("Numpad4", KP_4);
	DOM2Fox("Numpad5", KP_5);
	DOM2Fox("Numpad6", KP_6);
	DOM2Fox("Numpad7", KP_7);
	DOM2Fox("Numpad8", KP_8);
	DOM2Fox("Numpad9", KP_9);
	DOM2Fox("NumpadAdd", KP_ADD);
	DOM2Fox("NumpadBackspace", BACKSPACE);
	DOM2Fox("NumpadClear", CLEAR);
	DOM2Fox("NumpadClearEntry", CLEAR);
	//DOM2Fox("NumpadComma", UNKNOWN);
	DOM2Fox("NumpadDecimal", KP_PERIOD);
	DOM2Fox("NumpadDivide", KP_DIVIDE);
	DOM2Fox("NumpadEnter", KP_ENTER);
	DOM2Fox("NumpadEqual", EQUAL);
	//DOM2Fox("NumpadHash", UNKNOWN);
	//DOM2Fox("NumpadMemoryAdd", UNKNOWN);
	//DOM2Fox("NumpadMemoryClear", UNKNOWN);
	//DOM2Fox("NumpadMemoryRecall", UNKNOWN);
	//DOM2Fox("NumpadMemoryStore", UNKNOWN);
	//DOM2Fox("NumpadMemorySubtract", UNKNOWN);
	DOM2Fox("NumpadMultiply", KP_MULTIPLY);
	DOM2Fox("NumpadParenLeft", PARENLEFT);
	DOM2Fox("NumpadParenRight", PARENRIGHT);
	DOM2Fox("NumpadStar", KP_MULTIPLY); // or ASTERISK ?
	DOM2Fox("NumpadSubtract", KP_SUBTRACT);

	// Printable ASCII.
	if (!p_physical) {
		uint8_t b0 = (uint8_t)p_key[0];
		uint8_t b1 = (uint8_t)p_key[1];
		uint8_t b2 = (uint8_t)p_key[2];
		if (b1 == 0 && b0 > 0x1F && b0 < 0x7F) { // ASCII.
			if (b0 > 0x60 && b0 < 0x7B) { // Lowercase ASCII.
				b0 -= 32;
			}
			return b0;
		}

#define _U_2BYTES_MASK 0xE0
#define _U_2BYTES 0xC0
		// Latin-1 codes.
		if (b2 == 0 && (b0 & _U_2BYTES_MASK) == _U_2BYTES) { // 2-bytes utf8, only known latin.
			uint32_t key = ((b0 & ~_U_2BYTES_MASK) << 6) | (b1 & 0x3F);
			if (key >= 0xA0 && key <= 0xDF) {
				return key;
			}
			if (key >= 0xE0 && key <= 0xFF) { // Lowercase known latin.
				key -= 0x20;
				return key;
			}
		}
#undef _U_2BYTES_MASK
#undef _U_2BYTES
	}

	// Alphanumeric section.
	DOM2Fox("Backquote", QUOTELEFT);
	DOM2Fox("Backslash", BACKSLASH);
	DOM2Fox("BracketLeft", BRACKETLEFT);
	DOM2Fox("BracketRight", BRACKETRIGHT);
	DOM2Fox("Comma", COMMA);
	DOM2Fox("Digit0", 0);
	DOM2Fox("Digit1", 1);
	DOM2Fox("Digit2", 2);
	DOM2Fox("Digit3", 3);
	DOM2Fox("Digit4", 4);
	DOM2Fox("Digit5", 5);
	DOM2Fox("Digit6", 6);
	DOM2Fox("Digit7", 7);
	DOM2Fox("Digit8", 8);
	DOM2Fox("Digit9", 9);
	DOM2Fox("Equal", EQUAL);
	DOM2Fox("IntlBackslash", BACKSLASH);
	//DOM2Fox("IntlRo", UNKNOWN);
	DOM2Fox("IntlYen", YEN);

	DOM2Fox("KeyA", A);
	DOM2Fox("KeyB", B);
	DOM2Fox("KeyC", C);
	DOM2Fox("KeyD", D);
	DOM2Fox("KeyE", E);
	DOM2Fox("KeyF", F);
	DOM2Fox("KeyG", G);
	DOM2Fox("KeyH", H);
	DOM2Fox("KeyI", I);
	DOM2Fox("KeyJ", J);
	DOM2Fox("KeyK", K);
	DOM2Fox("KeyL", L);
	DOM2Fox("KeyM", M);
	DOM2Fox("KeyN", N);
	DOM2Fox("KeyO", O);
	DOM2Fox("KeyP", P);
	DOM2Fox("KeyQ", Q);
	DOM2Fox("KeyR", R);
	DOM2Fox("KeyS", S);
	DOM2Fox("KeyT", T);
	DOM2Fox("KeyU", U);
	DOM2Fox("KeyV", V);
	DOM2Fox("KeyW", W);
	DOM2Fox("KeyX", X);
	DOM2Fox("KeyY", Y);
	DOM2Fox("KeyZ", Z);

	DOM2Fox("Minus", MINUS);
	DOM2Fox("Period", PERIOD);
	DOM2Fox("Quote", APOSTROPHE);
	DOM2Fox("Semicolon", SEMICOLON);
	DOM2Fox("Slash", SLASH);

	// Functional keys in the Alphanumeric section.
	DOM2Fox("AltLeft", ALT);
	DOM2Fox("AltRight", ALT);
	DOM2Fox("Backspace", BACKSPACE);
	DOM2Fox("CapsLock", CAPSLOCK);
	DOM2Fox("ContextMenu", MENU);
	DOM2Fox("ControlLeft", CTRL);
	DOM2Fox("ControlRight", CTRL);
	DOM2Fox("Enter", ENTER);
	DOM2Fox("MetaLeft", SUPER_L);
	DOM2Fox("MetaRight", SUPER_R);
	DOM2Fox("ShiftLeft", SHIFT);
	DOM2Fox("ShiftRight", SHIFT);
	DOM2Fox("Space", SPACE);
	DOM2Fox("Tab", TAB);

	// ControlPad section.
	DOM2Fox("Delete", DELETE);
	DOM2Fox("End", END);
	DOM2Fox("Help", HELP);
	DOM2Fox("Home", HOME);
	DOM2Fox("Insert", INSERT);
	DOM2Fox("PageDown", PAGEDOWN);
	DOM2Fox("PageUp", PAGEUP);

	// ArrowPad section.
	DOM2Fox("ArrowDown", DOWN);
	DOM2Fox("ArrowLeft", LEFT);
	DOM2Fox("ArrowRight", RIGHT);
	DOM2Fox("ArrowUp", UP);

	// Function section.
	DOM2Fox("Escape", ESCAPE);
	DOM2Fox("F1", F1);
	DOM2Fox("F2", F2);
	DOM2Fox("F3", F3);
	DOM2Fox("F4", F4);
	DOM2Fox("F5", F5);
	DOM2Fox("F6", F6);
	DOM2Fox("F7", F7);
	DOM2Fox("F8", F8);
	DOM2Fox("F9", F9);
	DOM2Fox("F10", F10);
	DOM2Fox("F11", F11);
	DOM2Fox("F12", F12);
	//DOM2Fox("Fn", UNKNOWN); // never actually fired, but included in the standard draft.
	//DOM2Fox("FnLock", UNKNOWN);
	DOM2Fox("PrintScreen", PRINT);
	DOM2Fox("ScrollLock", SCROLLLOCK);
	DOM2Fox("Pause", PAUSE);

	// Media keys section.
	DOM2Fox("BrowserBack", BACK);
	DOM2Fox("BrowserFavorites", FAVORITES);
	DOM2Fox("BrowserForward", FORWARD);
	DOM2Fox("BrowserHome", OPENURL);
	DOM2Fox("BrowserRefresh", REFRESH);
	DOM2Fox("BrowserSearch", SEARCH);
	DOM2Fox("BrowserStop", STOP);
	//DOM2Fox("Eject", UNKNOWN);
	DOM2Fox("LaunchApp1", LAUNCH0);
	DOM2Fox("LaunchApp2", LAUNCH1);
	DOM2Fox("LaunchMail", LAUNCHMAIL);
	DOM2Fox("MediaPlayPause", MEDIAPLAY);
	DOM2Fox("MediaSelect", LAUNCHMEDIA);
	DOM2Fox("MediaStop", MEDIASTOP);
	DOM2Fox("MediaTrackNext", MEDIANEXT);
	DOM2Fox("MediaTrackPrevious", MEDIAPREVIOUS);
	//DOM2Fox("Power", UNKNOWN);
	//DOM2Fox("Sleep", UNKNOWN);
	DOM2Fox("AudioVolumeDown", VOLUMEDOWN);
	DOM2Fox("AudioVolumeMute", VOLUMEMUTE);
	DOM2Fox("AudioVolumeUp", VOLUMEUP);
	//DOM2Fox("WakeUp", UNKNOWN);
	return KEY_UNKNOWN;
#undef DOM2Fox
}
