#include "Th10Hook/DllLoader.h"

#include "Th10Hook/DllMain.h"

namespace th
{
	DllLoader::DllLoader() :
		Singleton(this),
		m_selfExit(true)
	{
	}

	void DllLoader::start()
	{
		m_mainThread = std::thread(&DllLoader::mainProc, this);
	}

	void DllLoader::join()
	{
		if (m_mainThread.joinable())
			m_mainThread.join();
	}

	void DllLoader::setSelfExit(bool selfExit)
	{
		m_selfExit = selfExit;
	}

	bool DllLoader::isSelfExit() const
	{
		return m_selfExit;
	}

	void DllLoader::mainProc()
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
				CreateThread(nullptr, 0, &DllLoader::ExitProc, this, 0, nullptr),
				&CloseHandle);
		}
	}

	DWORD DllLoader::ExitProc(LPVOID param)
	{
		DllLoader* dllLoader = reinterpret_cast<DllLoader*>(param);
		dllLoader->join();

		// 只在CreateThread创建的线程里有效
		FreeLibraryAndExitThread(g_dll, 0);
		return 0;
	}
}
