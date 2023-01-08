#pragma once

#include "Th10Base/Common.h"

#include <atomic>
#include <Base/Time.h>

#include "Th10Base/SharedMemory.h"
#include "Th10Base/SharedStatus.h"
#include "Th10Base/Input.h"

namespace th
{
	class SharedData
	{
	public:
		explicit SharedData(SharedMemory& sharedMemory);

		void notifyInit();
		bool waitInit(const Time& timeout = Time(3000));
		HWND getWindow() const;
		void setWindow(HWND window);
		bool isActive() const;
		void setActive(bool active);

		void notifyExit();
		void notifyUpdate();
		bool waitUpdate();
		SharedStatus& getWritableStatus();
		const SharedStatus& getReadableStatus() const;

		void notifyInput();
		bool waitInput(const Time& timeout = Time(10));
		Input& getWritableInput();
		const Input& getReadableInput() const;

	private:
		ip::interprocess_mutex m_initMutex;
		ip::interprocess_condition m_initCond;
		bool m_inited;
		HWND m_window;
		std::atomic<bool> m_active;

		ip::interprocess_mutex m_statusMutex;
		ip::interprocess_condition m_statusCond;
		bool m_statusUpdated;
		bool m_exit;
		ManagedPtr<SharedStatus> m_writableStatus;
		ManagedPtr<SharedStatus> m_swappableStatus;
		ManagedPtr<SharedStatus> m_readableStatus;

		ip::interprocess_mutex m_inputMutex;
		ip::interprocess_condition m_inputCond;
		bool m_inputUpdated;
		ManagedPtr<Input> m_writableInput;
		ManagedPtr<Input> m_swappableInput;
		ManagedPtr<Input> m_readableInput;
	};
}
