#include "Th10Hook/SelfFree.h"

#include "Th10Hook/HookLoader.h"
#include "Th10Hook/DllMain.h"

namespace th
{
	void SelfFree::Free()
	{
		try
		{
			HANDLE freeThread = CreateThread(nullptr, 0,
				&SelfFree::FreeProc, nullptr, 0, nullptr);
			if (freeThread == nullptr)
				BASE_THROW(WindowsError());
			CloseHandle(freeThread);
		}
		catch (...)
		{
			BASE_LOG_ERROR(PrintException());
		}
	}

	DWORD SelfFree::FreeProc(LPVOID)
	{
		HookLoader& hookLoader = HookLoader::GetInstance();
		hookLoader.join();

		// 只在CreateThread创建的线程里有效
		FreeLibraryAndExitThread(g_dll, 0);
		return 0;
	}
}
