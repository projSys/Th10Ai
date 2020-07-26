#include "Th10Hook/Common.h"
#include "Th10Hook/DI8Hook.h"

#include "Th10Hook/OldDxResult.h"
#include "Th10Hook/MyDetours.h"

namespace th
{
	DI8Hook::DI8Hook(DI8Listener* listener) :
		Singleton(this),
		m_listener(listener),
		m_getDeviceStateW(nullptr)
	{
		HMODULE dinput8Dll = GetModuleHandleW(L"dinput8.dll");
		if (dinput8Dll == nullptr)
			BASE_THROW(WindowsError());
		DirectInput8Create_t* directInput8Create = reinterpret_cast<DirectInput8Create_t*>(
			GetProcAddress(dinput8Dll, "DirectInput8Create"));
		if (directInput8Create == nullptr)
			BASE_THROW(WindowsError());

		CComPtr<IDirectInput8W> dinput8W;
		HRESULT hr = directInput8Create(GetModuleHandleW(nullptr), DIRECTINPUT_VERSION,
			IID_IDirectInput8W, reinterpret_cast<LPVOID*>(&dinput8W), nullptr);
		if (FAILED(hr))
			BASE_THROW(OldDxResult(hr));

		CComPtr<IDirectInputDevice8W> deviceW;
		hr = dinput8W->CreateDevice(GUID_SysKeyboard, &deviceW, nullptr);
		if (FAILED(hr))
			BASE_THROW(OldDxResult(hr));

		uint_t* vTableW = reinterpret_cast<uint_t*>(*reinterpret_cast<uint_t*>(deviceW.p));
		m_getDeviceStateW = reinterpret_cast<GetDeviceStateW_t*>(vTableW[9]);

		MyDetours detours;
		detours.attach(reinterpret_cast<PVOID*>(&m_getDeviceStateW), &DI8Hook::GetDeviceStateHookW);
	}

	DI8Hook::~DI8Hook()
	{
		MyDetours detours;
		detours.detach(reinterpret_cast<PVOID*>(&m_getDeviceStateW), &DI8Hook::GetDeviceStateHookW);
	}

	HRESULT DI8Hook::GetDeviceStateHookW(IDirectInputDevice8W* device, DWORD size, LPVOID data)
	{
		DI8Hook& di8Hook = DI8Hook::GetInstance();
		return di8Hook.getDeviceStateHookW(device, size, data);
	}

	HRESULT DI8Hook::getDeviceStateHookW(IDirectInputDevice8W* device, DWORD size, LPVOID data)
	{
		HRESULT hr = m_getDeviceStateW(device, size, data);

		try
		{
			m_listener->onGetDeviceStateW(device, size, data);
		}
		catch (...)
		{
			BASE_LOG_ERROR(PrintException());
		}

		return hr;
	}
}
