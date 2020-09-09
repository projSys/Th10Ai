#pragma once

#include "Th10Ai/Common.h"

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include "Th10Ai/Th10Data.h"

namespace th
{
	namespace interprocess = boost::interprocess;

	class SharedData
	{
	public:
		SharedData();

		void setWindow(HWND window);
		HWND getWindow() const;
		void activate();

		bool timedWaitHook(int64_t ms);
		void notifyUnhook();
		bool waitUpdate();

		const StatusData& getStatus() const;
		ActionData& getAction();
		void commit();

	private:
		HWND m_window;

		interprocess::interprocess_mutex m_hookMutex;
		interprocess::interprocess_condition m_hookCond;
		bool m_hooked;

		interprocess::interprocess_mutex m_unhookMutex;
		interprocess::interprocess_condition m_unhookCond;
		bool m_unhooked;

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
