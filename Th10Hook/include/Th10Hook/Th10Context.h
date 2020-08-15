#pragma once

#include <boost/interprocess/managed_windows_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <Base/Clock.h>

#include "Th10Hook/RawStatus.h"

namespace th
{
	struct Th10SharedData
	{
		HWND window;

		boost::interprocess::interprocess_mutex hookMutex;
		boost::interprocess::interprocess_condition hookCond;
		bool hooked;

		boost::interprocess::interprocess_mutex unhookMutex;
		boost::interprocess::interprocess_condition unhookCond;
		bool unhooked;

		boost::interprocess::interprocess_mutex updateMutex;
		boost::interprocess::interprocess_condition updateCond;
		bool updated;
		bool exited;

		time_t updateTime;
		RawStatus status;
	};

	class Th10Context
	{
	public:
		Th10Context();
		~Th10Context();

		HWND getWindow() const;

		void notifyHook();
		void notifyUnhook();
		void waitUnhook();
		void notifyUpdate();
		void notifyExit();

		void update();

	private:
		boost::interprocess::managed_windows_shared_memory m_memory;
		Th10SharedData* m_data;
		Clock m_clock;
	};
}
