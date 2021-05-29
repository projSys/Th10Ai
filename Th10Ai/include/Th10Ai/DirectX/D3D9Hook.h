#pragma once

#include "Th10Ai/Common.h"

#include <d3d9.h>
#include <Base/Singleton.h>

#include "Th10Ai/MyDetours.h"

namespace th
{
	class D3D9Hook :
		private Singleton<D3D9Hook>
	{
	public:
		D3D9Hook();

		void attach(MyDetours& detours);
		void detach(MyDetours& detours);

	private:
		using Direct3DCreate9_t = decltype(&Direct3DCreate9);

		static IDirect3D9* WINAPI Direct3DCreate9Hook(UINT SDKVersion);

		IDirect3D9* direct3DCreate9Hook(UINT SDKVersion);

		Direct3DCreate9_t m_direct3DCreate9Orig;
	};
}
