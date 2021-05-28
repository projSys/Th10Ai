#include "Th10Hook/Input.h"

#include <dinput.h>

namespace th
{
	Input::Input()
	{
		clear();
	}

	void Input::clear()
	{
		m_up = false;
		m_down = false;
		m_left = false;
		m_right = false;
		m_shoot = false;
		m_bomb = false;
		m_slow = false;
		m_skip = false;
	}

	void Input::move(DIR dir)
	{
		switch (dir)
		{
		case DIR::HOLD:
			break;

		case DIR::UP:
			m_up = true;
			break;

		case DIR::DOWN:
			m_down = true;
			break;

		case DIR::LEFT:
			m_left = true;
			break;

		case DIR::RIGHT:
			m_right = true;
			break;

		case DIR::UP_LEFT:
			m_up = true;
			m_left = true;
			break;

		case DIR::UP_RIGHT:
			m_up = true;
			m_right = true;
			break;

		case DIR::DOWN_LEFT:
			m_down = true;
			m_left = true;
			break;

		case DIR::DOWN_RIGHT:
			m_down = true;
			m_right = true;
			break;
		}
	}

	void Input::shoot()
	{
		m_shoot = true;
	}

	void Input::bomb()
	{
		m_bomb = true;
	}

	void Input::slow()
	{
		m_slow = true;
	}

	void Input::skip()
	{
		m_skip = true;
	}

	void Input::commit(DWORD size, LPVOID data)
	{
		// c_dfDIKeyboard
		if (size == 256 && data != nullptr)
		{
			BYTE* keyState = reinterpret_cast<BYTE*>(data);

			if (m_up)
				keyState[DIK_UP] = 0x80;
			else
				keyState[DIK_UP] = 0x00;

			if (m_down)
				keyState[DIK_DOWN] = 0x80;
			else
				keyState[DIK_DOWN] = 0x00;

			if (m_left)
				keyState[DIK_LEFT] = 0x80;
			else
				keyState[DIK_LEFT] = 0x00;

			if (m_right)
				keyState[DIK_RIGHT] = 0x80;
			else
				keyState[DIK_RIGHT] = 0x00;

			if (m_shoot)
				keyState[DIK_Z] = 0x80;
			else
				keyState[DIK_Z] = 0x00;

			if (m_bomb)
				keyState[DIK_X] = 0x80;
			else
				keyState[DIK_X] = 0x00;

			if (m_slow)
				keyState[DIK_LSHIFT] = 0x80;
			else
				keyState[DIK_LSHIFT] = 0x00;

			if (m_skip)
				keyState[DIK_LCONTROL] = 0x80;
			else
				keyState[DIK_LCONTROL] = 0x00;
		}
		// c_dfDIMouse
		//sizeof(DIMOUSESTATE);
		// c_dfDIMouse2
		//sizeof(DIMOUSESTATE2);
		// c_dfDIJoystick
		//sizeof(DIJOYSTATE);
		// c_dfDIJoystick2
		//sizeof(DIJOYSTATE2);
	}
}
