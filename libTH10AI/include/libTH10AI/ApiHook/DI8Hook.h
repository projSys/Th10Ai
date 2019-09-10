#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <atlbase.h>
#include <Base/Singleton.h>

#include "libTH10AI/ApiHook/HookFunc.h"

namespace th
{
	class DI8Hook :
		private Singleton<DI8Hook>
	{
	public:
		DI8Hook();
		~DI8Hook();

	private:
		// IDirectInput8
		typedef HRESULT(WINAPI *DirectInput8Create_t)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);
		// IDirectInputDevice8
		typedef HRESULT(STDMETHODCALLTYPE *GetDeviceState_t)(IDirectInputDevice8*, DWORD, LPVOID);

		static HRESULT STDMETHODCALLTYPE GetDeviceStateHook(IDirectInputDevice8* device, DWORD cbData, LPVOID vData);

		HRESULT getDeviceStateHook(IDirectInputDevice8* device, DWORD cbData, LPVOID vData);

		HookFunc<GetDeviceState_t> m_getDeviceState;
	};

	extern std::chrono::steady_clock::time_point g_getDeviceStateTime;
}
