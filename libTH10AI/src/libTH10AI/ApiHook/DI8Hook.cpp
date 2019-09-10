#include "libTH10AI/Common.h"
#include "libTH10AI/ApiHook/DI8Hook.h"

#include "libTH10AI/ApiHook/D3D9Hook.h"

namespace th
{
	DI8Hook::DI8Hook() :
		Singleton(this)
	{
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

	HRESULT DI8Hook::GetDeviceStateHook(IDirectInputDevice8* device, DWORD cbData, LPVOID vData)
	{
		DI8Hook& di8Hook = DI8Hook::GetInstance();
		return di8Hook.getDeviceStateHook(device, cbData, vData);
	}

	std::chrono::steady_clock::time_point g_getDeviceStateTime;

	HRESULT DI8Hook::getDeviceStateHook(IDirectInputDevice8* device, DWORD cbData, LPVOID vData)
	{
		// 迷之输入前的等待时间
		//g_getDeviceStateTime = std::chrono::steady_clock::now();
		//std::chrono::milliseconds interval = std::chrono::duration_cast<std::chrono::milliseconds>(
		//	g_getDeviceStateTime - g_presentEndTime);
		//std::cout << interval.count() << " ";

		HRESULT hr = m_getDeviceState(device, cbData, vData);

		return hr;
	}
}
