#include "Th10Base/SharedInput.h"

#include <dinput.h>

namespace th
{
	SharedInput::SharedInput()
	{
		clear();
	}

	void SharedInput::clear()
	{
		m_dir = Direction::HOLD;
		m_shoot = false;
		m_bomb = false;
		m_slow = false;
		m_skip = false;
	}

	void SharedInput::move(Direction dir)
	{
		m_dir = dir;
	}

	void SharedInput::shoot()
	{
		m_shoot = true;
	}

	void SharedInput::bomb()
	{
		m_bomb = true;
	}

	void SharedInput::slow()
	{
		m_slow = true;
	}

	void SharedInput::skip()
	{
		m_skip = true;
	}

	void SharedInput::commitTo(DWORD size, LPVOID data) const
	{
		// c_dfDIKeyboard
		if (size == 256 && data != nullptr)
		{
			BYTE* keyState = reinterpret_cast<BYTE*>(data);

			switch (m_dir)
			{
			case Direction::HOLD:
				keyState[DIK_UP] = 0x00;
				keyState[DIK_DOWN] = 0x00;
				keyState[DIK_LEFT] = 0x00;
				keyState[DIK_RIGHT] = 0x00;
				break;

			case Direction::UP:
				keyState[DIK_UP] = 0x80;
				keyState[DIK_DOWN] = 0x00;
				keyState[DIK_LEFT] = 0x00;
				keyState[DIK_RIGHT] = 0x00;
				break;

			case Direction::DOWN:
				keyState[DIK_UP] = 0x00;
				keyState[DIK_DOWN] = 0x80;
				keyState[DIK_LEFT] = 0x00;
				keyState[DIK_RIGHT] = 0x00;
				break;

			case Direction::LEFT:
				keyState[DIK_UP] = 0x00;
				keyState[DIK_DOWN] = 0x00;
				keyState[DIK_LEFT] = 0x80;
				keyState[DIK_RIGHT] = 0x00;
				break;

			case Direction::RIGHT:
				keyState[DIK_UP] = 0x00;
				keyState[DIK_DOWN] = 0x00;
				keyState[DIK_LEFT] = 0x00;
				keyState[DIK_RIGHT] = 0x80;
				break;

			case Direction::UP_LEFT:
				keyState[DIK_UP] = 0x80;
				keyState[DIK_DOWN] = 0x00;
				keyState[DIK_LEFT] = 0x80;
				keyState[DIK_RIGHT] = 0x00;
				break;

			case Direction::UP_RIGHT:
				keyState[DIK_UP] = 0x80;
				keyState[DIK_DOWN] = 0x00;
				keyState[DIK_LEFT] = 0x00;
				keyState[DIK_RIGHT] = 0x80;
				break;

			case Direction::DOWN_LEFT:
				keyState[DIK_UP] = 0x00;
				keyState[DIK_DOWN] = 0x80;
				keyState[DIK_LEFT] = 0x80;
				keyState[DIK_RIGHT] = 0x00;
				break;

			case Direction::DOWN_RIGHT:
				keyState[DIK_UP] = 0x00;
				keyState[DIK_DOWN] = 0x80;
				keyState[DIK_LEFT] = 0x00;
				keyState[DIK_RIGHT] = 0x80;
				break;
			}

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
