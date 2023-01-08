#include "Th10Hook/D3d9Hook.h"

#include <Base/Error.h>

#include "Th10Hook/MyDirect3D9.h"

namespace th
{
	D3d9Hook g_d3d9Hook;

	D3d9Hook::D3d9Hook() :
		Singleton(this),
		m_direct3DCreate9Orig(nullptr)
	{
	}

	void D3d9Hook::attach(MyDetours& detours)
	{
		HMODULE d3d9Dll = GetModuleHandleW(L"d3d9.dll");
		if (d3d9Dll == nullptr)
			throw Error(GetLastError());
		m_direct3DCreate9Orig = reinterpret_cast<Direct3DCreate9_t>(GetProcAddress(d3d9Dll, "Direct3DCreate9"));
		if (m_direct3DCreate9Orig == nullptr)
			throw Error(GetLastError());

		detours.attach(reinterpret_cast<PVOID*>(&m_direct3DCreate9Orig), Direct3DCreate9Hook);
	}

	void D3d9Hook::detach(MyDetours& detours)
	{
		detours.detach(reinterpret_cast<PVOID*>(&m_direct3DCreate9Orig), Direct3DCreate9Hook);
	}

	IDirect3D9* D3d9Hook::Direct3DCreate9Hook(UINT SDKVersion)
	{
		D3d9Hook& instance = D3d9Hook::GetInstance();
		return instance.direct3DCreate9Hook(SDKVersion);
	}

	IDirect3D9* D3d9Hook::direct3DCreate9Hook(UINT SDKVersion)
	{
		IDirect3D9* d3d9 = m_direct3DCreate9Orig(SDKVersion);
		if (d3d9 != nullptr)
			d3d9 = new MyDirect3D9(d3d9);
		return d3d9;
	}
}
