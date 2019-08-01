#include "GraphHook/Common.h"
#include "GraphHook/DI8Hook.h"

#include <boost/interprocess/sync/scoped_lock.hpp>

namespace gh
{
	DI8Hook::DI8Hook() :
		Singleton(this)
	{
		m_memory = bip::managed_shared_memory(bip::create_only, "DI8SharedMemory", 65536);
		m_data = m_memory.construct<DI8SharedData>("DI8SharedData")();
		memset(m_data->keyState, KS_NONE, sizeof(m_data->keyState));
		memset(m_keyState, KS_NONE, sizeof(m_keyState));

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

		uint_t* vtable = (uint_t*)(*((uint_t*)keyboard.p));
		m_getDeviceStateFunc = HookFunc<GetDeviceState_t>(reinterpret_cast<LPVOID>(vtable[9]),
			&DI8Hook::GetDeviceStateHook);
	}

	DI8Hook::~DI8Hook()
	{
		m_memory.destroy<DI8SharedData>("DI8SharedData");
	}

	HRESULT DI8Hook::GetDeviceStateHook(IDirectInputDevice8* device, DWORD cbData, LPVOID lpvData)
	{
		DI8Hook& di8Hook = DI8Hook::GetInstance();
		return di8Hook.getDeviceStateHook(device, cbData, lpvData);
	}

	HRESULT DI8Hook::getDeviceStateHook(IDirectInputDevice8* device, DWORD cbData, LPVOID lpvData)
	{
		HRESULT hr = m_getDeviceStateFunc(device, cbData, lpvData);
		// 东方无效
		if (hr == DIERR_INPUTLOST)
		{
			device->Acquire();
			hr = DI_OK;
		}

		try
		{
			// c_dfDIKeyboard
			if (cbData == 256)
			{
				{
					bip::scoped_lock<bip::interprocess_mutex> lock(m_data->keyMutex);
					memcpy_s(m_keyState, sizeof(m_keyState), m_data->keyState, sizeof(m_data->keyState));
				}
				BYTE* keyState = reinterpret_cast<BYTE*>(lpvData);
				for (int_t i = 0; i < 256; ++i)
				{
					if (m_keyState[i] == KS_PRESS)
						keyState[i] = 0x80;
					else if (m_keyState[i] == KS_RELEASE)
						keyState[i] = 0x00;
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
		catch (...)
		{
			//std::string what = boost::current_exception_diagnostic_information();
			//BOOST_LOG_TRIVIAL(error) << what;
		}

		return hr;
	}
}
