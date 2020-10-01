#pragma once

#include "Th10Ai/Common.h"

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include "Th10Ai/Th10Types.h"

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

		bool waitInit(int64_t ms);

		void notifyUninit();

		bool waitUpdate();

		StatusData& getStatus();
		ActionData& getAction();
		void commit();

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
