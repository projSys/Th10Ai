#include "libTH10AI/Common.h"
#include "libTH10AI/ApiHook/DI8Hook.h"

namespace th
{
	DI8Hook::DI8Hook(DI8Listener* listener) :
		Singleton(this),
		m_listener(listener),
		m_enabled(false)
	{
		if (listener == nullptr)
			THROW_BASE_EXCEPTION(Exception() << err_str("DI8Listener is null."));

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

		CComPtr<IDirectInputDevice8> device;
		hr = dinput8->CreateDevice(GUID_SysKeyboard, &device, nullptr);
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);

		uint_t* vTable = (uint_t*)(*((uint_t*)device.p));
		m_getDeviceState = HookFunc<GetDeviceState_t>(reinterpret_cast<LPVOID>(vTable[9]),
			&DI8Hook::GetDeviceStateHook);
	}

	void DI8Hook::enable(bool enabled)
	{
		m_enabled = enabled;
	}

	HRESULT DI8Hook::GetDeviceStateHook(IDirectInputDevice8* device, DWORD size, LPVOID data)
	{
		DI8Hook& di8Hook = DI8Hook::GetInstance();
		return di8Hook.getDeviceStateHook(device, size, data);
	}

	HRESULT DI8Hook::getDeviceStateHook(IDirectInputDevice8* device, DWORD size, LPVOID data)
	{
		if (!m_enabled)
			return m_getDeviceState(device, size, data);

		m_listener->onGetDeviceStateBegin(device, size, data);
		HRESULT hr = m_getDeviceState(device, size, data);
		m_listener->onGetDeviceStateEnd(hr, device, size, data);
		return hr;
	}
}
