#pragma once

#include "Th10Ai/Common.h"

#include <dinput.h>

#include "Th10Ai/MyDetours.h"

namespace th
{
	class DI8Hook
	{
	public:
		DI8Hook();

		void attach(MyDetours& detours);
		void detach(MyDetours& detours);

	private:
		using DirectInput8Create_t = decltype(&DirectInput8Create);

		HRESULT directInput8CreateHook(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);

		DirectInput8Create_t m_directInput8CreateOrig;

		friend HRESULT WINAPI DirectInput8CreateHook(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);
	};

	extern DI8Hook g_di8Hook;
}
