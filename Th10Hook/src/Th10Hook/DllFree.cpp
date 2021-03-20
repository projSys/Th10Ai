#include "Th10Hook/DllFree.h"

#include <Base/Windows/Thread.h>

#include "Th10Hook/DllMain.h"
#include "Th10Hook/DllLoader.h"

namespace th
{
	void DllFree::SelfFree()
	{
		try
		{
			Thread freeThread = Thread::Create(nullptr, 0,
				&DllFree::FreeProc, nullptr, 0);
		}
		catch (...)
		{
			BASE_LOG_ERROR(PrintException());
		}
	}

	DWORD DllFree::FreeProc(LPVOID)
	{
		DllLoader& dllLoader = DllLoader::GetInstance();
		dllLoader.join();

		// ֻ��CreateThread�������߳�����Ч
		FreeLibraryAndExitThread(g_module, 0);
		return 0;
	}
}
