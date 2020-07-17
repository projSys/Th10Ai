#pragma once

#include <thread>
#include <atomic>
#include <boost/interprocess/managed_windows_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include "Th10Ai/Status.h"

namespace th
{
	struct Th10SharedData
	{
		HWND window;

		boost::interprocess::interprocess_mutex hookMutex;
		boost::interprocess::interprocess_condition hookCond;
		bool isHooked;

		boost::interprocess::interprocess_mutex unhookMutex;
		boost::interprocess::interprocess_condition unhookCond;
		bool isUnhook;

		Status_t status;

		boost::interprocess::interprocess_mutex updateMutex;
		boost::interprocess::interprocess_condition updateCond;
		bool isUpdated;
	};

	class Th10Context
	{
	public:
		Th10Context();
		~Th10Context();

	private:
		void threadProc();

		thread m_thread;
		atomic_bool m_isDone;

		boost::interprocess::managed_windows_shared_memory m_memory;
		Th10SharedData* m_data;
	};
}
