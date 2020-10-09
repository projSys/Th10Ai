#include "Th10Hook/DI8Hook.h"

#include <Base/Windows/DxResult.h>

#include "Th10Hook/MyDetours.h"

namespace th
{
	DI8Hook::DI8Hook(DI8Listener* listener) :
		Singleton(this),
		m_listener(listener),
		m_getDeviceStateOrigA(nullptr)
	{
		HMODULE dinput8Dll = GetModuleHandleW(L"dinput8.dll");
		if (dinput8Dll == nullptr)
			BASE_THROW(WindowsError());
		DirectInput8Create_t* directInput8Create = reinterpret_cast<DirectInput8Create_t*>(
			GetProcAddress(dinput8Dll, "DirectInput8Create"));
		if (directInput8Create == nullptr)
			BASE_THROW(WindowsError());

		CComPtr<IDirectInput8A> dinput8A;
		HRESULT hr = directInput8Create(GetModuleHandleW(nullptr), DIRECTINPUT_VERSION,
			IID_IDirectInput8A, reinterpret_cast<LPVOID*>(&dinput8A), nullptr);
		if (FAILED(hr))
			BASE_THROW(DxResult(hr, u8"DirectInput8Create()调用失败。"));

		CComPtr<IDirectInputDevice8A> deviceA;
		hr = dinput8A->CreateDevice(GUID_SysKeyboard, &deviceA, nullptr);
		if (FAILED(hr))
			BASE_THROW(DxResult(hr, u8"CreateDevice()调用失败。"));

		uint_t* vTableA = reinterpret_cast<uint_t*>(*reinterpret_cast<uint_t*>(deviceA.p));
		m_getDeviceStateOrigA = reinterpret_cast<GetDeviceStateA_t*>(vTableA[9]);

		MyDetours::TransactionBegin();
		MyDetours::Attach(reinterpret_cast<PVOID*>(&m_getDeviceStateOrigA), &DI8Hook::GetDeviceStateHookA);
		MyDetours::TransactionCommit();
	}

	DI8Hook::~DI8Hook()
	{
		try
		{
			MyDetours::TransactionBegin();
			MyDetours::Detach(reinterpret_cast<PVOID*>(&m_getDeviceStateOrigA), &DI8Hook::GetDeviceStateHookA);
			MyDetours::TransactionCommit();
		}
		catch (...)
		{
			BASE_LOG_FATAL(PrintException());
		}
	}

	HRESULT DI8Hook::GetDeviceStateHookA(IDirectInputDevice8A* device, DWORD size, LPVOID data)
	{
		DI8Hook& di8Hook = DI8Hook::GetInstance();
		return di8Hook.getDeviceStateHookA(device, size, data);
	}

	HRESULT DI8Hook::getDeviceStateHookA(IDirectInputDevice8A* device, DWORD size, LPVOID data)
	{
		HRESULT hr = m_getDeviceStateOrigA(device, size, data);

		try
		{
			m_listener->onGetDeviceStateA(device, size, data);
		}
		catch (...)
		{
			BASE_LOG_ERROR(PrintException());
		}

		return hr;
	}
}
