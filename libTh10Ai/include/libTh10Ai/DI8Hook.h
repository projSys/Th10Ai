#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <atlbase.h>
#include <atomic>
#include <mutex>
#include <Base/Singleton.h>

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
		void commit();

	private:
		// IDirectInput8
		using DirectInput8Create_t = decltype(DirectInput8Create);
		// IDirectInputDevice8
		using GetDeviceStateW_t = HRESULT STDMETHODCALLTYPE(IDirectInputDevice8W*, DWORD, LPVOID);

		static HRESULT STDMETHODCALLTYPE GetDeviceStateHookW(IDirectInputDevice8W* device, DWORD size, LPVOID data);

		HRESULT getDeviceStateHookW(IDirectInputDevice8W* device, DWORD size, LPVOID data);

		std::atomic_bool m_enabled;
		GetDeviceStateW_t* m_getDeviceStateW;

		std::mutex m_keyMutex;
		KeyState m_writeState[256];
		KeyState m_readState[256];
		bool m_keyReadied;
	};
}
