#pragma once

#include <memory>
#include <boost/interprocess/managed_windows_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <Base/Singleton.h>

#include "Th10Hook/DllApp.h"

namespace th
{
	class Th10Ai;

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

	class Th10Hook :
		public DllApp,
		public Singleton<Th10Hook>
	{
	public:
		Th10Hook();

	private:
		virtual int_t run() override;

		static LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
		LRESULT windowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
		void onHook();
		void onUnhook();
		void onDestroy();

		int_t m_exitCode;

		boost::interprocess::managed_windows_shared_memory m_sharedMemory;
		Th10HookData* m_data;

		HWND m_window;
		bool m_isUnicode;
		WNDPROC m_prevWndProc;

		unique_ptr<Th10Ai> m_th10Ai;
		bool m_isHooked;
	};
}
