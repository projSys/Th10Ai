#include "TH10Bot/Common.h"
#include "TH10Bot/Key.h"

namespace th
{
	Key::Key(WORD vkey) :
		m_vkey(vkey),
		m_scan(MapVirtualKey(vkey, MAPVK_VK_TO_VSC)),
		m_pressed(false)
	{
	}

	Key::~Key()
	{
		try
		{
			release();
		}
		catch (...)
		{
		}
	}

	void Key::press()
	{
		if (!m_pressed)
		{
			INPUT input = {};
			input.type = INPUT_KEYBOARD;
			input.ki.wVk = m_vkey;
			input.ki.wScan = m_scan;
			//input.ki.dwFlags = KEYEVENTF_SCANCODE;
			UINT ret = SendInput(1, &input, sizeof(input));
			if (ret != 1)
				THROW_WINDOWS_ERROR(GetLastError());
			m_pressed = true;
		}
	}

	void Key::release()
	{
		if (m_pressed)
		{
			INPUT input = {};
			input.type = INPUT_KEYBOARD;
			input.ki.wVk = m_vkey;
			input.ki.wScan = m_scan;
			input.ki.dwFlags = /*KEYEVENTF_SCANCODE | */KEYEVENTF_KEYUP;
			UINT ret = SendInput(1, &input, sizeof(input));
			if (ret != 1)
				THROW_WINDOWS_ERROR(GetLastError());
			m_pressed = false;
		}
	}

	bool Key::isPressed() const
	{
		return m_pressed;
	}
}
