#include "Th10Hook/DirectX/D3D9Hook.h"

#include <Base/Windows/Module.h>

#include "Th10Hook/DirectX/MyDirect3D9.h"

namespace th
{
	D3D9Hook::D3D9Hook() :
		Singleton(this)
	{
		Module d3d9Dll = Module::Get("d3d9.dll");
		m_direct3DCreate9Orig =
			d3d9Dll.getProcAddress<Direct3DCreate9_t>("Direct3DCreate9");
	}

	void D3D9Hook::attach(MyDetours& detours)
	{
		detours.attach(reinterpret_cast<PVOID*>(&m_direct3DCreate9Orig),
			&D3D9Hook::Direct3DCreate9Hook);
	}

	void D3D9Hook::detach(MyDetours& detours)
	{
		detours.detach(reinterpret_cast<PVOID*>(&m_direct3DCreate9Orig),
			&D3D9Hook::Direct3DCreate9Hook);
	}

	IDirect3D9* D3D9Hook::Direct3DCreate9Hook(UINT SDKVersion)
	{
		D3D9Hook& d3d9Hook = D3D9Hook::GetInstance();
		return d3d9Hook.direct3DCreate9Hook(SDKVersion);
	}

	IDirect3D9* D3D9Hook::direct3DCreate9Hook(UINT SDKVersion)
	{
		IDirect3D9* d3d9 = m_direct3DCreate9Orig(SDKVersion);
		if (d3d9 != nullptr)
			d3d9 = new th::MyDirect3D9(d3d9);
		return d3d9;
	}
}
