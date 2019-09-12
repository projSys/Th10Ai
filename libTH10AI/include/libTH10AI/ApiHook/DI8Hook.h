#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <atlbase.h>
#include <atomic>
#include <Base/Singleton.h>

#include "libTH10AI/ApiHook/HookFunc.h"

namespace th
{
	class DI8Listener
	{
	public:
		virtual ~DI8Listener() = default;
		virtual void onGetDeviceStateBegin(IDirectInputDevice8* device, DWORD size, LPVOID data) {}
		virtual void onGetDeviceStateEnd(HRESULT& hr, IDirectInputDevice8* device, DWORD size, LPVOID data) {}
	};

	class DI8Hook :
		private Singleton<DI8Hook>
	{
	public:
		DI8Hook(DI8Listener* listener);

		void enable(bool enabled);

	private:
		// IDirectInput8
		typedef HRESULT(WINAPI *DirectInput8Create_t)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);
		// IDirectInputDevice8
		typedef HRESULT(STDMETHODCALLTYPE *GetDeviceState_t)(IDirectInputDevice8*, DWORD, LPVOID);

		static HRESULT STDMETHODCALLTYPE GetDeviceStateHook(IDirectInputDevice8* device, DWORD size, LPVOID data);

		HRESULT getDeviceStateHook(IDirectInputDevice8* device, DWORD size, LPVOID data);

		DI8Listener* m_listener;
		std::atomic_bool m_enabled;
		HookFunc<GetDeviceState_t> m_getDeviceState;
	};
}
