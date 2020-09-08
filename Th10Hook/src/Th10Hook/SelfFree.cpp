#include "Th10Hook/SelfFree.h"

#include "Th10Hook/HookLoader.h"
#include "Th10Hook/DllMain.h"

namespace th
{
	void SelfFree::Free()
	{
		HANDLE_ptr freeThread(
			CreateThread(nullptr, 0, &SelfFree::FreeProc, nullptr, 0, nullptr),
			&CloseHandle);
	}

	DWORD SelfFree::FreeProc(LPVOID)
	{
		HookLoader& hookLoader = HookLoader::GetInstance();
		hookLoader.join();

		// ֻ��CreateThread�������߳�����Ч
		FreeLibraryAndExitThread(g_dll, 0);
		return 0;
	}
}
