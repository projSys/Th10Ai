#include "Th10Hook/DirectX/DI8Hook.h"

#include <Base/Windows/Module.h>

#include "Th10Hook/DirectX/DXResult.h"
#include "Th10Hook/MyDetours.h"

namespace th
{
	DI8Hook::DI8Hook(DI8Listener* listener) :
		Singleton(this),
		m_listener(listener),
		m_getDeviceStateOrigA(nullptr)
	{
		Module dinput8Dll = Module::Get("dinput8.dll");
		DirectInput8Create_t directInput8Create =
			dinput8Dll.getProcAddress<DirectInput8Create_t>("DirectInput8Create");

		CComPtr<IDirectInput8A> di8A;
		HRESULT hr = directInput8Create(GetModuleHandleW(nullptr), DIRECTINPUT_VERSION,
			IID_IDirectInput8A, reinterpret_cast<LPVOID*>(&di8A), nullptr);
		if (FAILED(hr))
			BASE_THROW(DXResult(hr, u8"DirectInput8Create()调用失败。"));

		CComPtr<IDirectInputDevice8A> deviceA;
		hr = di8A->CreateDevice(GUID_SysKeyboard, &deviceA, nullptr);
		if (FAILED(hr))
			BASE_THROW(DXResult(hr, u8"CreateDevice()调用失败。"));

		uint_t* vTableA = reinterpret_cast<uint_t*>(*reinterpret_cast<uint_t*>(deviceA.p));
		m_getDeviceStateOrigA = reinterpret_cast<GetDeviceStateA_t>(vTableA[9]);

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
