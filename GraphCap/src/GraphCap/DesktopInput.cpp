#include "GraphCap/Common.h"
#include "GraphCap/DesktopInput.h"

namespace gc
{
	const WORD DesktopInput::KEY_MAP1[KEY_MAXCOUNT] =
	{
		0,

		VK_ESCAPE,

		VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10,
		VK_F11, VK_F12,

		'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',

		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
		'Z', 'X', 'C', 'V', 'B', 'N', 'M',

		VK_SHIFT, VK_SHIFT, VK_CONTROL, VK_CONTROL, VK_MENU, VK_MENU,

		VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT
	};

	const WORD DesktopInput::KEY_MAP2[KEY_MAXCOUNT] =
	{
		0,

		VK_ESCAPE,

		VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10,
		VK_F11, VK_F12,

		'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',

		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
		'Z', 'X', 'C', 'V', 'B', 'N', 'M',

		VK_LSHIFT, VK_RSHIFT, VK_LCONTROL, VK_RCONTROL, VK_LMENU, VK_RMENU,

		VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT
	};

	void DesktopInput::keyPress(Key key)
	{
		if (!(key > KEY_NONE && key < KEY_MAXCOUNT))
			THROW_BASE_EXCEPTION(Exception() << err_str(u8"错误的参数。"));

		INPUT input = {};
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = KEY_MAP1[key];
		input.ki.wScan = MapVirtualKey(KEY_MAP1[key], MAPVK_VK_TO_VSC);
		//input.ki.dwFlags = KEYEVENTF_SCANCODE;
		UINT ret = SendInput(1, &input, sizeof(input));
		if (ret != 1)
			THROW_WINDOWS_ERROR(GetLastError());
	}

	void DesktopInput::keyRelease(Key key)
	{
		if (!(key > KEY_NONE && key < KEY_MAXCOUNT))
			THROW_BASE_EXCEPTION(Exception() << err_str(u8"错误的参数。"));

		INPUT input = {};
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = KEY_MAP1[key];
		input.ki.wScan = MapVirtualKey(KEY_MAP1[key], MAPVK_VK_TO_VSC);
		input.ki.dwFlags = /*KEYEVENTF_SCANCODE | */KEYEVENTF_KEYUP;
		UINT ret = SendInput(1, &input, sizeof(input));
		if (ret != 1)
			THROW_WINDOWS_ERROR(GetLastError());
	}

	bool DesktopInput::isKeyPressed(Key key) const
	{
		if (!(key > KEY_NONE && key < KEY_MAXCOUNT))
			THROW_BASE_EXCEPTION(Exception() << err_str(u8"错误的参数。"));

		return (GetAsyncKeyState(KEY_MAP2[key]) & 0x8000);
	}

	void DesktopInput::mouseLeftPress()
	{
		INPUT input = {};
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		UINT ret = SendInput(1, &input, sizeof(input));
		if (ret != 1)
			THROW_WINDOWS_ERROR(GetLastError());
	}

	void DesktopInput::mouseLeftRelease()
	{
		INPUT input = {};
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		UINT ret = SendInput(1, &input, sizeof(input));
		if (ret != 1)
			THROW_WINDOWS_ERROR(GetLastError());
	}

	void DesktopInput::mouseRightPress()
	{
		INPUT input = {};
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
		UINT ret = SendInput(1, &input, sizeof(input));
		if (ret != 1)
			THROW_WINDOWS_ERROR(GetLastError());
	}

	void DesktopInput::mouseRightRelease()
	{
		INPUT input = {};
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
		UINT ret = SendInput(1, &input, sizeof(input));
		if (ret != 1)
			THROW_WINDOWS_ERROR(GetLastError());
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
