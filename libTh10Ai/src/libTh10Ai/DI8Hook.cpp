#include "libTh10Ai/Common.h"
#include "libTh10Ai/DI8Hook.h"

#include "libTh10Ai/MyDetours.h"
#include "libTh10Ai/D3D9Hook.h"

namespace th
{
	DI8Hook::DI8Hook() :
		Singleton(this),
		m_enabled(false),
		m_getDeviceStateW(nullptr),
		m_keyReadied(false)
	{
		for (KeyState& keyState : m_writeState)
			keyState = KS_NONE;
		for (KeyState& keyState : m_readState)
			keyState = KS_NONE;

		HMODULE dinput8Dll = GetModuleHandle(_T("dinput8.dll"));
		if (dinput8Dll == nullptr)
			BASE_THROW(WindowsError(GetLastError()));
		DirectInput8Create_t directInput8Create = reinterpret_cast<DirectInput8Create_t>(
			GetProcAddress(dinput8Dll, "DirectInput8Create"));
		if (directInput8Create == nullptr)
			BASE_THROW(WindowsError(GetLastError()));

		CComPtr<IDirectInput8W> dinput8W;
		HRESULT hr = directInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION,
			IID_IDirectInput8W, reinterpret_cast<LPVOID*>(&dinput8W), nullptr);
		if (FAILED(hr))
			BASE_THROW(DirectXResult(hr, DXVER::DI8));

		CComPtr<IDirectInputDevice8W> deviceW;
		hr = dinput8W->CreateDevice(GUID_SysKeyboard, &deviceW, nullptr);
		if (FAILED(hr))
			BASE_THROW(DirectXResult(hr, DXVER::DI8));

		uint_t* vTableW = reinterpret_cast<uint_t*>(*reinterpret_cast<uint_t*>(deviceW.p));
		m_getDeviceStateW = reinterpret_cast<GetDeviceStateW_t>(vTableW[9]);

		MyDetours detours;
		detours.attach(reinterpret_cast<PVOID*>(&m_getDeviceStateW), &DI8Hook::GetDeviceStateHookW);
	}

	DI8Hook::~DI8Hook()
	{
		MyDetours detours;
		detours.detach(reinterpret_cast<PVOID*>(&m_getDeviceStateW), &DI8Hook::GetDeviceStateHookW);
	}

	void DI8Hook::enable(bool enabled)
	{
		m_enabled = enabled;
	}

	HRESULT DI8Hook::GetDeviceStateHookW(IDirectInputDevice8W* device, DWORD size, LPVOID data)
	{
		DI8Hook& instance = DI8Hook::GetInstance();
		return instance.getDeviceStateHookW(device, size, data);
	}

	std::chrono::steady_clock::time_point g_getDeviceStateTime;

	HRESULT DI8Hook::getDeviceStateHookW(IDirectInputDevice8W* device, DWORD size, LPVOID data)
	{
		if (!m_enabled)
			return m_getDeviceStateW(device, size, data);

		g_getDeviceStateTime = std::chrono::steady_clock::now();
		time_t e1 = std::chrono::duration_cast<std::chrono::milliseconds>(g_presentEndTime - g_presentBeginTime).count();
		time_t e2 = std::chrono::duration_cast<std::chrono::milliseconds>(g_getDeviceStateTime - g_presentEndTime).count();
		//std::cout << e1 << " " << e2 << std::endl;
		if (e1 + e2 < 5)
			std::cout << "计算时间太少了：" << e1 + e2 << std::endl;

		HRESULT hr = m_getDeviceStateW(device, size, data);

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

		return hr;
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

	void DI8Hook::commit()
	{
		std::lock_guard<std::mutex> lock(m_keyMutex);
		memcpy_s(m_readState, sizeof(m_readState), m_writeState, sizeof(m_writeState));
		m_keyReadied = true;
	}
}
