#pragma once

#include "Th10Hook/Common.h"

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include "Th10Hook/Th10Types.h"

namespace th
{
	namespace interprocess = boost::interprocess;

	class SharedData
	{
	public:
		SharedData();

		HWND getWindow() const;

		void notifyInit();
		void notifyUninit();
		void waitUninit();
		void notifyUpdate();
		void notifyExit();

		StatusData& getStatus();
		ActionData& getAction();
		bool isActionUpdated();

	private:
		HWND m_window;

		interprocess::interprocess_mutex m_initMutex;
		interprocess::interprocess_condition m_initCond;
		bool m_inited;

		interprocess::interprocess_mutex m_uninitMutex;
		interprocess::interprocess_condition m_uninitCond;
		bool m_uninited;

		interprocess::interprocess_mutex m_updateMutex;
		interprocess::interprocess_condition m_updateCond;
		bool m_updated;
		bool m_exited;

		int64_t m_updateTime;
		StatusData m_status;
		ActionData m_action;
		bool m_actionUpdated;
	};
}
