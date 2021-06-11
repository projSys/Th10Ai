#pragma once

#include "Th10Ai/Common.h"

#include <d3d9.h>

#include "Th10Ai/MyDetours.h"

namespace th
{
	class D3D9Hook
	{
	public:
		D3D9Hook();

		void attach(MyDetours& detours);
		void detach(MyDetours& detours);

	private:
		using Direct3DCreate9_t = decltype(&Direct3DCreate9);

		IDirect3D9* direct3DCreate9Hook(UINT SDKVersion);

		Direct3DCreate9_t m_direct3DCreate9Orig;

		friend IDirect3D9* WINAPI Direct3DCreate9Hook(UINT SDKVersion);
	};

	extern D3D9Hook g_d3d9Hook;
}
