#include "Th10Ai/DirectX/D3D9Hook.h"

#include <system_error>

#include "Th10Ai/DirectX/MyDirect3D9.h"

namespace th
{
	D3D9Hook::D3D9Hook() :
		Singleton(this)
	{
		HMODULE d3d9Dll = GetModuleHandleW(L"d3d9.dll");
		if (d3d9Dll == nullptr)
			BASE_THROW(std::system_error(GetLastError(), std::system_category()));
		m_direct3DCreate9Orig =
			reinterpret_cast<Direct3DCreate9_t>(
				GetProcAddress(d3d9Dll, "Direct3DCreate9"));
		if (m_direct3DCreate9Orig == nullptr)
			BASE_THROW(std::system_error(GetLastError(), std::system_category()));
	}

	void D3D9Hook::attach(MyDetours& detours)
	{
		detours.attach(reinterpret_cast<PVOID*>(&m_direct3DCreate9Orig),
			Direct3DCreate9Hook);
	}

	void D3D9Hook::detach(MyDetours& detours)
	{
		detours.detach(reinterpret_cast<PVOID*>(&m_direct3DCreate9Orig),
			Direct3DCreate9Hook);
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
			d3d9 = new MyDirect3D9(d3d9);
		return d3d9;
	}
}
