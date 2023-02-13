#include "Th10Hook/Di8Hook.h"

#include <Base/Exception/SystemError.h>

#include "Th10Hook/InlineHook32.h"
#include "Th10Hook/MyDirectInput8A.h"

#pragma warning(disable: 6387)

namespace th
{
	Di8Hook::Di8Hook() :
		Singleton(this),
		m_directInput8CreateOrig(nullptr)
	{
	}

	void Di8Hook::attach(MyDetours& detours)
	{
		HMODULE dinput8Dll = GetModuleHandleW(L"dinput8.dll");
		if (dinput8Dll == nullptr)
			Throw(SystemError(GetLastError()));
		m_directInput8CreateOrig = reinterpret_cast<DirectInput8Create_t*>(
			GetProcAddress(dinput8Dll, "DirectInput8Create"));
		if (m_directInput8CreateOrig == nullptr)
			Throw(SystemError(GetLastError()));

		detours.attach(reinterpret_cast<PVOID*>(&m_directInput8CreateOrig), &Di8Hook::DirectInput8CreateHook);
		//InlineHook32 hook;
		//m_directInput8CreateOrig = reinterpret_cast<DirectInput8Create_t*>(
		//	hook.attach(m_directInput8CreateOrig, &Di8Hook::DirectInput8CreateHook));
	}

	void Di8Hook::detach(MyDetours& detours)
	{
		detours.detach(reinterpret_cast<PVOID*>(&m_directInput8CreateOrig), &Di8Hook::DirectInput8CreateHook);
	}

	HRESULT Di8Hook::DirectInput8CreateHook(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
	{
		Di8Hook& instance = Di8Hook::GetInstance();
		return instance.directInput8CreateHook(hinst, dwVersion, riidltf, ppvOut, punkOuter);
	}

	HRESULT Di8Hook::directInput8CreateHook(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
	{
		HRESULT hr = m_directInput8CreateOrig(hinst, dwVersion, riidltf, ppvOut, punkOuter);
		if (SUCCEEDED(hr))
		{
			if (riidltf == IID_IDirectInput8A)
				*ppvOut = new MyDirectInput8A(reinterpret_cast<IDirectInput8A*>(*ppvOut));
		}
		return hr;
	}
}
