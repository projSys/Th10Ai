#pragma once

#include <thread>
#include <atomic>
#include <Base/Singleton.h>

namespace th
{
	class DllLoader :
		public Singleton<DllLoader>
	{
	public:
		DllLoader();
		virtual ~DllLoader() = default;

		void start();
		void join();

		void setSelfExit(bool selfExit);
		bool isSelfExit() const;

	private:
		void mainProc();
		virtual void run() = 0;

		static DWORD WINAPI ExitProc(LPVOID param);

		thread m_mainThread;
		atomic_bool m_selfExit;
	};
}
