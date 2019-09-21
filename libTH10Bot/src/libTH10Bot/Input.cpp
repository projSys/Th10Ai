#include "libTH10Bot/Common.h"
#include "libTH10Bot/Input.h"

#include <thread>

#include "libTH10Bot/FrameSync.h"

namespace th
{
	Input::Input() :
		m_keyReadied(false),
		m_keyDelayed(false),
		m_di8Hook(this)
	{
		std::lock_guard<std::mutex> lock(m_keyMutex);
		for (KeyState& keyState : m_writeState)
			keyState = KS_NONE;
		for (KeyState& keyState : m_readState)
			keyState = KS_NONE;
	}

	std::chrono::steady_clock::time_point g_getDeviceStateTime;
	time_t g_smoothTimespan;

	void Input::onGetDeviceStateBegin(IDirectInputDevice8* device, DWORD size, LPVOID data)
	{
		// 帧平滑机制：获取输入前会有等待时间
		g_getDeviceStateTime = std::chrono::steady_clock::now();
		std::chrono::milliseconds interval = std::chrono::duration_cast<std::chrono::milliseconds>(
			g_getDeviceStateTime - g_presentEndTime);
		g_smoothTimespan = interval.count();
		//std::cout << interval.count() << " ";

		// c_dfDIKeyboard
		if (size == 256 && data != nullptr)
		{
			//// 如果前一帧的垂直同步等待时间 + 当前帧的平滑等待时间 < 5毫秒，则延时一下
			//if (g_presentTimespan + g_smoothTimespan < 5)
			//{
			//	time_t delayTimespan = 5 - g_presentTimespan - g_smoothTimespan;
			//	std::cout << "作弊延时: " << delayTimespan << std::endl;
			//	std::this_thread::sleep_for(std::chrono::milliseconds(delayTimespan));
			//	g_getDeviceStateTime = std::chrono::steady_clock::now();
			//}
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

	void Input::onGetDeviceStateEnd(HRESULT& hr, IDirectInputDevice8* device, DWORD size, LPVOID data)
	{
		if (FAILED(hr))
		{
			std::cout << "GetDeviceState()失败。" << std::endl;
			return;
		}

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
				m_keyDelayed = false;
			}
			else
			{
				m_keyDelayed = true;
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
		if (m_keyDelayed)
		{
			std::chrono::steady_clock::time_point inputTime = std::chrono::steady_clock::now();
			std::chrono::milliseconds interval = std::chrono::duration_cast<std::chrono::milliseconds>(
				inputTime - g_presentBeginTime);
			std::cout << "输入延迟了：" << g_presentTimespan + g_smoothTimespan
				<< " " << interval.count() << std::endl;
			m_keyDelayed = false;
		}
	}
}
