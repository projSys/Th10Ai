#include "libTH10AI/Common.h"
#include "libTH10AI/Input.h"

#include <thread>

#include "libTH10AI/FrameSync.h"

namespace th
{
	Input::Input() :
		m_keyReadied(false),
		m_inputDelayed(false),
		m_prevPresentTimespan(0),
		m_currInputTimespan(0),
		m_di8Hook(this)
	{
		for (KeyState& keyState : m_writeState)
			keyState = KS_NONE;
		for (KeyState& keyState : m_readState)
			keyState = KS_NONE;
	}

	std::chrono::steady_clock::time_point g_getDeviceStateTime;

	void Input::onGetDeviceStateBegin(IDirectInputDevice8* device, DWORD size, LPVOID data)
	{
		// 获取输入前的迷之等待时间
		g_getDeviceStateTime = std::chrono::steady_clock::now();
		//std::chrono::milliseconds interval = std::chrono::duration_cast<std::chrono::milliseconds>(
		//	g_getDeviceStateTime - g_presentEndTime);
		//std::cout << interval.count() << " ";
	}

	void Input::onGetDeviceStateEnd(HRESULT& hr, IDirectInputDevice8* device, DWORD size, LPVOID data)
	{
		if (FAILED(hr))
			return;
	AAA:
		// c_dfDIKeyboard
		if (size == 256 && data != nullptr)
		{
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
					m_inputDelayed = true;
					m_prevPresentTimespan = std::chrono::duration_cast<std::chrono::milliseconds>(
						g_presentEndTime - g_presentBeginTime).count();
					m_currInputTimespan = std::chrono::duration_cast<std::chrono::milliseconds>(
						g_getDeviceStateTime - g_presentEndTime).count();
				}
			}
			if (m_inputDelayed && m_prevPresentTimespan + m_currInputTimespan < 5)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(5 - m_prevPresentTimespan - m_currInputTimespan));
				g_getDeviceStateTime = std::chrono::steady_clock::now();
				goto AAA;
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

	void Input::commit(time_t handleTime)
	{
		std::lock_guard<std::mutex> lock(m_keyMutex);
		memcpy_s(m_readState, sizeof(m_readState), m_writeState, sizeof(m_writeState));
		m_keyReadied = true;
		if (m_inputDelayed)
		{
			std::cout << "操作延迟了：" << m_prevPresentTimespan << " " << m_currInputTimespan
				<< " " << handleTime << std::endl;
			m_inputDelayed = false;
		}
	}
}
