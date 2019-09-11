#include "libTH10AI/Common.h"
#include "libTH10AI/ApiHook/DI8Hook.h"

#include "libTH10AI/ApiHook/D3D9Hook.h"

namespace th
{
	DI8Hook::DI8Hook() :
		Singleton(this),
		m_enabled(false),
		m_keyReadied(false),
		m_inputDelayed(false),
		m_prevPresentTime(0),
		m_currInputTime(0)
	{
		for (KeyState& keyState : m_writeState)
			keyState = KS_NONE;
		for (KeyState& keyState : m_readState)
			keyState = KS_NONE;

		HMODULE dinput8Dll = GetModuleHandle(_T("dinput8.dll"));
		if (dinput8Dll == nullptr)
			THROW_WINDOWS_ERROR(GetLastError());
		DirectInput8Create_t directInput8Create = reinterpret_cast<DirectInput8Create_t>(
			GetProcAddress(dinput8Dll, "DirectInput8Create"));
		if (directInput8Create == nullptr)
			THROW_WINDOWS_ERROR(GetLastError());

		CComPtr<IDirectInput8> dinput8;
		HRESULT hr = directInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION,
			IID_IDirectInput8, reinterpret_cast<LPVOID*>(&dinput8), nullptr);
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);

		CComPtr<IDirectInputDevice8> keyboard;
		hr = dinput8->CreateDevice(GUID_SysKeyboard, &keyboard, nullptr);
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);

		uint_t* vTable = (uint_t*)(*((uint_t*)keyboard.p));
		m_getDeviceState = HookFunc<GetDeviceState_t>(reinterpret_cast<LPVOID>(vTable[9]),
			&DI8Hook::GetDeviceStateHook);
	}

	DI8Hook::~DI8Hook()
	{
	}

	HRESULT DI8Hook::GetDeviceStateHook(IDirectInputDevice8* device, DWORD cbData, LPVOID lpvData)
	{
		DI8Hook& di8Hook = DI8Hook::GetInstance();
		return di8Hook.getDeviceStateHook(device, cbData, lpvData);
	}

	std::chrono::steady_clock::time_point g_getDeviceStateTime;

	HRESULT DI8Hook::getDeviceStateHook(IDirectInputDevice8* device, DWORD cbData, LPVOID lpvData)
	{
		// 获取输入前的迷之等待时间
		g_getDeviceStateTime = std::chrono::steady_clock::now();
		//std::chrono::milliseconds interval = std::chrono::duration_cast<std::chrono::milliseconds>(
		//	g_getDeviceStateTime - g_presentEndTime);
		//std::cout << interval.count() << " ";

		if (!m_enabled)
			return m_getDeviceState(device, cbData, lpvData);

		HRESULT hr = m_getDeviceState(device, cbData, lpvData);
		if (FAILED(hr))
			return hr;

		// c_dfDIKeyboard
		if (cbData == 256 && lpvData != nullptr)
		{
			std::lock_guard<std::mutex> lock(m_keyMutex);
			if (m_keyReadied)
			{
				BYTE* keyState = reinterpret_cast<BYTE*>(lpvData);
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
				m_prevPresentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
					g_presentEndTime - g_presentBeginTime).count();
				m_currInputTime = std::chrono::duration_cast<std::chrono::milliseconds>(
					g_getDeviceStateTime - g_presentEndTime).count();
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

		return hr;
	}

	void DI8Hook::enable(bool enabled)
	{
		m_enabled = enabled;
	}

	void DI8Hook::clear()
	{
		for (KeyState& keyState : m_writeState)
			keyState = KS_NONE;
	}

	void DI8Hook::keyClear(uint8_t key)
	{
		m_writeState[key] = KS_NONE;
	}

	void DI8Hook::keyPress(uint8_t key)
	{
		m_writeState[key] = KS_PRESS;
	}

	void DI8Hook::keyRelease(uint8_t key)
	{
		m_writeState[key] = KS_RELEASE;
	}

	bool DI8Hook::isKeyPressed(uint8_t key) const
	{
		return m_writeState[key] == KS_PRESS;
	}

	void DI8Hook::commit(time_t handleTime)
	{
		std::lock_guard<std::mutex> lock(m_keyMutex);
		memcpy_s(m_readState, sizeof(m_readState), m_writeState, sizeof(m_writeState));
		m_keyReadied = true;
		if (m_inputDelayed)
		{
			std::cout << "操作延迟了：" << m_prevPresentTime << " " << m_currInputTime << " " << handleTime << std::endl;
			m_inputDelayed = false;
		}
	}
}
