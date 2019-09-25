#include "libTH10Bot/Common.h"
#include "libTH10Bot/Input.h"

namespace th
{
	Input::Input() :
		m_keyReadied(false),
		m_di8Hook(this)
	{
		std::lock_guard<std::mutex> lock(m_keyMutex);
		for (KeyState& keyState : m_writeState)
			keyState = KS_NONE;
		for (KeyState& keyState : m_readState)
			keyState = KS_NONE;
	}

	void Input::onGetDeviceStateBegin(IDirectInputDevice8* device, DWORD size, LPVOID data)
	{
	}

	void Input::onGetDeviceStateEnd(HRESULT& hr, IDirectInputDevice8* device, DWORD size, LPVOID data)
	{
		// c_dfDIKeyboard
		if (size == 256 && data != nullptr)
		{
			std::lock_guard<std::mutex> lock(m_keyMutex);
			if (m_keyReadied)
			{
				BYTE* keyState = reinterpret_cast<BYTE*>(data);
				for (int_t i = 0; i < 256; ++i)
				{
					switch (m_readState[i])
					{
					case KS_RELEASE:
						keyState[i] = 0x00;
						break;
					case KS_PRESS:
						keyState[i] = 0x80;
						break;
					}
				}
				m_keyReadied = false;
			}
			else
			{
				std::cout << "输入不及时。" << std::endl;
			}
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

	void Input::enable(bool enabled)
	{
		m_di8Hook.enable(enabled);
	}

	void Input::clear()
	{
		for (KeyState& keyState : m_writeState)
			keyState = KS_NONE;
	}

	void Input::keyClear(uint8_t key)
	{
		m_writeState[key] = KS_NONE;
	}

	void Input::keyPress(uint8_t key)
	{
		m_writeState[key] = KS_PRESS;
	}

	void Input::keyRelease(uint8_t key)
	{
		m_writeState[key] = KS_RELEASE;
	}

	bool Input::isKeyPressed(uint8_t key) const
	{
		return m_writeState[key] == KS_PRESS;
	}

	void Input::commit()
	{
		std::lock_guard<std::mutex> lock(m_keyMutex);
		memcpy_s(m_readState, sizeof(m_readState), m_writeState, sizeof(m_writeState));
		m_keyReadied = true;
	}
}
