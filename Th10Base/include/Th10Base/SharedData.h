#pragma once

#include "Th10Base/Common.h"

#include <atomic>
#include <Base/Time.h>

#include "Th10Base/SharedMemory.h"
#include "Th10Base/SharedStatus.h"
#include "Th10Base/SharedInput.h"

namespace th
{
	class SharedData
	{
	public:
		explicit SharedData(SharedMemory& sharedMemory);

		void notifyInit();
		bool waitInit(const Time& timeout);
		bool isActive() const;
		void setActive(bool active);
		HWND getWindow() const;
		void setWindow(HWND window);

		void notifyExit();
		void notifyUpdate();
		bool waitUpdate();
		bool waitUpdate(const Time& timeout);
		bool isExit() const;
		SharedStatus& getWritableStatus();
		const SharedStatus& getReadableStatus() const;

		void notifyInput();
		void waitInput();
		bool waitInput(const Time& timeout);
		SharedInput& getWritableInput();
		const SharedInput& getReadableInput() const;

		//void checkUpdate();
		//void checkInput();
		//Time updateTime;
		//Time inputTime;

	private:
		bip::interprocess_mutex m_initMutex;
		bip::interprocess_condition m_initCond;
		bool m_inited;
		std::atomic<bool> m_active;
		HWND m_window;

		bip::interprocess_mutex m_statusMutex;
		bip::interprocess_condition m_statusCond;
		bool m_statusUpdated;
		bool m_exit;
		ManagedPtr<SharedStatus> m_writableStatus;
		ManagedPtr<SharedStatus> m_swappableStatus;
		ManagedPtr<SharedStatus> m_readableStatus;

		bip::interprocess_mutex m_inputMutex;
		bip::interprocess_condition m_inputCond;
		bool m_inputUpdated;
		ManagedPtr<SharedInput> m_writableInput;
		ManagedPtr<SharedInput> m_swappableInput;
		ManagedPtr<SharedInput> m_readableInput;
	};
}
