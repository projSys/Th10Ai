#pragma once

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>
#include <atlbase.h>
#include <Base/Singleton.h>

namespace th
{
	class DI8Listener
	{
	public:
		virtual ~DI8Listener() = default;
		virtual void onGetDeviceStateW(IDirectInputDevice8W* device, DWORD size, LPVOID data) = 0;
	};

	class DI8Hook :
		private Singleton<DI8Hook>
	{
	public:
		DI8Hook(DI8Listener* listener);
		~DI8Hook();

	private:
		// IDirectInput8
		using DirectInput8Create_t = decltype(DirectInput8Create);
		// IDirectInputDevice8
		using GetDeviceStateW_t = HRESULT STDMETHODCALLTYPE(IDirectInputDevice8W*, DWORD, LPVOID);

		static HRESULT STDMETHODCALLTYPE GetDeviceStateHookW(IDirectInputDevice8W* device, DWORD size, LPVOID data);

		HRESULT getDeviceStateHookW(IDirectInputDevice8W* device, DWORD size, LPVOID data);

		DI8Listener* m_listener;
		GetDeviceStateW_t* m_getDeviceStateW;
	};
}
