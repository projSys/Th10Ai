#include "Th10Hook/Common.h"
#include "Th10Hook/DllApp.h"

#include "Th10Hook/DllMain.h"

namespace th
{

	DllApp::DllApp() :
		Singleton(this)
	{
	}

	void DllApp::start()
	{
		m_appThread = thread(&DllApp::appProc, this);
	}

	void DllApp::join()
	{
		if (m_appThread.joinable())
			m_appThread.join();
		m_appThread = thread();
	}

	void DllApp::appProc()
	{
		int_t exitCode = run();
		if (exitCode == 0)
		{
			HANDLE_ptr exitThread = HANDLE_ptr(reinterpret_cast<HANDLE>(
				CreateThread(nullptr, 0, &DllApp::ExitProc, this, 0, nullptr)),
				&CloseHandle);
			//if (exitThread == nullptr)
		}
	}

	DWORD DllApp::ExitProc(LPVOID param)
	{
		DllApp* dllApp = reinterpret_cast<DllApp*>(param);
		dllApp->join();

		FreeLibraryAndExitThread(g_dll, 0);
		return 0;
	}
}
