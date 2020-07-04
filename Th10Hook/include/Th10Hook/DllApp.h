#pragma once

#include <thread>
#include <Base/Singleton.h>

namespace th
{
	class DllApp :
		public Singleton<DllApp>
	{
	public:
		DllApp();
		virtual ~DllApp() = default;

		void start();
		void join();

	private:
		void appProc();
		static DWORD WINAPI ExitProc(LPVOID param);

		// 返回0表示dll自己退出，其他表示和所在进程一起退出。
		virtual int_t run() = 0;

		thread m_appThread;
	};
}
