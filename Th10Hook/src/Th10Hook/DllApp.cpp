#include "Th10Hook/DllApp.h"

#include "Th10Hook/DllMain.h"

namespace th
{
	DllApp::DllApp() :
		Singleton(this),
		m_selfExit(true)
	{
	}

	void DllApp::start()
	{
		m_mainThread = std::thread(&DllApp::mainProc, this);
	}

	void DllApp::join()
	{
		if (m_mainThread.joinable())
			m_mainThread.join();
	}

	void DllApp::setSelfExit(bool selfExit)
	{
		m_selfExit = selfExit;
	}

	bool DllApp::isSelfExit() const
	{
		return m_selfExit;
	}

	void DllApp::mainProc()
	{
		try
		{
			run();
		}
		catch (...)
		{
			BASE_LOG_ERROR(PrintException());
		}

		if (m_selfExit)
		{
			HANDLE_ptr exitThread(
				CreateThread(nullptr, 0, &DllApp::ExitProc, this, 0, nullptr),
				&CloseHandle);
		}
	}

	DWORD DllApp::ExitProc(LPVOID param)
	{
		DllApp* dllApp = reinterpret_cast<DllApp*>(param);
		dllApp->join();

		// 只在CreateThread创建的线程里有效
		FreeLibraryAndExitThread(g_dll, 0);
		return 0;
	}
}
