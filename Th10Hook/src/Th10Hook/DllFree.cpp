#include "Th10Hook/DllFree.h"

#include "Th10Hook/DllMain.h"
#include "Th10Hook/DllLoader.h"

namespace th
{
	void DllFree::SelfFree()
	{
		try
		{
			HANDLE freeThread = CreateThread(nullptr, 0,
				&DllFree::FreeProc, nullptr, 0, nullptr);
			if (freeThread == nullptr)
				BASE_THROW(WindowsError());
			CloseHandle(freeThread);
		}
		catch (...)
		{
			BASE_LOG_FATAL(PrintException());
		}
	}

	DWORD DllFree::FreeProc(LPVOID)
	{
		DllLoader& dllLoader = DllLoader::GetInstance();
		dllLoader.join();

		// 只在CreateThread创建的线程里有效
		FreeLibraryAndExitThread(g_dll, 0);
		return 0;
	}
}
