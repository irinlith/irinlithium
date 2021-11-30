#include "irinlithiumVKB.h"

#ifdef __unix__
	#ifndef __wayland__
		//X11 implementation

		XKeyEvent createKeyEvent(Display *display, Window *win, Window *winRoot, bool *press, char *keycode, int *modifiers) {
			XKeyEvent event;
			event.display     = display;
			event.window      = *win;
			event.root        = *winRoot;
			event.subwindow   = None;
			event.time        = CurrentTime;
			event.x           = 1;
			event.y           = 1;
			event.x_root      = 1;
			event.y_root      = 1;
			event.same_screen = 0;
			event.keycode     = XKeysymToKeycode(display, *keycode);
			event.state       = *modifiers;
			event.type = *press ? KeyPress : KeyRelease;
			return event;
		}

		XButtonEvent createButtonEvent(Display *display, Window *win, Window *winRoot, bool *press, char *button, int *modifiers) {
			XButtonEvent event;
			event.display = display;
			event.window = *win;
			event.root = *winRoot;
			event.subwindow = None;
			event.time = CurrentTime;
			event.x = 1;
			event.y = 1;
			event.x_root = 1;
			event.y_root = 1;
			event.same_screen = 0;
			event.button = *button;
			event.state = *modifiers;
			event.type = *press ? ButtonPress : ButtonRelease;
			return event;
		}

		void VKB_Event(char *code, int *mask, bool press, bool *isButton) {
			Display *display = XOpenDisplay(0);
			if (!display) return;
			Window winRoot = XDefaultRootWindow(display), winFocus;
			int revert;
			XGetInputFocus(display, &winFocus, &revert);
			if (!*isButton) {
				XKeyEvent keyevent = createKeyEvent(display, &winFocus, &winRoot, &press, code, mask);
				XSendEvent(keyevent.display, keyevent.window, 1, keyevent.keycode, (XEvent*)&keyevent);
			}
			else {
				XButtonEvent buttonevent = createButtonEvent(display, &winFocus, &winRoot, &press, code, mask);
				XSendEvent(buttonevent.display, buttonevent.window, 1, buttonevent.button, (XEvent*)&buttonevent);
			}
			XSync(display, 1);
			XCloseDisplay(display);
		}

		void VKB_HoldRelease(useconds_t delay, char code, int mask, bool isButton) {
			VKB_Event(&code, &mask, 1, &isButton);
			usleep(delay);
			VKB_Event(&code, &mask, 0, &isButton);
		}

		void VKB_TypeString(const char *str, int mask, useconds_t delay, char *shiftmask) {
			while (*str) {
				VKB_HoldRelease(20, *str, mask | ((*str >= 65 && *str <= 90) || (strcontain(shiftmask, *str) ? ShiftMask : 0)), 0);
				usleep(delay);
				++str;
			}
		}

		void VKB_MoveCursor(int x, int y) {
			Display *display = XOpenDisplay(0);
			if (!display) return;
			XWarpPointer(display, 0, 0, 0, 0, 0, 0, x, y);
		}
	#else
		//Wayland implementation (?)
	#endif


#elif _WIN32
	void VKB_Event(BYTE *keycode, BYTE *prekey, BYTE *prekey2, bool press) {
		/*KEYBDINPUT kbi;
		kbi.wVk = keycode;
		kbi.wScan = 0;
		kbi.dwFlags = dwFlags;
		kbi.time = 0;
		kbi.dwExtraInfo = (ULONG_PTR) GetMessageExtraInfo();
		INPUT input;
		input.type = INPUT_KEYBOARD;
		input.ki = kbi;
		SendInput(1, &input, sizeof(INPUT));*/

		//keybd_event(keycode, 0, dwFlags, 0);

		INPUT inputs[3];
		ZeroMemory(inputs, sizeof(inputs));

		for (size_t i = 0; i < (1 + (*prekey > 0 ? (*prekey2 > 0 ? 2 : 1) : 0)); i++) {
			inputs[i].type = INPUT_KEYBOARD;
			//inputs[i].ki.wScan = *keycode;
			inputs[i].ki.wVk = (*prekey > 0 && i == 0 ? *prekey : (*prekey2 > 0 && *prekey > 0 && i == 1 ? *prekey2 : *keycode));
			inputs[i].ki.dwFlags = /*KEYEVENTF_SCANCODE | */(!press ? KEYEVENTF_KEYUP : KEYEVENTF_EXTENDEDKEY);
		}

		SendInput(3, inputs, sizeof(INPUT));
	}

	void VKB_HoldRelease(DWORD delay, BYTE keycode, BYTE prekey, BYTE prekey2) {
		VKB_Event(&keycode, &prekey, &prekey2, 1);
		Sleep(delay * 0.001);
		VKB_Event(&keycode, &prekey, &prekey2, 0);
	}

	void VKB_TypeString(const char *str, DWORD delay, char *shiftmask) {
		while (*str) {
			VKB_HoldRelease(20, (*str >= 97 && *str <= 122) ? *str - 32 : *str, strcontain(shiftmask, *str) ? VK_LSHIFT : 0, 0);
			Sleep(delay * 0.001);
			++str;
		}
	}
#endif
