#pragma once

#include <mutex>

#include "libTH10AI/ApiHook/DI8Hook.h"

namespace th
{
	enum KeyState
	{
		KS_NONE = -1,

		KS_RELEASE,
		KS_PRESS,

		KS_MAXCOUNT
	};

	class Input :
		private DI8Listener
	{
	public:
		Input();

		void enable(bool enabled);
		void clear();
		void keyClear(uint8_t key);
		void keyPress(uint8_t key);
		void keyRelease(uint8_t key);
		bool isKeyPressed(uint8_t key) const;
		void commit(time_t handleTime);

	private:
		virtual void onGetDeviceStateBegin(IDirectInputDevice8* device, DWORD size, LPVOID data) override;
		virtual void onGetDeviceStateEnd(HRESULT& hr, IDirectInputDevice8* device, DWORD size, LPVOID data) override;

		KeyState m_writeState[256];
		KeyState m_readState[256];
		std::mutex m_keyMutex;
		bool m_keyReadied;
		bool m_inputDelayed;
		time_t m_prevPresentTimespan;
		time_t m_currInputTimespan;

		DI8Hook m_di8Hook;
	};

	extern std::chrono::steady_clock::time_point g_getDeviceStateTime;
}
