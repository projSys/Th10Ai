#pragma once

#include "Th10Ai/Common.h"

#include <dinput.h>
#include <Base/Singleton.h>

#include "Th10Ai/MyDetours.h"

namespace th
{
	class DI8Hook :
		private Singleton<DI8Hook>
	{
	public:
		DI8Hook();

		void attach(MyDetours& detours);
		void detach(MyDetours& detours);

	private:
		using DirectInput8Create_t = decltype(&DirectInput8Create);

		static HRESULT WINAPI DirectInput8CreateHook(HINSTANCE hinst, DWORD dwVersion,
			REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);

		HRESULT directInput8CreateHook(HINSTANCE hinst, DWORD dwVersion,
			REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);

		DirectInput8Create_t m_directInput8CreateOrig;
	};
}
