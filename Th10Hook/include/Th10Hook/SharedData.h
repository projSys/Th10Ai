#pragma once

#include "Th10Hook/Common.h"

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include "Th10Hook/Th10Data.h"

namespace th
{
	namespace interprocess = boost::interprocess;

	class SharedData
	{
	public:
		SharedData();

		HWND getWindow() const;

		void notifyHook();
		void notifyUnhook();
		void waitUnhook();
		void notifyUpdate();
		void notifyExit();

		void update();
		ActionData& getAction();
		bool isActionUpdated();

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
