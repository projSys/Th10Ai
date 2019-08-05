#include "GraphCap/Common.h"
#include "GraphCap/DesktopInput.h"

namespace gc
{
	const WORD DesktopInput::KEY_MAP[KEY_MAXCOUNT] =
	{
		0,

		VK_ESCAPE, VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12,

		'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',

		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
		'Z', 'X', 'C', 'V', 'B', 'N', 'M',

		VK_SHIFT, VK_SHIFT, VK_CONTROL, VK_CONTROL, VK_MENU, VK_MENU,

		VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT
	};

	const int DesktopInput::IS_KEY_PRESSED_MAP[KEY_MAXCOUNT] =
	{
		0,

		VK_ESCAPE, VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12,

		'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',

		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
		'Z', 'X', 'C', 'V', 'B', 'N', 'M',

		VK_LSHIFT, VK_RSHIFT, VK_LCONTROL, VK_RCONTROL, VK_LMENU, VK_RMENU,

		VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT
	};

	const DWORD DesktopInput::MOUSE_PRESS_MAP[MOUSE_MAXCOUNT] =
	{
		0,

		MOUSEEVENTF_LEFTDOWN,
		MOUSEEVENTF_RIGHTDOWN,
		MOUSEEVENTF_MIDDLEDOWN,
		MOUSEEVENTF_XDOWN
	};

	const DWORD DesktopInput::MOUSE_RELEASE_MAP[MOUSE_MAXCOUNT] =
	{
		0,

		MOUSEEVENTF_LEFTUP,
		MOUSEEVENTF_RIGHTUP,
		MOUSEEVENTF_MIDDLEUP,
		MOUSEEVENTF_XUP
	};

	const int DesktopInput::IS_MOUSE_PRESSED_MAP[MOUSE_MAXCOUNT] =
	{
		0,

		VK_LBUTTON,
		VK_RBUTTON,
		VK_MBUTTON,
		VK_XBUTTON1
	};

	void DesktopInput::keyPress(Key key)
	{
		if (!(key > KEY_NONE && key < KEY_MAXCOUNT))
			THROW_BASE_EXCEPTION(Exception() << err_str("Out of range."));

		INPUT input = {};
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = KEY_MAP[key];
		input.ki.wScan = MapVirtualKey(KEY_MAP[key], MAPVK_VK_TO_VSC);
		//input.ki.dwFlags = KEYEVENTF_SCANCODE;
		UINT ret = SendInput(1, &input, sizeof(input));
		if (ret != 1)
			THROW_WINDOWS_ERROR(GetLastError());
	}

	void DesktopInput::keyRelease(Key key)
	{
		if (!(key > KEY_NONE && key < KEY_MAXCOUNT))
			THROW_BASE_EXCEPTION(Exception() << err_str("Out of range."));

		INPUT input = {};
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = KEY_MAP[key];
		input.ki.wScan = MapVirtualKey(KEY_MAP[key], MAPVK_VK_TO_VSC);
		input.ki.dwFlags = /*KEYEVENTF_SCANCODE | */KEYEVENTF_KEYUP;
		UINT ret = SendInput(1, &input, sizeof(input));
		if (ret != 1)
			THROW_WINDOWS_ERROR(GetLastError());
	}

	bool DesktopInput::isKeyPressed(Key key) const
	{
		if (!(key > KEY_NONE && key < KEY_MAXCOUNT))
			THROW_BASE_EXCEPTION(Exception() << err_str("Out of range."));

		return (GetAsyncKeyState(IS_KEY_PRESSED_MAP[key]) & 0x8000) != 0;
	}

	void DesktopInput::mousePress(Mouse mouse)
	{
		if (!(mouse > MOUSE_NONE && mouse < MOUSE_MAXCOUNT))
			THROW_BASE_EXCEPTION(Exception() << err_str("Out of range."));

		INPUT input = {};
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSE_PRESS_MAP[mouse];
		UINT ret = SendInput(1, &input, sizeof(input));
		if (ret != 1)
			THROW_WINDOWS_ERROR(GetLastError());
	}

	void DesktopInput::mouseRelease(Mouse mouse)
	{
		if (!(mouse > MOUSE_NONE && mouse < MOUSE_MAXCOUNT))
			THROW_BASE_EXCEPTION(Exception() << err_str("Out of range."));

		INPUT input = {};
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSE_RELEASE_MAP[mouse];
		UINT ret = SendInput(1, &input, sizeof(input));
		if (ret != 1)
			THROW_WINDOWS_ERROR(GetLastError());
	}

	bool DesktopInput::isMousePressed(Mouse mouse) const
	{
		if (!(mouse > MOUSE_NONE && mouse < MOUSE_MAXCOUNT))
			THROW_BASE_EXCEPTION(Exception() << err_str("Out of range."));

		return (GetAsyncKeyState(IS_MOUSE_PRESSED_MAP[mouse]) & 0x8000) != 0;
	}

	Point DesktopInput::getMousePos() const
	{
		POINT pos = {};
		if (!GetCursorPos(&pos))
			THROW_WINDOWS_ERROR(GetLastError());
		return Point(pos.x, pos.y);
	}

	void DesktopInput::setMousePos(const Point& pos)
	{
		if (!SetCursorPos(static_cast<int>(pos.x), static_cast<int>(pos.y)))
			THROW_WINDOWS_ERROR(GetLastError());
	}
}
