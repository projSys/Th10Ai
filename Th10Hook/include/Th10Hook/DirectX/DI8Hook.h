#pragma once

#include "Th10Hook/Common.h"

#include <dinput.h>
#include <Base/Singleton.h>

#include "Th10Hook/MyDetours.h"

namespace th
{
	class DI8Hook :
		public Singleton<DI8Hook>
	{
	public:
		DI8Hook();

		void attach();
		void detach();

	private:
		using DirectInput8Create_t = decltype(&DirectInput8Create);

		static HRESULT WINAPI DirectInput8CreateHook(HINSTANCE hinst, DWORD dwVersion,
			REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);

		HRESULT directInput8CreateHook(HINSTANCE hinst, DWORD dwVersion,
			REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);

		DirectInput8Create_t m_directInput8CreateOrig;
	};
}
