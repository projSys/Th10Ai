#pragma once

#include <memory>
#include <mutex>
#include <condition_variable>
#include <boost/thread/thread.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <Base/Singleton.h>

namespace th
{
	class Th10Ai;

	struct Th10Data
	{
		HWND window;
	};

	class Th10Context :
		public Singleton<Th10Context>
	{
	public:
		Th10Context();

		void onDllProcessAttach();

	private:
		void hookProc();

		static LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
		LRESULT windowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
		void onHook();
		void onUnhook();
		void onDestroy();

		boost::thread m_thread;

		string m_name;
		boost::interprocess::managed_shared_memory m_managedShm;
		Th10Data* m_data;

		HWND m_window;
		bool m_isUnicode;
		WNDPROC m_prevWndProc;
		bool m_isSelfExit;

		mutex m_mutex;
		condition_variable m_cond;

		std::shared_ptr<Th10Ai> m_th10Ai;
	};
}
