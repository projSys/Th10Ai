#pragma once

#include "Th10Hook/Common.h"

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>
#include <atlbase.h>
#include <Base/TypeTraits.h>
#include <Base/Singleton.h>

namespace th
{
	class DI8Listener
	{
	public:
		virtual ~DI8Listener() = default;
		virtual void onGetDeviceStateA(IDirectInputDevice8A* device, DWORD size, LPVOID data) = 0;
	};

	class DI8Hook :
		private Singleton<DI8Hook>
	{
	public:
		DI8Hook(DI8Listener* listener);
		~DI8Hook();

	private:
		// IDirectInput8
		using DirectInput8Create_t = decltype(&DirectInput8Create);
		// IDirectInputDevice8
		using GetDeviceStateA_t = FuncTraits<decltype(&IDirectInputDevice8A::GetDeviceState)>::Raw_t;

		static HRESULT STDMETHODCALLTYPE GetDeviceStateHookA(IDirectInputDevice8A* device, DWORD size, LPVOID data);

		HRESULT getDeviceStateHookA(IDirectInputDevice8A* device, DWORD size, LPVOID data);

		DI8Listener* m_listener;
		GetDeviceStateA_t m_getDeviceStateOrigA;
	};
}
