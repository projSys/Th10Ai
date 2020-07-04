#pragma once

#include <boost/interprocess/managed_windows_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <wx/wx.h>

namespace th
{
	struct Th10HookData
	{
		HWND window;

		boost::interprocess::interprocess_mutex hookMutex;
		boost::interprocess::interprocess_condition hookCond;
		bool isHooked;

		boost::interprocess::interprocess_mutex unhookMutex;
		boost::interprocess::interprocess_condition unhookCond;
		bool isUnhook;
	};

	class MyFrame :
		public wxFrame
	{
	public:
		MyFrame();
		virtual ~MyFrame();

	private:
		void onClose(wxCloseEvent& event);

		boost::interprocess::managed_windows_shared_memory m_sharedMemory;
		Th10HookData* m_data;

		wxDECLARE_EVENT_TABLE();
	};
}
