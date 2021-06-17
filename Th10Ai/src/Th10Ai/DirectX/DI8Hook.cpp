#include "Th10Ai/DirectX/DI8Hook.h"

#include <Base/Windows/SystemError.h>

#include "Th10Ai/DirectX/MyDirectInput8A.h"

namespace th
{
	DI8Hook g_di8Hook;

	HRESULT WINAPI DirectInput8CreateHook(HINSTANCE hinst, DWORD dwVersion,
		REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
	{
		return g_di8Hook.directInput8CreateHook(hinst, dwVersion, riidltf,
			ppvOut, punkOuter);
	}

	DI8Hook::DI8Hook() :
		m_directInput8CreateOrig(nullptr)
	{
	}

	void DI8Hook::attach(MyDetours& detours)
	{
		HMODULE dinput8Dll = GetModuleHandleW(L"dinput8.dll");
		if (dinput8Dll == nullptr)
			BASE_THROW(SystemError, GetLastError());
		m_directInput8CreateOrig =
			reinterpret_cast<DirectInput8Create_t>(
				GetProcAddress(dinput8Dll, "DirectInput8Create"));
		if (m_directInput8CreateOrig == nullptr)
			BASE_THROW(SystemError, GetLastError());

		detours.attach(reinterpret_cast<PVOID*>(&m_directInput8CreateOrig),
			DirectInput8CreateHook);
	}

	void DI8Hook::detach(MyDetours& detours)
	{
		detours.detach(reinterpret_cast<PVOID*>(&m_directInput8CreateOrig),
			DirectInput8CreateHook);
	}

	HRESULT DI8Hook::directInput8CreateHook(HINSTANCE hinst, DWORD dwVersion,
		REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
	{
		HRESULT hr = m_directInput8CreateOrig(hinst, dwVersion, riidltf,
			ppvOut, punkOuter);
		if (SUCCEEDED(hr))
		{
			if (riidltf == IID_IDirectInput8A)
				*ppvOut = new MyDirectInput8A(reinterpret_cast<IDirectInput8A*>(*ppvOut));
		}
		return hr;
	}
}
