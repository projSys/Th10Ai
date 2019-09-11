#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <atlbase.h>
#include <atomic>
#include <mutex>
#include <Base/Singleton.h>

#include "libTH10AI/ApiHook/HookFunc.h"

namespace th
{
	enum KeyState
	{
		KS_NONE = -1,

		KS_RELEASE,
		KS_PRESS,

		KS_MAXCOUNT
	};

	class DI8Hook :
		private Singleton<DI8Hook>
	{
	public:
		DI8Hook();
		~DI8Hook();

		void enable(bool enabled);
		void clear();
		void keyClear(uint8_t key);
		void keyPress(uint8_t key);
		void keyRelease(uint8_t key);
		bool isKeyPressed(uint8_t key) const;
		void commit(time_t handleTime);

	private:
		// IDirectInput8
		typedef HRESULT(WINAPI *DirectInput8Create_t)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);
		// IDirectInputDevice8
		typedef HRESULT(STDMETHODCALLTYPE *GetDeviceState_t)(IDirectInputDevice8*, DWORD, LPVOID);

		static HRESULT STDMETHODCALLTYPE GetDeviceStateHook(IDirectInputDevice8* device, DWORD cbData, LPVOID lpvData);

		HRESULT getDeviceStateHook(IDirectInputDevice8* device, DWORD cbData, LPVOID lpvData);

		std::atomic_bool m_enabled;
		KeyState m_writeState[256];
		KeyState m_readState[256];
		std::mutex m_keyMutex;
		bool m_keyReadied;
		bool m_inputDelayed;
		time_t m_prevPresentTime;
		time_t m_currInputTime;

		HookFunc<GetDeviceState_t> m_getDeviceState;
	};

	extern std::chrono::steady_clock::time_point g_getDeviceStateTime;
}
