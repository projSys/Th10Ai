#include "Th10Hook/DirectX/DI8Hook.h"

#include <Base/Windows/Module.h>

#include "Th10Hook/DirectX/MyDirectInput8A.h"

namespace th
{
	DI8Hook::DI8Hook() :
		Singleton(this)
	{
		Module dinput8Dll = Module::Get("dinput8.dll");
		m_directInput8CreateOrig =
			dinput8Dll.getProcAddress<DirectInput8Create_t>("DirectInput8Create");
	}

	void DI8Hook::attach(MyDetours& detours)
	{
		detours.attach(reinterpret_cast<PVOID*>(&m_directInput8CreateOrig),
			&DI8Hook::DirectInput8CreateHook);
	}

	void DI8Hook::detach(MyDetours& detours)
	{
		detours.detach(reinterpret_cast<PVOID*>(&m_directInput8CreateOrig),
			&DI8Hook::DirectInput8CreateHook);
	}

	HRESULT DI8Hook::DirectInput8CreateHook(HINSTANCE hinst, DWORD dwVersion,
		REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
	{
		DI8Hook& di8Hook = DI8Hook::GetInstance();
		return di8Hook.directInput8CreateHook(hinst, dwVersion, riidltf, ppvOut, punkOuter);
	}

	HRESULT DI8Hook::directInput8CreateHook(HINSTANCE hinst, DWORD dwVersion,
		REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
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
