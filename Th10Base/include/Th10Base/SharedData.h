#pragma once

#include "Th10Base/Common.h"

#include <atomic>

#include "Th10Base/SharedMemory.h"
#include "Th10Base/SharedStatus.h"
#include "Th10Base/Input.h"

namespace th
{
	class SharedData
	{
	public:
		explicit SharedData(SharedMemory& sharedMemory);

	public:
		// BUG：在托管共享内存中创建超过5个同步对象会导致析构异常
		//bip::interprocess_mutex initMutex;
		//bip::interprocess_condition initCond;
		bool inited;
		std::atomic<bool> active;
		HWND window;

		bip::interprocess_mutex statusMutex;
		bip::interprocess_condition statusCond;
		bool statusUpdated;
		bool exit;
		ManagedPtr<SharedStatus> writableStatus;
		ManagedPtr<SharedStatus> swappableStatus;
		ManagedPtr<SharedStatus> readableStatus;

		bip::interprocess_mutex inputMutex;
		bip::interprocess_condition inputCond;
		bool inputUpdated;
		ManagedPtr<Input> writableInput;
		ManagedPtr<Input> swappableInput;
		ManagedPtr<Input> readableInput;
	};
}
