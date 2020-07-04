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

		// ����0��ʾdll�Լ��˳���������ʾ�����ڽ���һ���˳���
		virtual int_t run() = 0;

		thread m_appThread;
	};
}
